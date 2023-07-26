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

#ifndef CONTRACTTYPEWIDGET_H
#define CONTRACTTYPEWIDGET_H

#include <QWidget>
#include <QSettings>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableView>

#include "datamodule.h"

/*
 * Справочник типов договоров.
 *
 * В этом справочнике вставка, правка и удаление записей в таблице
 * базы данных производится с помощью диалоговых окон без транзакций.
 */
class ContractTypeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ContractTypeWidget(QSettings *p_settings,
                                DataModule *p_dm,
                                QWidget *p_parent = nullptr);
    virtual ~ContractTypeWidget();

private slots:
    void insertRow();
    void editRow();
    void deleteRow();

private:
    const int       WIDGET_WIDTH {700};
    const int       WIDGET_HEIGHT {500};
    const int       DIALOG_WIDTH {500};
    const int       DIALOG_HEIGHT {150};
    const int       ITEM_WIDTH {225};

    void            setWidgets();
    void            setTableView();

    QVBoxLayout     *referenceLayout {nullptr};
    QHBoxLayout     *buttonLayout {nullptr};
    QPushButton     *insertButton {nullptr};
    QPushButton     *editButton {nullptr};
    QPushButton     *deleteButton {nullptr};
    QTableView      *tableView {nullptr};

    QSettings       *settings {nullptr};
    DataModule      *dm {nullptr};
    QSqlTableModel  *model {nullptr};
};

#endif // CONTRACTTYPEWIDGET_H
