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

#include <QMessageBox>
#include <QSqlError>
#include <QHeaderView>

#include "contractstatewidget.h"

// конструктор справочника состояний договоров
ContractStateWidget::ContractStateWidget(QSettings *p_settings,
                                         DataModule *p_dm,
                                         QWidget *p_parent)
      : QWidget(p_parent),
        settings(p_settings),
        dm(p_dm)
{
    setFixedSize(WIDGET_WIDTH, WIDGET_HEIGHT);

    // создание модели данных справочника
    dm->createContractStateModel();
    model = dm->getContractStateModel();

    // создание элементов интерфейса

    // раскладка справочника - вертикальная
    referenceLayout = new QVBoxLayout;
    // раскладка кнопок - горизонтальная
    buttonLayout = new QHBoxLayout;

    // создание таблицы
    tableView = new QTableView;
    tableView->setModel(model);
    tableView->setAlternatingRowColors(true);
    tableView->setColumnHidden(model->fieldIndex("state_id"), true);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->horizontalHeader()->setStretchLastSection(true);
    referenceLayout->addWidget(tableView);

    // создание кнопок
    insertButton = new QPushButton(tr("Insert"));
    deleteButton = new QPushButton(tr("Delete"));
    revertButton = new QPushButton(tr("Revert"));
    submitButton = new QPushButton(tr("Submit"));
    buttonLayout->addWidget(insertButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(revertButton);
    buttonLayout->addWidget(submitButton);
    referenceLayout->addLayout(buttonLayout);

    // установить раскладку справочника
    setLayout(referenceLayout);

    // соединение кнопок и слотов
    connect(insertButton, &QPushButton::clicked,
            this, &ContractStateWidget::insertRow);
    connect(deleteButton, &QPushButton::clicked,
            this, &ContractStateWidget::deleteRow);
    connect(revertButton, &QPushButton::clicked,
            this, &ContractStateWidget::revert);
    connect(submitButton, &QPushButton::clicked,
            this, &ContractStateWidget::submit);

    // подключить слот обработки изменения данных строки
    connect(model, &QSqlTableModel::dataChanged,
            this, &ContractStateWidget::rowDataChanged);

    setWidgets();

} // ContractStateWidget::ContractStateWidget()

// деструктор
ContractStateWidget::~ContractStateWidget()
{
    // удалить  модель данных справочника
    dm->removeContractStateModel();
} // ~ContractStateWidget()

// слот вставки строки
void ContractStateWidget::insertRow()
{
    int crow = tableView->currentIndex().row();
    model->insertRow(crow);
    tableView->setCurrentIndex(model->index(crow, 1));
    QItemSelection selection(model->index(crow, 1), model->index(crow, 1));
    tableView->selectionModel()->select(selection,
                                        QItemSelectionModel::Select);
    tableView->edit(model->index(crow, 1));

    setWidgets();
} // insertRow()

// слот удаления строки
void ContractStateWidget::deleteRow()
{
    model->removeRow(tableView->currentIndex().row());
    setWidgets();
} // deleteRow()

// слот отката всех изменений в справочнике
void ContractStateWidget::revert()
{
    model->revertAll();

    // обновить таблицу
    tableView->hide();
    tableView->show();

    setWidgets();
} // revert()

// слот утверждения всех изменений в справочнике
void ContractStateWidget::submit()
{
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
    } else {
        model->database().rollback();
        QMessageBox::warning(this, tr("Contract Application"),
                             tr("The database reported an error: %1")
                             .arg(model->lastError().text()));
    }
    setWidgets();
} // submit()

// слот обработки изменения данных
void ContractStateWidget::rowDataChanged(const QModelIndex &topLeft,
                                         const QModelIndex &bottomRight)
{
    // hide warnings
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)

    // измененить состояние кнопок "отменить" , "утвердить"
    setWidgets();

} // rowDataChanged()

// установить виджеты в зависимости от...
void ContractStateWidget::setWidgets()
{
    // в таблице не выбрана строка - выбрать первую, если есть
    if ((tableView->currentIndex().row() < 0) &&
        (model->rowCount() > 0))
    {
        tableView->setCurrentIndex(model->index(0, 1));
        QItemSelection selection(model->index(0, 1), model->index(0, 1));
        tableView->selectionModel()->select(selection,
                                            QItemSelectionModel::Select);
    }
    // установить состояние кнопок
    deleteButton->setEnabled(model->rowCount() > 0);
    revertButton->setEnabled(model->isDirty());
    submitButton->setEnabled(model->isDirty());

} // setWidgets()
