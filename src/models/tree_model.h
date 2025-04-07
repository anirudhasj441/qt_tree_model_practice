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

struct TreeNode{
    QString key;
    QString value;
    QList<TreeNode*> children;
    TreeNode* parent = nullptr;
} ;

class TreeModel: public QAbstractItemModel {
    Q_OBJECT

public:
    // constructor
    TreeModel( QObject* aParent = nullptr );

    // destructor
    ~TreeModel();

public:
    QModelIndex index( int row, int column, 
            const QModelIndex &parent = QModelIndex() ) const override;

    QModelIndex parent( const QModelIndex &index ) const override;

    int rowCount( const QModelIndex &parent = QModelIndex()) const override;

    int columnCount( const QModelIndex &parent = QModelIndex()) const override;

    QVariant data( const QModelIndex &index, 
            int role = Qt::DisplayRole ) const override;

    bool setData( const QModelIndex &index, const QVariant &value, 
            int role = Qt::EditRole ) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

private: 
    TreeNode* mRootNode{ nullptr };

};


#endif