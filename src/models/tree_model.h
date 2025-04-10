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
#include <QIcon>

// class QList;
struct TreeNode {
    QString key;
    QVariant value;
    QList<TreeNode*> children;
    bool expanded{ false };
    QIcon icon;
    QString jsonPath;
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
            const QModelIndex& aParent = QModelIndex() ) const override;

    QModelIndex parent( const QModelIndex& aIndex ) const override;

    int rowCount( const QModelIndex& aParent = QModelIndex()) const override;

    int columnCount( const QModelIndex& aParent = QModelIndex()) const override;

    QVariant data( const QModelIndex& aIndex, 
            int aRole = Qt::DisplayRole ) const override;

    bool setData( const QModelIndex& aIndex, 
            const QVariant& aValue, int aRole = Qt::EditRole ) override;

    Qt::ItemFlags flags(const QModelIndex& aIndex) const override;

    QVariant headerData(int section, Qt::Orientation orientation, 
            int role = Qt::DisplayRole) const override;

    void buildTree( const jsoncons::ojson& aJsonData );

    TreeNode* rootNode( ) {
        return  mRootNode;
    }

private: 
    TreeNode* mRootNode{ nullptr };
    jsoncons::ojson mJson;

private:
    QList<TreeNode*> createChildNodes( const jsoncons::ojson& aJsonData, 
            TreeNode* aParent = nullptr, QString aJsonPath = "" );

    bool updateJsonValue( const QString& aJsonPath, const QVariant& aValue );

};



#endif