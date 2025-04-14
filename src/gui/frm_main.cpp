/// @file frm_main.cpp
/// Definitions for the main form.
///
/// @version 0.1
/// @author Anirudha Jadhav <anirudhasj441@gmail.com>
///
/// (C)2025 Coding Devil.
/// https://www.codingdevil.in/

#include "ui_frm_main.h"
#include "frm_main.h"
#include "models/tree_model.h"
#include "models/tree_model_delegate.h"
#include <QTimer>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include <QStandardPaths>
#include <QSettings>
#include <jsoncons/json.hpp>
#include <jsoncons/pretty_print.hpp>

MainForm::MainForm( QMainWindow* aParent ) : QMainWindow( aParent ), 
        ui( new Ui::MainForm ), mTreeModel( new TreeModel( this )) 
{
    ui->setupUi( this ); 

    connect( this->mTreeModel, &QAbstractItemModel::dataChanged, this,
            &MainForm::treeModel_DataChanged );

    connect( this, &MainForm::fileSavedChanged, this, &MainForm::mainForm_FileSavedChanged );

    /// call onMounted after the ui is mounted
    QTimer::singleShot( 0, this, &MainForm::onMounted ); 
}

MainForm::
~MainForm() {
   
}

void MainForm::
actionLoadJson_Triggered() {
    // Code to execute on load json action

    QSettings settings( "CodingDevil", "JsonViewer" );

    // checking if there last open dir is present in QSetting
    QString lastDir = settings.value( 
        "lastDir",
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
    ).toString();

    // open file dialog to import json file
    QString fileName = QFileDialog::getOpenFileName(
        this, 
        "Open JSON File", 
        lastDir,
        tr("JSON Files (*.json)")
    );
    
    // check if the file name is empty
    if ( fileName.isEmpty() ) {
        qDebug() << "No file selected";
        return;
    }

    this->mOpenFilePath = fileName;

    // read the json file
    QFile file( this->mOpenFilePath );
    if ( !file.open( QFile::ReadOnly ) ) {
        qDebug() << "Could not open file for reading";
        return;
    }

    // save the last opened directory in QSettings
    settings.setValue( "lastDir", QFileInfo( this->mOpenFilePath )
            .absolutePath());

    QString fileContent = file.readAll();

    // parse the json file

    jsoncons::ojson jsonData = jsoncons::ojson::parse( 
            fileContent.toStdString());

    // create a tree model
    this->mTreeModel->buildTree( jsonData );
    
    ui->treeView->setModel( this->mTreeModel );

    ui->treeView->setItemDelegate( new TreeModelDelegate( this ));

    file.close();

    this->setFileSaved( true );

    std::ostringstream os;

    os << pretty_print( this->mTreeModel->getJson());

    ui->textBrowser->setPlainText(  
            QString::fromStdString( os.str()));
}

void MainForm::
actionSave_Triggered() {
    if ( this->mOpenFilePath.isEmpty() ) { return; }

    QFile file( this->mOpenFilePath );

    if( !file.open( QFile::WriteOnly )) {
        qDebug() << "File could not open!";
        return;
    }

    std::ostringstream os;

    os << pretty_print( this->mTreeModel->getJson() );

    file.write( QString::fromStdString( os.str()).toUtf8());

    file.close();
    qDebug() << "File saved!";

    this->setFileSaved( true );

}

void MainForm::
actionSaveAs_Triggered() {
    if ( this->mOpenFilePath.isEmpty() ) { return; }

    QSettings settings( "CodingDevil", "JsonViewer" );

    // checking if there last open dir is present in QSetting
    QString lastDir = settings.value( 
        "lastDir/saveAs",
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
    ).toString();

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save As",
        lastDir,
        tr("JSON Files (*.json)")
    );

    if( fileName.isEmpty()) {
        qDebug() << "File not selected by user!!";

        return;
    }

    this->mOpenFilePath = fileName;

    settings.setValue( "lastDir/saveAs", QFileInfo( this->mOpenFilePath ).
            absolutePath());

    QFile file( this->mOpenFilePath );

    if( !file.open( QFile::WriteOnly )) {
        qDebug() << "File could not open!";
        return;
    }

    std::ostringstream os;

    os << pretty_print( this->mTreeModel->getJson() );

    file.write( QString::fromStdString( os.str()).toUtf8());

    file.close();

    this->setFileSaved( true );
    qDebug() << "File saved as: " << this->mOpenFilePath;
}

void MainForm::
treeView_Expanded( const QModelIndex& aIndex ) {
    // code to execute when tree view is expanded
    TreeNode* node = static_cast<TreeNode*>( aIndex.internalPointer() );
    node->expanded = true;
    this->mTreeModel->dataChanged( aIndex, aIndex );
}

void MainForm::
treeView_Collapsed( const QModelIndex& aIndex ) {
    // code to execute when tree view is collapsed
    TreeNode* node = static_cast<TreeNode*>( aIndex.internalPointer() );
    node->expanded = false;
    this->mTreeModel->dataChanged( aIndex, aIndex );
}

void MainForm::
treeModel_DataChanged( const QModelIndex &aTopLeft, 
        const QModelIndex &aBottomRight, const QList<int> &aRoles) 
{
    qDebug() << "Data changed" ;

    QFile file( this->mOpenFilePath );

    if ( !file.open( QFile::ReadOnly )) {
        qDebug() << "File Coluld not be opened";
        return;
    }

    jsoncons::ojson fileJson = jsoncons::ojson::parse( file.readAll());

    if( this->mTreeModel->getJson() == fileJson ) {
        qDebug() << "File not changed!";
        return ;
    }

    file.close();

    std::ostringstream os;
    os << jsoncons::pretty_print( this->mTreeModel->getJson());

    ui->textBrowser->setPlainText(  
            QString::fromStdString( os.str()));

    this->setFileSaved( false );
}

void MainForm::
setFileSaved( bool aValue ) {
    this->mFileSaved = aValue;

    emit this->fileSavedChanged();
}

void MainForm::
mainForm_FileSavedChanged( ) {
    QFileInfo fileInfo( this->mOpenFilePath );
    QString saveIndicator = this->mFileSaved ? "" : " *";
    this->setWindowTitle( QString("Json Editor - %1 %2").arg(
        fileInfo.fileName(), saveIndicator ));
}

void MainForm::
onMounted() {
    // code to be executed after the main window shows up

}
