/// @file tree_model.cpp
/// Implementation for the TreeModel class.
///
/// @version 0.1
/// @author Anirudha Jadhav <<anirudhasj441@gmail.com>
///
/// (C)2025 Coding Devil.
/// https://www.codingdevil.in/

#include "tree_model.h"
#include <QList>
#include <QIcon>

TreeModel::
TreeModel( QObject *aParent ) : QAbstractItemModel( aParent ) {
    // Nothing to do here
    this->mRootNode = new TreeNode();
}


TreeModel::
~TreeModel() {
    // Nothing to do here

    delete mRootNode;
}


QModelIndex TreeModel::
index( int aRow, int aColumn, const QModelIndex &aParent ) const {
    TreeNode* node;

    node = aParent.isValid() ? 
            static_cast<TreeNode*>( aParent.internalPointer() ) : mRootNode;

    if( node == nullptr ) { return QModelIndex(); }

    if( node->children.size() <= aRow ) { return QModelIndex(); }

    return createIndex( aRow, aColumn, node->children.at( aRow ));
}

QModelIndex TreeModel::
parent( const QModelIndex& aIndex ) const {
    if( !aIndex.isValid() ) { return QModelIndex(); }

    TreeNode* node = static_cast<TreeNode*>( aIndex.internalPointer() );
    TreeNode* parentNode = node->parent;

    if( nullptr == parentNode || mRootNode == parentNode ) { 
        return QModelIndex(); 
    }

    int row = parentNode->parent->children.indexOf( parentNode );

    return createIndex( row, 0, parentNode );
}

int TreeModel::
rowCount( const QModelIndex& aParent ) const {
    if( !aParent.isValid() ) { return this->mRootNode->children.size(); }
    
    TreeNode* node = static_cast<TreeNode*>( aParent.internalPointer() );
    
    if( node == nullptr ) { return 0; }
    
    return node->children.size();
}


int TreeModel::
columnCount( const QModelIndex& aParent ) const {
    return 2;
}

QVariant TreeModel::
data( const QModelIndex& aIndex, int aRole ) const {
    if( !aIndex.isValid() ) { return QVariant(); }

    TreeNode* node = static_cast<TreeNode*>( aIndex.internalPointer() );

    if( node == nullptr ) { return QVariant(); }

    if( aRole == Qt::DisplayRole ) {
        if( aIndex.column() == 0 ) {
            return node->key;
        } else if( aIndex.column() == 1 ) {
            return node->value;
        }
    }

    if( Qt::DecorationRole == aRole ) {
        if( aIndex.column() == 0 ) {
            return node->expanded ? QIcon( ":/icons/folder-open.svg" ) :
                    QIcon( ":/icons/folder-close.svg" );
        }
    }

    return QVariant();
}

QVariant TreeModel::
headerData(int section, Qt::Orientation orientation, 
        int role ) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return QVariant();
    }

    switch ( section ) {
        case 0:
            return QString("Property");
        case 1:
            return QString("Value");
        default:
            return QVariant();
    }
}

QList<TreeNode*> TreeModel::
createChildNodes( const jsoncons::ojson& aJsonData, TreeNode* aParent ) {
    QList<TreeNode*> nodes;
    if( aJsonData.is_object() ) {
        for( const auto& item : aJsonData.object_range() ) {
            TreeNode* node = new TreeNode();
            
            node->key = QString::fromStdString( item.key() );
            node->value = QString::fromStdString( item.value().as_string());
            node->parent = aParent;
            node->children = QList<TreeNode*>();

            if(  item.value().is_object() || item.value().is_array() ) {
                node->value = QString("");
                QList<TreeNode*> children = createChildNodes( item.value(), node );
                node->children = children;
            }
            
            nodes.append( node );
            
        }
    } else if( aJsonData.is_array() ) {
        int count = 0;
        for( const auto& item : aJsonData.array_range() ) {
            TreeNode* node = new TreeNode();

            node->key = QString::number( count );
            node->value = QString::fromStdString( item.as_string());
            node->parent = aParent;

            if( item.is_object() ) {
                QList<TreeNode*> children = createChildNodes( item, node );
                node->children = children;
            }

            nodes.append( node );
            count++;
        }
    }
    return nodes;
}

void TreeModel::
buildTree( const jsoncons::ojson& aJsonData ) {
    beginResetModel();
    this->mRootNode->children = createChildNodes( aJsonData, this->mRootNode );
    endResetModel();
} 