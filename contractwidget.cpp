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
#include <QSqlRecord>
#include <QSqlField>

#include "contractwidget.h"
#include "contractdelegate.h"

// конструктор класса обработки договоров
ContractWidget::ContractWidget(QSettings *p_settings,
                               DataModule *p_dm,
                               QWidget *p_parent)
    : QWidget(p_parent),
      settings(p_settings),
      dm(p_dm)
{
    setFixedSize(p_parent->width() * 0.96, p_parent->height() * 0.92);

    // создать модель данных
    dm->createContractModel();
    model = dm->getContractModel();

    // создать графические элементы интерфейса
    createTableView();
    createEditBox();
    createControlBox();

    // расположить элементы интерфейса
    widgetLayout = new QVBoxLayout;
    widgetLayout->addWidget(tableBox);
    widgetLayout->addWidget(editBox);
    widgetLayout->addWidget(controlBox);
    setLayout(widgetLayout);

    // соединение кнопок и слотов
    connect(insertButton, &QPushButton::clicked,
            this, &ContractWidget::insertRow);
    connect(editButton, &QPushButton::clicked,
            this, &ContractWidget::editRow);
    connect(deleteButton, &QPushButton::clicked,
            this, &ContractWidget::deleteRow);
    connect(revertButton, &QPushButton::clicked,
            this, &ContractWidget::revert);
    connect(submitButton, &QPushButton::clicked,
            this, &ContractWidget::submit);

    // подключить слот обработки изменения данных
    connect(model, &QSqlRelationalTableModel::dataChanged,
            this, &ContractWidget::dataChanged);

    // синхронизировать таблицу и форму
    connect(tableView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
            mapper, SLOT(setCurrentModelIndex(QModelIndex)));

    setWidgets();

} // ContractWidget::ContractWidget()

// деструктор
ContractWidget::~ContractWidget()
{
    // удалить  модель данных
    dm->removeContractModel();

} // ~ContractWidget()

// создание таблицы
void ContractWidget::createTableView()
{
    tableBox = new QGroupBox;
    tableLayout = new QVBoxLayout;
    tableBox->setFixedWidth(width() - 30);
    tableBox->setFixedHeight(height() - CONTROL_BOX_HEIGHT - 30);
    tableView = new QTableView(tableBox);
    tableView->setModel(model);
    tableView->setItemDelegate(new ContractDelegate(tableView));
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setAlternatingRowColors(true);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSortingEnabled(true);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setCurrentIndex(model->index(0, 0));
    tableView->setColumnWidth(model->fieldIndex("contract_id"), 80);
    tableView->setColumnWidth(model->fieldIndex("typer_name"), 120);
    tableView->setColumnWidth(model->fieldIndex("state_name"), 140);
    tableView->setColumnWidth(model->fieldIndex("partner_name"), 250);
    tableView->setColumnWidth(model->fieldIndex("vat_rate"), 100);
    tableView->setColumnWidth(model->fieldIndex("contract_sum"), 180);
    tableView->setColumnWidth(model->fieldIndex("vat_sum"), 180);
    tableView->setColumnWidth(model->fieldIndex("total_sum"), 180);
    tableView->setColumnWidth(model->fieldIndex("updated_date"), 230);
    tableLayout->addWidget(tableView);
    tableBox->setLayout(tableLayout);
} // createTableView()

