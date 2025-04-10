/// @file tree_model_delegate.h
/// Implementation for the TreeModelDelegate class.
///
/// @version 0.1
/// @author Anirudha Jadhav <<anirudhasj441@gmail.com>
///
/// (C)2025 Coding Devil.
/// https://www.codingdevil.in/

#ifndef TREE_MODEL_DELEGATE_H
#define TREE_MODEL_DELEGATE_H

#include <QStyledItemDelegate>

class TreeModelDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    // constructor
    TreeModelDelegate( QObject* aParent = nullptr );

    ~TreeModelDelegate();

public:
    QWidget* createEditor( 
        QWidget* aParent, 
        const QStyleOptionViewItem& aOption,
        const QModelIndex &aIndex
    ) const override;
    
    void setEditorData( QWidget *aEditor, 
            const QModelIndex &aIndex ) const override;

    void setModelData(QWidget *aEditor, QAbstractItemModel *aModel, 
            const QModelIndex &aIndex) const override;
    
    void paint(
            QPainter* aPainter, 
            const QStyleOptionViewItem& aOption, 
            const QModelIndex& aIndex
    ) const;
};


#endif // TREE_MODEL_DELEGATE_H
