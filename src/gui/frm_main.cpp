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
#include <QTimer>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include <QStandardPaths>
#include <QSettings>
#include <jsoncons/json.hpp>

MainForm::MainForm( QMainWindow* aParent ) : QMainWindow( aParent ), 
        ui( new Ui::MainForm ), mTreeModel( new TreeModel( this )) 
{
    ui->setupUi( this ); 

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

    // read the json file
    QFile file( fileName );
    if ( !file.open( QFile::ReadOnly ) ) {
        qDebug() << "Could not open file for reading";
        return;
    }

    // save the last opened directory in QSettings
    settings.setValue( "lastDir", QFileInfo( fileName ).absolutePath());

    QString fileContent = file.readAll();

    ui->textBrowser->setPlainText( fileContent );

    // parse the json file

    jsoncons::ojson jsonData = jsoncons::ojson::parse( 
            fileContent.toStdString());

    // create a tree model
    this->mTreeModel->buildTree( jsonData );
    
    ui->treeView->setModel( mTreeModel );

    file.close();
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
onMounted() {
    // code to be executed after the main window shows up

}
