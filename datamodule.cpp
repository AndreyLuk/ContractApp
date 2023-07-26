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
#include <QSqlQuery>
#include <QSqlError>

#include "datamodule.h"

// конструктор
DataModule::DataModule(QWidget *p_parent)
    : parent(p_parent)
{
    // подключить базу данных PostgreSQL по умолчанию или сообщить об ошибке
    if (QSqlDatabase::drivers().contains("QPSQL")) {
        m_db = QSqlDatabase::addDatabase("QPSQL");
    } else {
        QMessageBox::critical(parent, tr("Database driver error"),
                              tr("Unable to load QPSQL database driver."));
    }

} // DataModule::DataModule()

// деструктор
DataModule::~DataModule()
{
    removeContractTypeModel();
    removeContractStateModel();
    removePartnerModel();
    removeContractModel();

    if (databaseOpened()) {
        m_db.close();
    }
} // ~DataModule()

// открыть базу данных, возвращает "истина" в случае успеха
bool DataModule::openDatabase()
{
    if (!m_db.isValid()) {
        return false;
    }
    if (!databaseOpened()) {
        m_db.setHostName(m_hostName);
        m_db.setPort(m_port);
        m_db.setDatabaseName(m_databaseName);
        m_db.setUserName(m_userName);
        m_db.setPassword(m_password);

        if (!m_db.open()) {
            QMessageBox::critical(parent, tr("Database error"),
                                  m_db.lastError().text());
            return false;
        } else {
            QSqlQuery query1(m_db);
            bool result1 = query1.exec("set search_path to " + m_schema);
            QSqlQuery query2(m_db);
            bool result2 =
                    query2.exec("select count(contract_id) from contract");
            if (!(result1 && result2)) {
                QMessageBox::critical(parent, tr("Database error"),
                  tr(
                   "Wrong database schema or table <<contract>> does not exist."
                  ));
                m_db.close();
                return false;
            }
            return true;
        }
    } else {
        return true;    // база данных была открыта раньше
    }
} // openDatabase()

// проверить, открыта ли база данных
bool DataModule::databaseOpened()
{
    return m_db.isOpen();
} // databaseOpened()

// вернуть QSqlDatabase
QSqlDatabase DataModule::database()
{
    return m_db;
} // database()

// получить наименование хоста
const QString& DataModule::hostName()
{
    return m_hostName;
} // hostName()

// установить наименование хоста
void DataModule::setHostName(const QString& p_hostName)
{
    m_hostName = p_hostName;
} // setHostName()

// получить порт
const int& DataModule::port()
{
    return m_port;
} // port()

// установить порт
void DataModule::setPort(const int& p_port)
{
    m_port = p_port;
} // setPort()

// получить наименование базы данных
const QString& DataModule::databaseName()
{
    return m_databaseName;
} // databaseName()

// установить наименование базы данных
void DataModule::setDatabaseName(const QString& p_databaseName)
{
    m_databaseName = p_databaseName;
} // setDatabaseName()

// получить наименование схемы
const QString& DataModule::schema()
{
    return m_schema;
} // schema()

// установить наименование схемы
void DataModule::setSchema(const QString& p_schema)
{
    m_schema = p_schema;
} // setSchema()

// получить имя пользователя
const QString& DataModule::userName()
{
    return m_userName;
} // userName()

// установить имя пользователя
void DataModule::setUserName(const QString& p_userName)
{
    m_userName = p_userName;
} // setUserName()

// получить пароль
const QString& DataModule::password()
{
    return m_password;
} // password()

// установить пароль
void DataModule::setPassword(const QString& p_password)
{
    m_password = p_password;
} // setPassword()

// создать модель данных справочника типов договоров
void DataModule::createContractTypeModel()
{
    if (contractTypeModel == nullptr) {
        contractTypeModel = new QSqlTableModel(this, m_db);
        contractTypeModel->setTable(m_schema + "." + "contract_type");

        // данные сохраняются без управления транзакциями
        contractTypeModel->setEditStrategy(QSqlTableModel::OnRowChange);
        // сортировать по наименованию
        contractTypeModel->setSort(1, Qt::AscendingOrder);
        // установить заголовки полей
        contractTypeModel->setHeaderData(
                    contractTypeModel->fieldIndex("type_id"),
                    Qt::Horizontal, tr("ID"));
        contractTypeModel->setHeaderData(
                    contractTypeModel->fieldIndex("type_name"),
                    Qt::Horizontal, tr("Contract Type"));

        if (!contractTypeModel->select()) {
            // ошибка выбора данных
            QMessageBox::critical(parent, tr("Database error"),
                                  contractTypeModel->lastError().text());
        }
    }
} // createContractTypeModel()

