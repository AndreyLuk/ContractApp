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

#include "startwidget.h"

// конструктор виджета "начало"
StartWidget::StartWidget(QSettings *p_settings,
                         DataModule *p_dm,
                         QWidget *p_parent)
    : QWidget(p_parent),
      settings(p_settings),
      dm(p_dm)
{
    setFixedSize(WIDGET_WIDTH, WIDGET_HEIGHT);

    // создать главную группу
    mainStartBox = new QGroupBox(tr(""), this);
    mainStartLayout = new QVBoxLayout;

    // создать группу подключения к базе
    dbBox = new QGroupBox;
    dbBox->setTitle(tr("Database connection"));
    dbLayout = new QFormLayout;

    hostNameLabel = new QLabel(tr("Host"));
    hostNameLabel->setFixedWidth(width() / 3);
    hostNameEdit = new QLineEdit;
    dbLayout->addRow(hostNameLabel, hostNameEdit);

    portLabel = new QLabel(tr("Port"));
    portEdit = new QLineEdit;
    dbLayout->addRow(portLabel, portEdit);

    databaseNameLabel = new QLabel(tr("Database"));
    databaseNameEdit = new QLineEdit;
    dbLayout->addRow(databaseNameLabel, databaseNameEdit);

    schemaLabel = new QLabel(tr("Schema"));
    schemaEdit = new QLineEdit;
    dbLayout->addRow(schemaLabel, schemaEdit);

    userNameLabel = new QLabel(tr("User"));
    userNameEdit = new QLineEdit;
    dbLayout->addRow(userNameLabel, userNameEdit);

    passwordLabel = new QLabel(tr("Password"));
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    dbLayout->addRow(passwordLabel, passwordEdit);

    // заполнить текстовые поля информацией от предыдущего сеанса,
    // в котором было успешное соединение с базой данных
    readDbConnectionInfo();

    connectButton = new QPushButton(tr("Connect"));
    connectButton->setMinimumWidth(100);
    dbLayout->addRow(connectButton);

    dbBox->setLayout(dbLayout);
    mainStartLayout->addWidget(dbBox);

    // создать группу опций
    optionBox = new QGroupBox(tr("Options"));
    optionLayout = new QFormLayout;

    // тема
    themeLabel = new QLabel(tr("Theme"));
    themeCombo = new QComboBox;

    QStringList themeList = QStyleFactory::keys();

    for (auto i = 0; i < themeList.length(); ++i) {
        themeCombo->insertItem(i, themeList.at(i));
    }

    optionLayout->addRow(themeLabel, themeCombo);

    // язык
    langLabel = new QLabel(tr("Language"));
    languageCombo = new QComboBox;
    languageCombo->insertItem(0, tr("English"));
    languageCombo->insertItem(1, tr("Russian"));
    optionLayout->addRow(langLabel, languageCombo);

    languageCombo->setCurrentIndex(settings->value("languageIndex", 0).toInt());

    optionBox->setLayout(optionLayout);
    mainStartLayout->addWidget(optionBox);
    mainStartBox->setLayout(mainStartLayout);

    setDbBoxItems(false);

    connect(connectButton, &QPushButton::clicked,
            this, &StartWidget::openDatabase);
    connect(themeCombo, &QComboBox::currentTextChanged,
            this, &StartWidget::setTheme);
    connect(languageCombo, SIGNAL(currentIndexChanged(int)),
            SLOT(setLanguage(int)));
    themeCombo->setCurrentIndex(themeCombo->findText(
                                    p_settings->value("theme").toString()));
    setDbBoxItems(dm->databaseOpened());

} // StartWidget::StartWidget()

// деструктор
StartWidget::~StartWidget()
{
    writeDbConnectionInfo();
} // ~StartWidget()

// слот открывает базу данных
void StartWidget::openDatabase()
{
    dm->setHostName(hostNameEdit->text());
    dm->setPort(QVariant(portEdit->text()).toInt());
    dm->setDatabaseName(databaseNameEdit->text());
    dm->setSchema(schemaEdit->text());
    dm->setUserName(userNameEdit->text());
    dm->setPassword(passwordEdit->text());

    if (dm->openDatabase()) {
        setDbBoxItems(dm->databaseOpened());
        emit dbOpened(); // отправить сигнал в главное окно
    }

} // openDatabase()

// слот устанавливает другую тему
void StartWidget::setTheme(QString name)
{
    QApplication::setStyle(QStyleFactory::create(name));
    settings->setValue("theme", name);

} // setTheme(QString name)

// слот устанавливает другой язык
void StartWidget::setLanguage(int index)
{
    settings->setValue("languageIndex", index);

    QMessageBox::information(this, tr("Settings"),
             tr("The application language will be changed after restart."));
} // setLanguage()

// установить элементы группы подключения к базе
void StartWidget::setDbBoxItems(bool p_connected)
{
    hostNameEdit->setEnabled(!p_connected);
    portEdit->setEnabled(!p_connected);
    databaseNameEdit->setEnabled(!p_connected);
    schemaEdit->setEnabled(!p_connected);
    userNameEdit->setEnabled(!p_connected);
    passwordEdit->setEnabled(!p_connected);
    connectButton->setEnabled(!p_connected);
} // setDbBoxItems()

// заполнить текстовые поля информацией от предыдущего сеанса,
// в котором было успешное соединение с базой данных,
// или значениями по умолчанию
void StartWidget::readDbConnectionInfo()
{
    hostNameEdit->setText(settings->value("hostName",
                                          QString("localhost")).toString());
    portEdit->setText(settings->value("port", QString("5432")).toString());
    databaseNameEdit->setText(settings->value("databaseName",
                                              QString("postgres")).toString());
    schemaEdit->setText(settings->value("schema",
                                        QString("contracts")).toString());
    userNameEdit->setText(settings->value("userName",
                                          QString("postgres")).toString());
    passwordEdit->setText(settings->value("password",
                                          QString("")).toString());
} // readDbConnectionInfo()

// сохранить параметры успешного соединения с базой данных
void StartWidget::writeDbConnectionInfo()
{
    if (dm->databaseOpened()) {
        settings->setValue("host", hostNameEdit->text());
        settings->setValue("port", portEdit->text());
        settings->setValue("database", databaseNameEdit->text());
        settings->setValue("schema", schemaEdit->text());
        settings->setValue("user", userNameEdit->text());
        settings->setValue("password", passwordEdit->text());
    }
} // writeDbConnectionInfo()