// создание формы вставки/правки
void ContractWidget::createEditBox()
{
    editBox = new QGroupBox;
    editBox->setFixedWidth(width() - 30);
    editBox->setFixedHeight(height() - CONTROL_BOX_HEIGHT - 30);
    editLayout = new QFormLayout;

    contractIdLabel = new QLabel(tr("Contract ID"));
    contractIdEdit = new QLineEdit;             //contract_id
    contractIdEdit->setFixedWidth(LINE_EDIT_WIDTH);
    contractIdEdit->setReadOnly(true);
    contractIdEdit->setStyleSheet(GRAY_STYLE);
    contractIdEdit->setAlignment(Qt::AlignRight);
    editLayout->addRow(contractIdLabel, contractIdEdit);

    typeLabel = new QLabel(tr("Type"));
    typeComboBox = new QComboBox;
    typeComboBox->setFixedWidth(COMBO_WIDTH);
    editLayout->addRow(typeLabel, typeComboBox);

    stateLabel = new QLabel(tr("State"));
    stateComboBox = new QComboBox;
    stateComboBox->setFixedWidth(COMBO_WIDTH);
    editLayout->addRow(stateLabel, stateComboBox);

    partnerLabel = new QLabel(tr("Partner"));
    partnerComboBox = new QComboBox;
    partnerComboBox->setFixedWidth(COMBO_WIDTH);
    editLayout->addRow(partnerLabel, partnerComboBox);

    contractNumLabel = new QLabel(tr("Contract Number"));
    contractNumEdit = new QLineEdit;
    contractNumEdit->setFixedWidth(LINE_EDIT_WIDTH);
    editLayout->addRow(contractNumLabel, contractNumEdit);

    contractDateLabel = new QLabel(tr("Contract Date"));
    contractDateEdit = new QDateEdit;
    contractDateEdit->setFixedWidth(EDIT_DATE_WIDTH);
    editLayout->addRow(contractDateLabel, contractDateEdit);

    startLabel = new QLabel(tr("Start"));
    startEdit = new QDateEdit;
    startEdit->setFixedWidth(EDIT_DATE_WIDTH);
    editLayout->addRow(startLabel, startEdit);

    finishLabel = new QLabel(tr("Finish"));
    finishEdit = new QDateEdit;
    finishEdit->setFixedWidth(EDIT_DATE_WIDTH);
    editLayout->addRow(finishLabel, finishEdit);

    vatRateLabel = new QLabel(tr("VAT Rate (%)"));
    vatRateEdit = new QLineEdit;
    vatRateEdit->setFixedWidth(LINE_EDIT_WIDTH);
    vatRateEdit->setAlignment(Qt::AlignRight);
    vatRateEdit->setInputMask("09");
    vatRateEdit->setMaxLength(2);
    editLayout->addRow(vatRateLabel, vatRateEdit);
    connect(vatRateEdit, SIGNAL(textChanged(QString)),
                                    SLOT(calcSum()));

    contractSumLabel = new QLabel(tr("Contract Sum"));
    contractSumEdit = new QLineEdit;
    contractSumEdit->setFixedWidth(LINE_EDIT_WIDTH);
    contractSumEdit->setAlignment(Qt::AlignRight);
    contractSumEdit->setMaxLength(15);
    editLayout->addRow(contractSumLabel, contractSumEdit);
    connect(contractSumEdit, SIGNAL(textChanged(QString)),
                                    SLOT(calcSum()));

    vatSumLabel = new QLabel(tr("VAT Sum"));
    vatSumEdit = new QLineEdit;
    vatSumEdit->setFixedWidth(LINE_EDIT_WIDTH);
    vatSumEdit->setReadOnly(true);
    vatSumEdit->setStyleSheet(GRAY_STYLE);
    vatSumEdit->setAlignment(Qt::AlignRight);
    editLayout->addRow(vatSumLabel, vatSumEdit);

    totalSumLabel = new QLabel(tr("Total Sum"));
    totalSumEdit = new QLineEdit;
    totalSumEdit->setFixedWidth(LINE_EDIT_WIDTH);
    totalSumEdit->setReadOnly(true);
    totalSumEdit->setStyleSheet(GRAY_STYLE);
    totalSumEdit->setAlignment(Qt::AlignRight);
    editLayout->addRow(totalSumLabel, totalSumEdit);

    contractNoteLabel = new QLabel(tr("Notes"));
    contractNoteEdit = new QPlainTextEdit;
    contractNoteEdit->setFixedHeight(TEXT_EDIT_HEIGHT);
    editLayout->addRow(contractNoteLabel, contractNoteEdit);

    updatedDateLabel = new QLabel(tr("Updated date"));
    updatedDateEdit = new QLineEdit;
    updatedDateEdit->setFixedWidth(LINE_EDIT_WIDTH);
    updatedDateEdit->setReadOnly(true);
    updatedDateEdit->setInputMask("0000-00-00 00:00:00");
    updatedDateEdit->setStyleSheet(GRAY_STYLE);
    editLayout->addRow(updatedDateLabel, updatedDateEdit);

    updatedByLabel = new QLabel(tr("Updated by"));
    updatedByEdit = new QLineEdit;
    updatedByEdit->setFixedWidth(LINE_EDIT_WIDTH);
    updatedByEdit->setReadOnly(true);
    updatedByEdit->setStyleSheet(GRAY_STYLE);
    editLayout->addRow(updatedByLabel, updatedByEdit);

    editBox->setHidden(true);
    editBox->setLayout(editLayout);

    // установить отображение модели на визуальные компоненты
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));

    int typeIndex = 1;
    int stateIndex = 2;
    int partnerIndex = 3;

    mapper->addMapping(contractIdEdit, model->fieldIndex("contract_id"));
    mapper->addMapping(typeComboBox, typeIndex);
    mapper->addMapping(stateComboBox, stateIndex);
    mapper->addMapping(partnerComboBox, partnerIndex);
    mapper->addMapping(contractNumEdit, model->fieldIndex("contract_num"));
    mapper->addMapping(contractDateEdit, model->fieldIndex("contract_date"));
    mapper->addMapping(startEdit, model->fieldIndex("start_date"));
    mapper->addMapping(finishEdit, model->fieldIndex("finish_date"));
    mapper->addMapping(vatRateEdit, model->fieldIndex("vat_rate"));
    mapper->addMapping(contractSumEdit, model->fieldIndex("contract_sum"));
    mapper->addMapping(vatSumEdit, model->fieldIndex("vat_sum"));
    mapper->addMapping(totalSumEdit, model->fieldIndex("total_sum"));
    mapper->addMapping(contractNoteEdit, model->fieldIndex("contract_note"));
    mapper->addMapping(updatedDateEdit, model->fieldIndex("updated_date"));
    mapper->addMapping(updatedByEdit, model->fieldIndex("updated_by"));

    typeComboBox->setModel(model->relationModel(typeIndex));
    typeComboBox->setModelColumn(
                model->relationModel(typeIndex)->fieldIndex("type_name"));
    stateComboBox->setModel(model->relationModel(stateIndex));
    stateComboBox->setModelColumn(
                model->relationModel(stateIndex)->fieldIndex("state_name"));
    partnerComboBox->setModel(model->relationModel(partnerIndex));
    partnerComboBox->setModelColumn(model->relationModel(
                                 partnerIndex)->fieldIndex("partner_name"));
    mapper->toFirst();

} // createEditBox()

