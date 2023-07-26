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

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlError>
#include <QHeaderView>

#include "contracttypewidget.h"

// конструктор справочника типов договоров
ContractTypeWidget::ContractTypeWidget(QSettings *p_settings,
                                       DataModule *p_dm,
                                       QWidget *p_parent)
    : QWidget(p_parent),
      settings(p_settings),
      dm(p_dm)
{
    setFixedSize(WIDGET_WIDTH, WIDGET_HEIGHT);

    // создание модели данных справочника
    dm->createContractTypeModel();
    model = dm->getContractTypeModel();

    // создание элементов интерфейса

    // раскладка справочника - вертикальная
    referenceLayout = new QVBoxLayout;
    // раскладка кнопок - горизонтальная
    buttonLayout = new QHBoxLayout;

    // создание таблицы
    tableView = new QTableView;
    setTableView();
    referenceLayout->addWidget(tableView);

    // создание кнопок
    insertButton = new QPushButton(tr("Insert"));
    editButton = new QPushButton(tr("Edit"));
    deleteButton = new QPushButton(tr("Delete"));
    buttonLayout->addWidget(insertButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    referenceLayout->addLayout(buttonLayout);

    // установить раскладку справочника
    setLayout(referenceLayout);

    // соединение кнопок и слотов
    connect(insertButton, &QPushButton::clicked,
            this, &ContractTypeWidget::insertRow);
    connect(editButton, &QPushButton::clicked,
            this, &ContractTypeWidget::editRow);
    connect(deleteButton, &QPushButton::clicked,
            this, &ContractTypeWidget::deleteRow);

    setWidgets();

} // ContractTypeWidget::ContractTypeWidget()

// деструктор
ContractTypeWidget::~ContractTypeWidget()
{
    // удалить  модель данных справочника
    dm->removeContractTypeModel();
} // ~ContractTypeWidget()

// установка и настройка таблицы
void ContractTypeWidget::setTableView()
{
    tableView->setModel(model);
    tableView->setAlternatingRowColors(true);
    tableView->setColumnHidden(model->fieldIndex("type_id"), true);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->horizontalHeader()->setStretchLastSection(true);
} // setTableView()

// слот вставки строки
void ContractTypeWidget::insertRow()
{
    // создание диалогового окна вставки записи
    QDialog dialogBox(this);
    dialogBox.setWindowTitle(tr("Insertion"));
    dialogBox.setFixedSize(DIALOG_WIDTH, DIALOG_HEIGHT);
    QGridLayout boxlayout(&dialogBox);

    QLabel nameLabel(tr("Contract type"), &dialogBox);
    nameLabel.setFixedWidth(ITEM_WIDTH);
    QLineEdit nameEdit(&dialogBox);
    nameEdit.setFixedWidth(ITEM_WIDTH);
    boxlayout.addWidget(&nameLabel, 0, 0);
    boxlayout.addWidget(&nameEdit, 0, 1);

    QPushButton saveButton(tr("Save"), &dialogBox);
    saveButton.setFixedWidth(ITEM_WIDTH);
    QPushButton cancelButton(tr("Cancel"), &dialogBox);
    cancelButton.setFixedWidth(ITEM_WIDTH);
    cancelButton.setDefault(true);

    QDialogButtonBox *buttonBox =
            new QDialogButtonBox(Qt::Horizontal, &dialogBox);
    buttonBox->addButton(&saveButton, QDialogButtonBox::YesRole);
    buttonBox->addButton(&cancelButton, QDialogButtonBox::NoRole);
    boxlayout.addWidget(buttonBox, 1, 0, 1, 2, Qt::AlignHCenter);

    connect(buttonBox, &QDialogButtonBox::accepted,
            &dialogBox, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected,
            &dialogBox, &QDialog::reject);

    int nextId = dm->getNextContractTypeID();

    // запуск диалога вставки записи
    if (dialogBox.exec()) {     // нажата кнопка "сохранение"

        if ((nameEdit.text().length() > 0) && (nextId > 0)) {
            // значение введено и следующий ID получен

            QSqlRecord record;
            QSqlField field1("type_id", QVariant::Int);
            QSqlField field2("type_name", QVariant::String);
            field1.setValue(QVariant(nextId));
            field2.setValue(QVariant(nameEdit.text()));
            record.append(field1);
            record.append(field2);

            if (model->insertRecord(-1, record)) { // запись вставлена

                // обновить модель данных и таблицу
                model->select();
                setTableView();

                // Выбрать вставленную запись
                for (int i = 0; i < model->rowCount(); ++i) {
                    if (nameEdit.text() ==
                        model->record(i).value("type_name").toString()) {
                        tableView->selectRow(i);
                        break;
                    }
                }

            } else {    // ошибка сохранения
                QMessageBox::critical(this, tr("Contract Application"),
                         tr("The database reported an error: %1")
                         .arg(model->lastError().text()));
            }
        }
    }
    setWidgets();

} // insertRow()

// слот правки строки
void ContractTypeWidget::editRow()
{
    // данные о текущей записи
    int currentRow = tableView->currentIndex().row();
    QModelIndex fieldIndex = model->index(currentRow, 1);
    QString oldTypeName(model->data(model->index(currentRow, 1)).toString());

    // создание диалогового окна
    QDialog dialogBox(this);
    dialogBox.setWindowTitle(tr("Edition"));
    dialogBox.setFixedSize(DIALOG_WIDTH, DIALOG_HEIGHT);
    QGridLayout boxlayout(&dialogBox);

    QLabel nameLabel(tr("Contract type"), &dialogBox);
    nameLabel.setFixedWidth(ITEM_WIDTH);
    QLineEdit nameEdit(oldTypeName, &dialogBox);
    nameEdit.setFixedWidth(ITEM_WIDTH);
    boxlayout.addWidget(&nameLabel, 0, 0);
    boxlayout.addWidget(&nameEdit, 0, 1);

    QPushButton saveButton(tr("Save"), &dialogBox);
    saveButton.setFixedWidth(ITEM_WIDTH);
    QPushButton cancelButton(tr("Cancel"), &dialogBox);
    cancelButton.setFixedWidth(ITEM_WIDTH);
    cancelButton.setDefault(true);

    QDialogButtonBox *buttonBox =
            new QDialogButtonBox(Qt::Horizontal, &dialogBox);
    buttonBox->addButton(&saveButton, QDialogButtonBox::YesRole);
    buttonBox->addButton(&cancelButton, QDialogButtonBox::NoRole);
    boxlayout.addWidget(buttonBox, 1, 0, 1, 2, Qt::AlignHCenter);

    connect(buttonBox, &QDialogButtonBox::accepted,
            &dialogBox, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected,
            &dialogBox, &QDialog::reject);

    // запуск диалога правки записи
    if (dialogBox.exec()) {     // нажата кнопка "сохранение"

        if ( ( oldTypeName != nameEdit.text() ) &&
            ( !nameEdit.text().isEmpty() ) ) {
            // значение изменено и оно не пустое

            model->setData(fieldIndex, QVariant(nameEdit.text()));

            // сохранить запись
            if (model->submit()) {
                // запись сохранена

                // обновить модель данных и тпблицу
                model->select();
                setTableView();

                // Выбрать вставленную запись
                for (int i = 0; i < model->rowCount(); ++i) {
                    if (nameEdit.text() ==
                        model->record(i).value("type_name").toString()) {
                        tableView->selectRow(i);
                        break;
                    }
                }

            } else {
                // ошибка сохранения
                QMessageBox::critical(this, tr("Contract Application"),
                         tr("The database reported an error: %1")
                         .arg(model->lastError().text()));
            }
        }
    }
} // editRow()

// слот удаления строки
void ContractTypeWidget::deleteRow()
{
    // данные о текущей записи
    int currentRow = tableView->currentIndex().row();
    QModelIndex fieldIndex = model->index(currentRow, 1);
    QString strOldTypeName(model->data(fieldIndex).toString());

    // создание диалогового окна
    QDialog dialogBox(this);
    dialogBox.setWindowTitle(tr("Deletion"));
    dialogBox.setFixedSize(DIALOG_WIDTH, DIALOG_HEIGHT);
    QGridLayout boxlayout(&dialogBox);

    QLabel nameLabel(tr("Contract type"), &dialogBox);
    nameLabel.setFixedWidth(ITEM_WIDTH);
    QLineEdit nameEdit(strOldTypeName, &dialogBox);
    nameEdit.setFixedWidth(ITEM_WIDTH);
    nameEdit.setDisabled(true);
    boxlayout.addWidget(&nameLabel, 0, 0);
    boxlayout.addWidget(&nameEdit, 0, 1);

    QPushButton deleteButton(tr("Delete"), &dialogBox);
    deleteButton.setFixedWidth(ITEM_WIDTH);
    QPushButton cancelButton(tr("Cancel"), &dialogBox);
    cancelButton.setFixedWidth(ITEM_WIDTH);
    cancelButton.setDefault(true);

    QDialogButtonBox *buttonBox =
            new QDialogButtonBox(Qt::Horizontal, &dialogBox);
    buttonBox->addButton(&deleteButton, QDialogButtonBox::YesRole);
    buttonBox->addButton(&cancelButton, QDialogButtonBox::NoRole);
    boxlayout.addWidget(buttonBox, 1, 0, 1, 2, Qt::AlignHCenter);

    connect(buttonBox, &QDialogButtonBox::accepted,
            &dialogBox, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected,
            &dialogBox, &QDialog::reject);

    // запуск диалога удаления записи
    if (dialogBox.exec()) {     // нажата кнопка "удаление"

        if (model->removeRow(currentRow)) {  // удалить запись

        } else {    // ошибка удаления
            QMessageBox::critical(this, tr("Contract Application"),
                     tr("The database reported an error: %1")
                     .arg(model->lastError().text()));
        }
        // обновить данные и выбрать первую запись
        model->select();
        setTableView();
        tableView->selectRow(0);

    }
    setWidgets();
} // deleteRow()

// установить виджеты в зависимости от...
void ContractTypeWidget::setWidgets()
{
    // в таблице не выбрана строка - выбрать первую, если есть
    if ((tableView->currentIndex().row() < 0) &&
        (model->rowCount() > 0))
    {
        tableView->setCurrentIndex(model->index(0, 1));
        QItemSelection selection(model->index(0, 1),
                                 model->index(0, 1));
        tableView->selectionModel()->select(selection,
                                            QItemSelectionModel::Select);
    }
    // установить состояние кнопок правки и удаления
    if (model->rowCount() > 0) {
        editButton->setEnabled(true);
        deleteButton->setEnabled(true);
    } else {
        editButton->setDisabled(true);
        deleteButton->setDisabled(true);
    }
} // setWidgets()
