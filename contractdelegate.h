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
#ifndef CONTRACTDELEGATE_H
#define CONTRACTDELEGATE_H

#include <QSqlRelationalDelegate>
#include <QModelIndex>

/*
 * A class for tuning a contract table.
 */
class ContractDelegate : public QSqlRelationalDelegate
{
public:
    ContractDelegate(QObject *parent);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const Q_DECL_OVERRIDE;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // CONTRACTDELEGATE_H