// создание кнопок
void ContractWidget::createControlBox()
{
    controlBox = new QGroupBox;
    insertButton = new QPushButton(tr("Insert"));
    editButton = new QPushButton(tr("Edit"));
    deleteButton = new QPushButton(tr("Delete"));
    revertButton   = new QPushButton(tr("Revert"));
    submitButton = new QPushButton(tr("Submit"));
    controlLayout = new QHBoxLayout;
    controlLayout->addWidget(insertButton);
    controlLayout->addWidget(editButton);
    controlLayout->addWidget(deleteButton);
    controlLayout->addWidget(revertButton);
    controlLayout->addWidget(submitButton);
    controlBox->setLayout(controlLayout);
    controlBox->setFixedHeight(CONTROL_BOX_HEIGHT);

} // createControlBox()

// слот вставки строки
void ContractWidget::insertRow()
{
    m_mode = Mode::FORM;
    m_currentId = 0;

    int crow = 0;
    model->insertRow(crow);
    tableView->selectRow(crow);

    mapper->toFirst();

    // установить начальные данные
    contractIdEdit->setText("0");

    typeComboBox->setCurrentIndex(0);
    QModelIndex idx = typeComboBox->model()->index(0, 0);
    model->record(crow).setValue("type_id", typeComboBox->model()->data(idx));

    stateComboBox->setCurrentIndex(0);
    idx = stateComboBox->model()->index(0, 0);
    model->record(crow).setValue("state_id", stateComboBox->model()->data(idx));

    partnerComboBox->setCurrentIndex(0);
    idx = partnerComboBox->model()->index(0, 0);
    model->record(crow).setValue("partner_id",
                                 partnerComboBox->model()->data(idx));

    contractDateEdit->setDate(QDate::currentDate());
    startEdit->setDate(QDate::currentDate());
    finishEdit->setDate(QDate::currentDate());
    vatRateEdit->setText("0");
    contractSumEdit->setText("0");
    vatSumEdit->setText("0");
    totalSumEdit->setText("0");
    updatedDateEdit->setText(QDateTime::currentDateTime().toString(Qt::ISODate));

    updatedByEdit->setText(
                settings->value("userName", QString("postgres")).toString());

    mapper->submit();

    setWidgets();

} // insertRow()