// получить указатель на модель данных справочника типов договоров
QSqlTableModel *DataModule::getContractTypeModel()
{
    return contractTypeModel;

} // getContractTypeModel()

// получить следующий ID строки справочника типов договоров
int DataModule::getNextContractTypeID()
{
    int nextID = -1;

    QSqlQuery query("select max(type_id) from " + m_schema +
                    ".contract_type", m_db);

    if (query.first()) {
        nextID = query.value(0).toInt();
        ++nextID;
    }

    return nextID;
} // getNextContractTypeID()

// удалить модель данных справочника типов договоров
void DataModule::removeContractTypeModel()
{
    if (contractTypeModel != nullptr) {
        // отменить все изменения, если есть
        if (contractTypeModel->isDirty())
            contractTypeModel->revert();
        delete contractTypeModel;
        contractTypeModel = nullptr;
    }
} // removeContractTypeModel()

// создать модель данных справочника состояний договоров
void DataModule::createContractStateModel()
{
    if (contractStateModel == nullptr) {
        contractStateModel = new QSqlTableModel(this, m_db);
        contractStateModel->setTable(m_schema + "." + "contract_state");

        // данные сохраняются под управлением транзакциями
        contractStateModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        // сортировать по наименованию
        contractStateModel->setSort(1, Qt::AscendingOrder);
        // установить заголовки полей
        contractStateModel->setHeaderData(
                    contractStateModel->fieldIndex("state_id"),
                    Qt::Horizontal, tr("ID"));
        contractStateModel->setHeaderData(
                    contractStateModel->fieldIndex("state_name"),
                    Qt::Horizontal, tr("Contract State"));

        if (!contractStateModel->select()) {
            // ошибка выбора данных
            QMessageBox::critical(parent, tr("Database error"),
                                  contractStateModel->lastError().text());
        }
    }
} // createContractStateModel()

// получить указатель на модель данных справочника состояний договоров
QSqlTableModel *DataModule::getContractStateModel()
{
    return contractStateModel;

} // getContractStateModel()

// удалить модель данных справочника состояний договоров
void DataModule::removeContractStateModel()
{
    if (contractStateModel != nullptr) {
        // отменить все транзакции, если есть
        if (contractStateModel->isDirty())
            contractStateModel->revertAll();
        delete contractStateModel;
        contractStateModel = nullptr;
    }
} // removeContractStateModel()

// создать модель данных справочника контрагентов
void DataModule::createPartnerModel()
{
    if (partnerModel == nullptr) {
        partnerModel = new QSqlTableModel(this, m_db);
        partnerModel->setTable(m_schema + "." + "partner");

        // данные сохраняются под управлением транзакциями
        partnerModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
        // сортировать по наименованию
        partnerModel->setSort(1, Qt::AscendingOrder);
        // установить заголовки полей
        partnerModel->setHeaderData(
                    partnerModel->fieldIndex("partner_id"),
                    Qt::Horizontal, tr("ID"));
        partnerModel->setHeaderData(
                    partnerModel->fieldIndex("partner_name"),
                    Qt::Horizontal, tr("Partner Name"));
        partnerModel->setHeaderData(
                    partnerModel->fieldIndex("inum"),
                    Qt::Horizontal, tr("Tax Number"));

        if (!partnerModel->select()) {
            // ошибка выбора данных
            QMessageBox::critical(parent, tr("Database error"),
                                  partnerModel->lastError().text());
        }
    }
} // createPartnerModel()

// получить указатель на модель данных справочника контрагентов
QSqlTableModel *DataModule::getPartnerModel()
{
    return partnerModel;

} // getPartnerModel()

