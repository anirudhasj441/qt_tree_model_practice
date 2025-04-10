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
#include <QCheckBox>
#include <jsoncons_ext/jsonpointer/jsonpointer.hpp>

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
        if( aIndex.column() != 0 ) {
            return QVariant();
        }

        if( !node->children.isEmpty() ) {
            return node->expanded ? QIcon( ":/icons/folder-open.svg" ) :
                    QIcon( ":/icons/folder-close.svg" );
        }

        return node->icon;
    }

    return QVariant();
}

bool TreeModel::
setData( const QModelIndex& aIndex, const QVariant& aValue, int aRole ) {
    if( !aIndex.isValid() ) { return false; }

    if( Qt::EditRole != aRole ) { return false; }

    TreeNode* node = static_cast<TreeNode*>( aIndex.internalPointer());

    if( node == nullptr ) { return false; }

    if( aIndex.column() == 0 ) {
        node->key = aValue.toString();
    } else if( aIndex.column() == 1 ) {
        node->value = aValue;
    }

    this->updateJsonValue( node->jsonPath, node->value );

    emit dataChanged( aIndex, aIndex );

    return true;
}

Qt::ItemFlags TreeModel::
flags ( const QModelIndex& aIndex ) const {
    if( !aIndex.isValid() ) { return Qt::NoItemFlags; }

    TreeNode* node = static_cast<TreeNode*>( aIndex.internalPointer());

    if( node == nullptr ) { return Qt::NoItemFlags; }

    if( !node->children.isEmpty()) {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    if( aIndex.column() == 0 ) {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    if( aIndex.column() == 1 ) {
        return Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable;
    }

    return Qt::NoItemFlags;
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
createChildNodes( const jsoncons::ojson& aJsonData, TreeNode* aParent, QString aJsonPath ) {
    QList<TreeNode*> nodes;
    if ( aJsonData.is_object() ) {
        for ( const auto& item : aJsonData.object_range() ) {
            TreeNode* node = new TreeNode();

            QString jsonsPath = aJsonPath + "/" + QString::fromStdString( 
                    item.key());

            node->key = QString::fromStdString( item.key() );
            node->value =  QString::fromStdString(item.value().as_string());
            node->icon = QIcon( ":/icons/string_value.svg" );
            node->jsonPath = jsonsPath;

            if( item.value().is_bool()) {
                node->value =  item.value().as<bool>();
                node->icon = QIcon( ":/icons/boolean_value.svg" );
            }
            if( item.value().is_int64()) {
                node->value =  item.value().as<int>();
                node->icon = QIcon( ":/icons/number_value.svg" );
            }
            if( item.value().is_double()) {
                node->value =  item.value().as_double();
                node->icon = QIcon( ":/icons/number_value.svg" );
            }
            node->parent = aParent;
            node->children = QList<TreeNode*>();

            if(  item.value().is_object() || item.value().is_array() ) {
                node->value = QString("");
                QList<TreeNode*> children = createChildNodes( item.value(), 
                        node, jsonsPath );
                node->children = children;
            }
            
            nodes.append( node );
            
        }
    } else if( aJsonData.is_array() ) {
        for ( std::size_t i = 0; i < aJsonData.size(); ++i ) {
            TreeNode* node = new TreeNode();

            jsoncons::ojson item = aJsonData[ i ];

            QString jsonsPath = aJsonPath + "/" + QString::number( i );

            node->key = QString::number( i );
            node->value = QString::fromStdString( item.as_string());
            node->parent = aParent;
            node->jsonPath = jsonsPath;

            if( item.is_object() ) {
                QList<TreeNode*> children = createChildNodes( item, node, 
                        jsonsPath );
                node->children = children;
            }

            nodes.append( node );
        }
    }
    return nodes;
}

bool TreeModel::
updateJsonValue( const QString& aJsonPath, const QVariant& aValue ) {
    jsoncons::ojson value;

    switch (aValue.typeId()) {
        case QMetaType::QString:
            value = aValue.toString().toStdString(); // this is JSON string
            break;
        case QMetaType::Bool:
            value = aValue.toBool(); // directly use native bool
            break;
        case QMetaType::Int:
            value = aValue.toInt(); // directly use int
            break;
        case QMetaType::Double:
            value = aValue.toDouble(); // directly use double
            break;
        default:
            // fallback: store as string
            value = aValue.toString().toStdString();
            break;
    }

    // check if the json path is valid
    if( !jsoncons::jsonpointer::contains( 
            this->mJson, aJsonPath.toStdString())) 
    {
        qDebug() << "Invalid JSON path: " << aJsonPath;
        return false;
    }

    jsoncons::jsonpointer::add( this->mJson, aJsonPath.toStdString(), 
            value );

    return true;
}

TreeNode* TreeModel::
rootNode( ) {
    return mRootNode;
}

jsoncons::ojson TreeModel::
jsonData( ) {
    return this->mJson;
}

void TreeModel::
buildTree( const jsoncons::ojson& aJsonData ) {
    this->mJson = aJsonData;    
    beginResetModel();
    this->mRootNode->children = createChildNodes( this->mJson, this->mRootNode );
    endResetModel();
} 