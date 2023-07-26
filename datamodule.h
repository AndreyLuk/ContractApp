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

#ifndef DATAMODULE_H
#define DATAMODULE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>

/*
 * модуль для работы с базой данных PostgreSQL
 */
class DataModule: QObject
{
    Q_OBJECT
public:
    explicit        DataModule(QWidget *p_parent);
    virtual         ~DataModule();

    bool            openDatabase();
    bool            databaseOpened();
    QSqlDatabase    database();

    const QString&  hostName();
    void            setHostName(const QString& p_hostName);
    const int&      port();
    void            setPort(const int& p_port);
    const QString&  databaseName();
    void            setDatabaseName(const QString& p_databaseName);
    const QString&  schema();
    void            setSchema(const QString& p_schema);
    const QString&  userName();
    void            setUserName(const QString& p_userName);
    const QString&  password();
    void            setPassword(const QString& p_password);

    void            createContractTypeModel();
    QSqlTableModel  *getContractTypeModel();
    int             getNextContractTypeID();
    void            removeContractTypeModel();

    void            createContractStateModel();
    QSqlTableModel  *getContractStateModel();
    void            removeContractStateModel();

    void            createPartnerModel();
    QSqlTableModel  *getPartnerModel();
    void            removePartnerModel();

    void                        createContractModel();
    QSqlRelationalTableModel    *getContractModel();
    QString                     getNextContractIdAsString();
    void                        removeContractModel();

private:
    QWidget         *parent {nullptr};      // указатель на родительский виджет
    QSqlDatabase    m_db {QSqlDatabase()};  // соединение с базой данных
    QString         m_hostName {QString()}; // наименование хоста
    int             m_port {5432};          // номер порта
    QString         m_databaseName {QString()}; // наименование базы данных
    QString         m_schema {QString()};   // схема
    QString         m_userName {QString()}; // имя пользователя
    QString         m_password {QString()}; // пароль

    // модель данных справочника типов договоров
    QSqlTableModel  *contractTypeModel {nullptr};
    // модель данных справочника состояний договоров
    QSqlTableModel  *contractStateModel {nullptr};
    // модель данных справочника контрагентов
    QSqlTableModel  *partnerModel {nullptr};
    // модель данных договоров
    QSqlRelationalTableModel    *contractModel {nullptr};

};

#endif // DATAMODULE_H