// удалить модель данных справочника контрагентов
void DataModule::removePartnerModel()
{
    if (partnerModel != nullptr) {
        // отменить все транзакции, если есть
        if (partnerModel->isDirty())
            partnerModel->revertAll();
        delete partnerModel;
        partnerModel = nullptr;
    }
} // removePartnerModel()

// создать модель данных договоров
void DataModule::createContractModel()
{
    if (contractModel == nullptr) {
        contractModel = new QSqlRelationalTableModel(this, m_db);
        contractModel->setTable(m_schema + "." + "contract");

        // данные сохраняются под управлением транзакциями
        contractModel->setEditStrategy(
                    QSqlRelationalTableModel::OnManualSubmit);
        // сортировать по ID
        contractModel->setSort(0, Qt::AscendingOrder);

        // индексы полей, связанных с другими таблицами
        int typeIndex = contractModel->fieldIndex("type_id");
        int stateIndex = contractModel->fieldIndex("state_id");
        int partnerIndex = contractModel->fieldIndex("partner_id");

        // установить заголовки полей
        contractModel->setHeaderData(contractModel->fieldIndex("contract_id"),
                             Qt::Horizontal, tr("ID"));
        contractModel->setHeaderData(typeIndex,
                             Qt::Horizontal, tr("Type"));
        contractModel->setHeaderData(stateIndex,
                             Qt::Horizontal, tr("State"));
        contractModel->setHeaderData(partnerIndex,
                             Qt::Horizontal, tr("Partner"));
        contractModel->setHeaderData(contractModel->fieldIndex("contract_num"),
                             Qt::Horizontal, tr("Contract Num"));
        contractModel->setHeaderData(contractModel->fieldIndex("contract_date"),
                             Qt::Horizontal, tr("Contract Date"));
        contractModel->setHeaderData(contractModel->fieldIndex("start_date"),
                             Qt::Horizontal, tr("Start"));
        contractModel->setHeaderData(contractModel->fieldIndex("finish_date"),
                             Qt::Horizontal, tr("Finish"));
        contractModel->setHeaderData(contractModel->fieldIndex("vat_rate"),
                             Qt::Horizontal, tr("VAT Rate"));
        contractModel->setHeaderData(contractModel->fieldIndex("contract_sum"),
                             Qt::Horizontal, tr("Contract Sum"));
        contractModel->setHeaderData(contractModel->fieldIndex("vat_sum"),
                             Qt::Horizontal, tr("VAT Sum"));
        contractModel->setHeaderData(contractModel->fieldIndex("total_sum"),
                             Qt::Horizontal, tr("Total Sum"));
        contractModel->setHeaderData(contractModel->fieldIndex("contract_note"),
                             Qt::Horizontal, tr("Note"));
        contractModel->setHeaderData(contractModel->fieldIndex("updated_date"),
                             Qt::Horizontal, tr("Updated"));
        contractModel->setHeaderData(contractModel->fieldIndex("updated_by"),
                             Qt::Horizontal, tr("Updated by"));

        // установить связь с другими таблицами
        contractModel->setRelation(typeIndex, QSqlRelation("contract_type",
                                                 "type_id", "type_name"));
        contractModel->setRelation(stateIndex, QSqlRelation("contract_state",
                                                  "state_id", "state_name"));
        contractModel->setRelation(partnerIndex, QSqlRelation("partner",
                                               "partner_id", "partner_name"));

        if (!contractModel->select()) {
            // ошибка выбора данных
            QMessageBox::critical(parent, tr("Database error"),
                                  contractModel->lastError().text());
        }
    }
} // createContractModel()

// получить указатель на модель данных договоров
QSqlRelationalTableModel *DataModule::getContractModel()
{
    return contractModel;

} // getContractModel()

// получить следующий ID договора как строку
QString DataModule::getNextContractIdAsString()
{
    QSqlQuery query(m_db);
    query.exec("select (max(contract_id) + 1) from " +
               m_schema + ".contract");
    query.first();
    return query.value(0).toString();

} // getNextContractIdAsString()

// удалить модель данных договоров
void DataModule::removeContractModel()
{
    if (contractModel != nullptr) {
        // отменить все транзакции, если есть
        if (contractModel->isDirty())
            contractModel->revertAll();
        delete contractModel;
        contractModel = nullptr;
    }
} // removeContractModel()
