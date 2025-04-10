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

namespace Ui {
    class MainForm;
}

class TreeModel;

class MainForm : public QMainWindow {
    Q_OBJECT

public:
    /// Constructor
    MainForm( QMainWindow* aParent = nullptr );
    virtual ~MainForm();

private:
    Ui::MainForm* ui{ nullptr };

    TreeModel* mTreeModel{ nullptr };

private slots:
    void actionLoadJson_Triggered();

    void treeView_Expanded( const QModelIndex& aIndex );

    void treeView_Collapsed( const QModelIndex& aIndex );

    void treeModel_DataChanged( const QModelIndex &topLeft, 
            const QModelIndex &bottomRight, 
            const QList<int> &roles = QList<int>()
    );


private:
    /// Function call after ui is mounted
    void onMounted();
};

#endif // FRM_MAIN_H