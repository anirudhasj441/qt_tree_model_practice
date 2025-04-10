/// @file tree_model_delegate.cpp
/// Implementation for the TreeModelDelegate class.
///
/// @version 0.1
/// @author Anirudha Jadhav <<anirudhasj441@gmail.com>
///
/// (C)2025 Coding Devil.
/// https://www.codingdevil.in/

#include "tree_model_delegate.h"
#include "tree_model.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QApplication>

TreeModelDelegate::
TreeModelDelegate( QObject* aParent ) : QStyledItemDelegate( aParent ) {
    // Nothing to do here
}


TreeModelDelegate::
~TreeModelDelegate() {

}

QWidget* TreeModelDelegate::
createEditor( QWidget* aParent, const QStyleOptionViewItem &aOption,
        const QModelIndex &aIndex ) const 
{
    TreeNode* node = static_cast<TreeNode*>( aIndex.internalPointer() );

    if( node == nullptr ) { return nullptr; }

    if ( 1 != aIndex.column() ) {
        return nullptr;
    }

    if( QMetaType::QString == node->value.typeId()) {
        QLineEdit* editor = new QLineEdit( aParent );
        editor->setText( node->value.toString());

        return editor;
    }
    if( QMetaType::Bool == node->value.typeId()) {
        QCheckBox* checkBox = new QCheckBox( aParent );
        checkBox->setChecked( node->value.toBool());

        return checkBox;
    }

    if( QMetaType::Int == node->value.typeId()) {
        QSpinBox* spinBox = new QSpinBox( aParent );
        spinBox->setValue( node->value.toInt());

        return spinBox;
    }

    if( QMetaType::Double == node->value.typeId()) {
        QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox( aParent );
        doubleSpinBox->setValue( node->value.toDouble());

        return doubleSpinBox;
    }  

    return nullptr;
}

void TreeModelDelegate::
setEditorData( QWidget *aEditor, const QModelIndex &aIndex ) const {
    TreeNode* node = static_cast<TreeNode*>( aIndex.internalPointer());

    if( node == nullptr ) { return; }

    if( QMetaType::QString == node->value.typeId()) {
        QString value = node->value.toString();

        QLineEdit* editor = qobject_cast<QLineEdit*>( aEditor );

        editor->setText( value );
        return;
    }

    if ( QMetaType::Bool == node->value.typeId()) {
        bool value = node->value.toBool();

        QCheckBox* editor = qobject_cast<QCheckBox*>( aEditor );

        editor->setChecked( value );
        return;
    }

    if ( QMetaType::Int == node->value.typeId()) {
        int value = node->value.toInt();

        QSpinBox* editor = qobject_cast<QSpinBox*>( aEditor );

        editor->setValue( value );
        return;
    }

    if ( QMetaType::Double == node->value.typeId()) {
        double value = node->value.toDouble();

        QDoubleSpinBox* editor = qobject_cast<QDoubleSpinBox*>( aEditor );

        editor->setValue( value );
        return;
    }
}

void TreeModelDelegate::
setModelData(QWidget *aEditor, QAbstractItemModel *aModel, 
        const QModelIndex &aIndex) const 
{
    TreeNode* node = static_cast<TreeNode*>( aIndex.internalPointer());

    if( node == nullptr ) { return; }

    if( QMetaType::QString == node->value.typeId()) {
        QLineEdit* editor = qobject_cast<QLineEdit*>( aEditor );

        QString value = editor->text();

        aModel->setData( aIndex, value, Qt::EditRole );
        return;
    }

    if ( QMetaType::Bool == node->value.typeId()) {
        QCheckBox* editor = qobject_cast<QCheckBox*>( aEditor );

        bool value = editor->isChecked();

        aModel->setData( aIndex, value, Qt::EditRole );
        return;
    }

    if ( QMetaType::Int == node->value.typeId()) {
        QSpinBox* editor = qobject_cast<QSpinBox*>( aEditor );

        int value = editor->value();

        aModel->setData( aIndex, value, Qt::EditRole );
        return;
    }

    if ( QMetaType::Double == node->value.typeId()) {
        QDoubleSpinBox* editor = qobject_cast<QDoubleSpinBox*>( aEditor );

        double value = editor->value();

        aModel->setData( aIndex, value, Qt::EditRole );
        return;
    }

   
}

void TreeModelDelegate::
paint( QPainter* aPainter, const QStyleOptionViewItem& aOption, 
        const QModelIndex& aIndex ) const 
{
    TreeNode* node = static_cast<TreeNode*>( aIndex.internalPointer());

    if( node == nullptr ) { return; }

    
    if( QMetaType::Bool == node->value.typeId() && aIndex.column() == 1 ) {

        bool checked = node->value.toBool();

        QStyleOptionButton checkBoxOption;
        checkBoxOption.state |= QStyle::State_Enabled;
        if (checked)
            checkBoxOption.state |= QStyle::State_On;
        else
            checkBoxOption.state |= QStyle::State_Off;

        checkBoxOption.rect = QApplication::style()->subElementRect( 
                QStyle::SE_CheckBoxIndicator, &aOption );
        QApplication::style()->drawControl(QStyle::CE_CheckBox, 
                &checkBoxOption, aPainter );

        return;
    }

    QStyledItemDelegate::paint( aPainter, aOption, aIndex );
}