// слот правки строки
void ContractWidget::editRow()
{
    m_mode = Mode::FORM;
    m_currentId = QVariant(contractIdEdit->text()).toInt();
    setWidgets();

} // editRow()

// слот удаления договора
void ContractWidget::deleteRow()
{
    model->removeRow(tableView->currentIndex().row());
    setWidgets();

} // deleteRow()

// слот отката всех изменений
void ContractWidget::revert()
{
    model->revertAll();
    m_mode = Mode::TABLE;

    // обновить таблицу
    tableView->hide();
    tableView->show();

    setWidgets();

} // revert()

// слот утверждения всех изменений
void ContractWidget::submit()
{
    if (m_mode == Mode::FORM) {
        if (contractIdEdit->text() == "0") {
            // перед сохранением нового договора вставить идентификатор
            contractIdEdit->setText(dm->getNextContractIdAsString());
        }
        mapper->submit();
    }

    model->database().transaction();
    if (model->submitAll()) {
        model->database().commit();

        // правка успешно сохранена
        if (m_currentId > 0) {
            // выбрать отредактированную запись
            for (int i = 0; i < model->rowCount(); ++i) {
                if (m_currentId ==
                    model->record(i).value("contract_id").toInt()) {
                    tableView->selectRow(i);
                    break;
                }
            }
        }

        m_mode = Mode::TABLE;
    } else {
        model->database().rollback();
        QMessageBox::critical(this, tr("Contract Application"),
                             tr("The database reported an error: %1")
                             .arg(model->lastError().text()));
    }

    setWidgets();

} // submit()

// слот подсчета НДС и суммы договора с НДС
// и вставки этих значений в соответствующие поля формы
void ContractWidget::calcSum()
{
    double vatRate = vatRateEdit->text().toDouble() / 100;
    double contractSum = contractSumEdit->text().toDouble();
    double vatSum = contractSum * vatRate;
    double totalSum = contractSum + vatSum;
    vatSumEdit->setText(QVariant(vatSum).toString());
    totalSumEdit->setText(QVariant(totalSum).toString());
} // calcSum()

// слот обработки изменения данных
void ContractWidget::dataChanged(const QModelIndex &topLeft,
                                 const QModelIndex &bottomRight)
{
    // hide warnings
    Q_UNUSED(topLeft)
    Q_UNUSED(bottomRight)

    // измененить состояние кнопок
    setWidgets();

} // dataChanged()

// установить виджеты в зависимости от...
void ContractWidget::setWidgets()
{
    // строка таблицы не выбрана, выбрать первую, если возможно
    if ((tableView->currentIndex().row() < 0) && (model->rowCount() > 0)) {
        tableView->setCurrentIndex(model->index(0, 1));
        QItemSelection selection(model->index(0, 1), model->index(0, 1));
        tableView->selectionModel()->select(selection,
                                            QItemSelectionModel::Select);
    }

    // показать таблицу или форму в зависимости от режима
    if (m_mode == Mode::TABLE) {
        tableBox->setVisible(true);
        editBox->setHidden(true);
    } else {
        tableBox->setHidden(true);
        editBox->setVisible(true);
    }

    // установить кнопки
    insertButton->setEnabled(m_mode == Mode::TABLE);
    editButton->setEnabled( (m_mode == Mode::TABLE) &&
                            (model->rowCount() > 0));
    deleteButton->setEnabled( (m_mode == Mode::TABLE) &&
                              (model->rowCount() > 0));

    revertButton->setEnabled(model->isDirty() || (m_mode == Mode::FORM));
    submitButton->setEnabled(model->isDirty());

} // setWidgets()
