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

#ifndef PARTNERWIDGET_H
#define PARTNERWIDGET_H

#include <QWidget>
#include <QSettings>

#include "datamodule.h"

namespace Ui {
class PartnerWidget;
}

/*
 * Справочник контрагентов.
 *
 * Форма справочника создана в Qt Designer.
 *
 * В этом справочнике вставка, правка и удаление записей
 * производится в таблице.
 * Для фиксации данных применяются транзакции.
 */
class PartnerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PartnerWidget(QSettings *p_settings,
                           DataModule *p_dm,
                           QWidget *parent = nullptr);
    virtual ~PartnerWidget();

private slots:
    void insertRow();
    void deleteRow();
    void revert();
    void submit();

    void rowDataChanged(const QModelIndex &topLeft,
                        const QModelIndex &bottomRight);

private:

    const int       WIDGET_WIDTH {700};
    const int       WIDGET_HEIGHT {500};

    void            setWidgets();

    Ui::PartnerWidget *ui {nullptr};

    QSettings       *settings {nullptr};
    DataModule      *dm {nullptr};
    QSqlTableModel  *model {nullptr};

};

#endif // PARTNERWIDGET_H
