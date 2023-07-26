#ifndef CONTRACTSTATEWIDGET_H
#define CONTRACTSTATEWIDGET_H

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

#include <QWidget>
#include <QSettings>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableView>

#include "datamodule.h"

/*
 * Справочник состояний договоров.
 *
 * В этом справочнике вставка, правка и удаление записей
 * производится в таблице.
 * Для фиксации данных применяются транзакции.
 */
class ContractStateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ContractStateWidget(QSettings *p_settings,
                                 DataModule *p_dm,
                                 QWidget *parent = nullptr);
    virtual ~ContractStateWidget();

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

    QVBoxLayout     *referenceLayout {nullptr};
    QHBoxLayout     *buttonLayout {nullptr};
    QPushButton     *insertButton {nullptr};
    QPushButton     *deleteButton {nullptr};
    QPushButton     *revertButton {nullptr};
    QPushButton     *submitButton {nullptr};
    QTableView      *tableView {nullptr};

    QSettings       *settings {nullptr};
    DataModule      *dm {nullptr};
    QSqlTableModel  *model {nullptr};
};

#endif // CONTRACTSTATEWIDGET_H
