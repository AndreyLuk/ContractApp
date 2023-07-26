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

#include "partnerwidget.h"
#include "ui_partnerwidget.h"

// конструктор справочника контрагентов
PartnerWidget::PartnerWidget(QSettings *p_settings,
                             DataModule *p_dm,
                             QWidget *p_parent)
    : QWidget(p_parent),
      ui(new Ui::PartnerWidget),
      settings(p_settings),
      dm(p_dm)
{
    setFixedSize(WIDGET_WIDTH, WIDGET_HEIGHT);

    // создание модели данных справочника
    dm->createPartnerModel();
    model = dm->getPartnerModel();

    // создание элементов интерфейса
    ui->setupUi(this);

    // настройка таблицы
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(model->fieldIndex("partner_id"), true);
    ui->tableView->setColumnWidth(1, WIDGET_WIDTH * 0.65);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    // соединение кнопок и слотов
    connect(ui->insertButton, &QPushButton::clicked,
            this, &PartnerWidget::insertRow);
    connect(ui->deleteButton, &QPushButton::clicked,
            this, &PartnerWidget::deleteRow);
    connect(ui->revertButton, &QPushButton::clicked,
            this, &PartnerWidget::revert);
    connect(ui->submitButton, &QPushButton::clicked,
            this, &PartnerWidget::submit);

    // подключить слот обработки изменения данных строки
    connect(model, &QSqlTableModel::dataChanged,
            this, &PartnerWidget::rowDataChanged);

    setWidgets();

} // PartnerWidget::PartnerWidget()

// деструктор
PartnerWidget::~PartnerWidget()
{
    delete ui;

    // удалить  модель данных справочника
    dm->removePartnerModel();

} // ~PartnerWidget()

// слот вставки строки
void PartnerWidget::insertRow()
{
    int crow = ui->tableView->currentIndex().row();
    model->insertRow(crow);
    ui->tableView->setCurrentIndex(model->index(crow, 1));
    QItemSelection selection(model->index(crow, 1), model->index(crow, 1));
    ui->tableView->selectionModel()->select(selection,
                                        QItemSelectionModel::Select);
    ui->tableView->edit(model->index(crow, 1));

    setWidgets();
} // insertRow()

// слот удаления строки
void PartnerWidget::deleteRow()
{
    model->removeRow(ui->tableView->currentIndex().row());
    setWidgets();
} // deleteRow()

// слот отката всех изменений в справочнике
void PartnerWidget::revert()
{
    model->revertAll();

    // обновить таблицу
    ui->tableView->hide();
    ui->tableView->show();

    setWidgets();
} // revert()

// слот утверждения всех изменений в справочнике
void PartnerWidget::submit()
{
    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();
    } else {
        model->database().rollback();
        QMessageBox::critical(this, tr("Contract Application"),
                             tr("The database reported an error: %1")
                             .arg(model->lastError().text()));
    }
    setWidgets();
} // submit()

// слот обработки изменения данных
void PartnerWidget::rowDataChanged(const QModelIndex &topLeft,
                                         const QModelIndex &bottomRight)
{
    // hide warnings
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)

    // измененить состояние кнопок "отменить" , "утвердить"
    setWidgets();

} // rowDataChanged()

// установить виджеты в зависимости от...
void PartnerWidget::setWidgets()
{
    // в таблице не выбрана строка - выбрать первую, если есть
    if ((ui->tableView->currentIndex().row() < 0) &&
        (model->rowCount() > 0))
    {
        ui->tableView->setCurrentIndex(model->index(0, 1));
        QItemSelection selection(model->index(0, 1), model->index(0, 1));
        ui->tableView->selectionModel()->select(selection,
                                            QItemSelectionModel::Select);
    }

    // установить состояние кнопок
    ui->deleteButton->setEnabled(model->rowCount() > 0);
    ui->revertButton->setEnabled(model->isDirty());
    ui->submitButton->setEnabled(model->isDirty());

} // setWidgets()
