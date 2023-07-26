/*
 * This file is a part of a project "contract" -
 * a Qt Widgets client/server application.
 *
 * It's a working demo, simplified, not a production level.
 *
 * Author: Andrey V. Lukashenko.
 *
 * Anyone can use parts of this program without any warranty
 * and without a reference to author.
 */

#include "contractdelegate.h"

#include <QApplication>
#include <QLineEdit>

ContractDelegate::ContractDelegate(QObject *parent)
    : QSqlRelationalDelegate(parent)
{
}

void ContractDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    QStyleOptionHeader numberValue;
    numberValue.rect = option.rect;
    numberValue.text = index.data().toString() + " ";

    QStyleOptionHeader dateValue;
    dateValue.rect = option.rect;
    dateValue.text = index.data().toString() + " ";

    QStyleOptionHeader timeValue;
    timeValue.rect = option.rect;
    timeValue.text = index.data().toString() + " ";

    switch (index.column()) {
    case 0:
    case 8:
    case 9:
    case 10:
    case 11:
        // set right alignment for numbers
        numberValue.textAlignment = Qt::AlignRight | Qt::AlignVCenter;
        QApplication::style()->drawControl(QStyle::CE_HeaderLabel, &numberValue,
                                           painter);
        break;
    case 5:
    case 6:
    case 7:
        // set center alignment for dates
        dateValue.textAlignment = Qt::AlignCenter;
        QApplication::style()->drawControl(QStyle::CE_HeaderLabel, &dateValue,
                                           painter);
        break;
    case 13:
        // set center alignment for timestamp
        timeValue.textAlignment = Qt::AlignCenter;
        QApplication::style()->drawControl(QStyle::CE_HeaderLabel, &timeValue,
                                           painter);
        break;
    default:
        QSqlRelationalDelegate::paint(painter, option, index);
        break;
    }
}

QWidget *ContractDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    int columnNumber = index.column();

    if (columnNumber != 0) {
        return QSqlRelationalDelegate::createEditor(parent, option, index);
    } else {

        // for id column
        QLineEdit *idEdit = new QLineEdit(parent);
        idEdit->setAlignment(Qt::AlignRight);
        idEdit->setFrame(false);
        return idEdit;
    }
}
