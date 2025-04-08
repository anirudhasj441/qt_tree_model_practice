/// @file tree_model.h
/// Declaration for the TreeModel class.
///
/// @version 0.1
/// @author Anirudha Jadhav <<anirudhasj441@gmail.com>
///
/// (C)2025 Coding Devil.
/// https://www.codingdevil.in/

#ifndef TREE_MODEL_H
#define TREE_MODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <jsoncons/json.hpp>

struct TreeNode{
    QString key;
    QString value;
    QList<TreeNode*> children;
    TreeNode* parent = nullptr;
};

class TreeModel: public QAbstractItemModel {
    Q_OBJECT

public:
    // constructor
    TreeModel( QObject* aParent = nullptr );

    // destructor
    ~TreeModel();

public:
    QModelIndex index( int aRow, int aColumn, 
            const QModelIndex &aParent = QModelIndex() ) const override;

    QModelIndex parent( const QModelIndex& aIndex ) const override;

    int rowCount( const QModelIndex& aParent = QModelIndex()) const override;

    int columnCount( const QModelIndex& aParent = QModelIndex()) const override;

    QVariant data( const QModelIndex& aIndex, 
            int aRole = Qt::DisplayRole ) const override;

    void buildTree( const jsoncons::ojson& aJsonData );

    TreeNode* rootNode( ) {
        return  mRootNode;
    }

private: 
    TreeNode* mRootNode{ nullptr };

private:
    TreeNode* createNode( const jsoncons::ojson& aJsonData, TreeNode* aParent = nullptr );


};



#endif