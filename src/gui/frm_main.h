/// @file frm_main.h
/// Declaration for the main form.
///
/// @version 0.1
/// @author Anirudha Jadhav <<anirudhasj441@gmail.com>
///
/// (C)2025 Coding Devil.
/// https://www.codingdevil.in/

#ifndef FRM_MAIN_H
#define	FRM_MAIN_H

#include <QMainWindow>
#include <jsoncons/json.hpp>

namespace Ui {
    class MainForm;
}

class TreeModel;

class MainForm : public QMainWindow {
    Q_OBJECT

public:
    /// Constructor
    MainForm( QMainWindow* aParent = nullptr );

    // Destructor
    virtual ~MainForm();

private:
    Ui::MainForm* ui{ nullptr };

    TreeModel* mTreeModel{ nullptr };

    QString mOpenFilePath;

    bool mFileSaved{ false };

private slots:
    void actionLoadJson_Triggered();

    void actionSave_Triggered();

    void actionSaveAs_Triggered();

    void treeView_Expanded( const QModelIndex& aIndex );

    void treeView_Collapsed( const QModelIndex& aIndex );

    void treeModel_DataChanged( const QModelIndex &topLeft, 
            const QModelIndex &bottomRight, 
            const QList<int> &roles = QList<int>()
    );

    void treeModel_ModelReset();

    void mainForm_FileSavedChanged();

private:

    /// function to set file saved status
    ///
    /// @param[ in ] aValue file saved status
    ///
    /// return 
    void setFileSaved( bool aValue ); 

    /// function for savng file
    ///
    /// @param[ in ] aFilePath file path to save
    ///
    /// @return void
    void saveFile( const QString aFilePath );

    /// function to dump json to QSTring
    ///
    /// @param[ in ] aJson json data
    ///
    /// @return QString json data in string format
    QString jsonToString( const jsoncons::ojson& aJson ) const;

    /// Function call after ui is mounted
    void onMounted();

protected:
    void closeEvent( QCloseEvent* aEvent ) override ;

signals:
    void fileSavedChanged( );
};

#endif // FRM_MAIN_H