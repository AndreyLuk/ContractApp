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

#include "mainwindow.h"

// конструктор главного окна приложения
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedWidth(MAINWINDOW_WIDTH);
    setFixedHeight(MAINWINDOW_HEIGHT);

    settings = new QSettings(ORGANIZATION_NAME, APPLICATION_NAME);
    move(settings->value("pos", QPoint(50, 50)).toPoint());
    QApplication::setStyle(QStyleFactory::create(
                           settings->value("theme", "Windows").toString()));

    QApplication::setPalette(QPalette(QColor(20, 60, 100)));

    setWindowTitle(tr("Contract Application"));

    createActions();
    createMenus();

    dm = new DataModule(this);  // создать модуль данных

    showStatusBarMessage(tr("Ready"));

    createStartWidget();      // создать виджет "начало"

} // MainWindow::MMainWindow()

// деструктор главного окна приложения
MainWindow::~MainWindow()
{
    deleteAllWidgets();
    writeSettings();

    if (settings != nullptr) {
        delete settings;
    }
    if (dm != nullptr) {
        delete dm;
    }
} // ~MainWindow()

// создать действия
void MainWindow::createActions()
{
    startAction = new QAction(tr("&Start"), this);
    startAction->setStatusTip(tr("At the begining"));
    connect(startAction, &QAction::triggered,
            this, &MainWindow::createStartWidget);

    contractAction = new QAction(tr("&Contracts"), this);
    contractAction->setStatusTip(tr("Operations with contracts"));
    connect(contractAction, &QAction::triggered,
            this, &MainWindow::createContractWidget);

    reportAction = new QAction(tr("&Report"), this);
    reportAction->setStatusTip(tr("Contract reports"));
    connect(reportAction, &QAction::triggered,
            this, &MainWindow::createReportWidget);

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setStatusTip(tr("Exit from application"));
    connect(exitAction, &QAction::triggered,
            this, &MainWindow::close);

    contractTypeAction = new QAction(tr("Contract &types"), this);
    contractTypeAction->setStatusTip(tr("Contract types"));
    connect(contractTypeAction, &QAction::triggered,
            this, &MainWindow::createContractTypeReference);

    contractStateAction = new QAction(tr("C&ontract states"), this);
    contractStateAction->setStatusTip(tr("Contract states"));
    connect(contractStateAction, &QAction::triggered,
            this, &MainWindow::createContractStateReference);

    partnerAction = new QAction(tr("&Partners"), this);
    partnerAction->setStatusTip(tr("Partners"));
    connect(partnerAction, &QAction::triggered,
            this, &MainWindow::createPartnerReference);

    helpAction = new QAction(tr("&Content"), this);
    helpAction->setStatusTip(tr("Help content"));
    connect(helpAction, &QAction::triggered,
            this, &MainWindow::showHelpDialog);

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setStatusTip(tr("About program"));
    connect(aboutAction, &QAction::triggered,
            this, &MainWindow::showAboutDialog);

    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setStatusTip("About Qt");
    connect(aboutQtAction, &QAction::triggered,
            this, &MainWindow::showAboutQtDialog);
} // createActions()

// создать меню
void MainWindow::createMenus()
{
    activityMenu = menuBar()->addMenu(tr("&Actions"));
    activityMenu->addAction(startAction);
    activitySeparator1 = activityMenu->addSeparator();
    activityMenu->addAction(contractAction);
    activityMenu->addAction(reportAction);
    activitySeparator2 = activityMenu->addSeparator();
    activityMenu->addAction(exitAction);

    referenceMenu = menuBar()->addMenu(tr("&References"));
    referenceMenu->addAction(contractTypeAction);
    referenceMenu->addAction(contractStateAction);
    referenceMenu->addAction(partnerAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAction);
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
} // createMenus()

// показать сообщение в строке состояния
void MainWindow::showStatusBarMessage(const QString& pStr)
{
    statusBar()->showMessage(pStr);
} // setStatusBarMessage()

// слот устанавливает действия доступными или недоступными
// в зависимости от подключения к базе данных
void MainWindow::setActions()
{
    // "начало" устанавливается по особому алгоритму
    startAction->setEnabled(startWidget == nullptr);

    if (startWidget != nullptr) {
        dm->databaseOpened() ?
                    showStatusBarMessage(tr("Database is connected")) :
                    showStatusBarMessage(tr("Database is not connected"));
    }

    bool dbOpened = dm->databaseOpened();

    contractAction->setEnabled(dbOpened);
    reportAction->setEnabled(dbOpened);
    contractTypeAction->setEnabled(dbOpened);
    contractStateAction->setEnabled(dbOpened);
    partnerAction->setEnabled(dbOpened);
} // setMenu()

// удалить все виджеты в главном окне (если есть) и обновить меню
void MainWindow::deleteAllWidgets()
{
    if (startWidget != nullptr) {
        delete startWidget;
        startWidget = nullptr;
    }
    if (contractWidget != nullptr) {
        delete contractWidget;
        contractWidget = nullptr;
    }
    if (reportWidget != nullptr) {
        delete reportWidget;
        reportWidget = nullptr;
    }
    if (contractTypeWidget != nullptr) {
        delete contractTypeWidget;
        contractTypeWidget = nullptr;
    }
    if (contractStateWidget != nullptr) {
        delete contractStateWidget;
        contractStateWidget = nullptr;
    }
    setActions();
} // deleteAllWidgets()

// получить указатель на Установки/Settings для передачи main()
const QSettings *MainWindow::getSettings()
{
    return settings;
} // getSettings()

// слот создает и отображает виджет "Начало"
void MainWindow::createStartWidget()
{
    deleteAllWidgets();
    startWidget = new StartWidget(settings, dm, this);
    setCentralWidget(startWidget);
    setWindowTitle(tr("Contract Application"));
    startAction->setDisabled(true);

    dm->databaseOpened() ? showStatusBarMessage(tr("Database is connected")) :
                  showStatusBarMessage(tr("Database is not connected"));

    connect(startWidget, &StartWidget::dbOpened,
            this, &MainWindow::setActions);
} // createStartWidget()

// слот создает и отображает виджет "Договоры"
void MainWindow::createContractWidget()
{
    deleteAllWidgets();
    contractWidget = new ContractWidget(settings, dm, this);
    setCentralWidget(contractWidget);
    setWindowTitle(tr("Contract Application - Contracts"));
    contractAction->setDisabled(true);
} // createContractWidget()

// слот создает и отображает виджет "Печать"
void MainWindow::createReportWidget()
{
    deleteAllWidgets();
    reportWidget = new ReportWidget(settings, dm, this);
    setCentralWidget(reportWidget);
    setWindowTitle(tr("Contract Application - Contract report"));
    reportAction->setDisabled(true);
} // createPrintWidget()

// слот создает и отображает справочник типов договоров
void MainWindow::createContractTypeReference()
{
    deleteAllWidgets();
    contractTypeWidget = new ContractTypeWidget(settings, dm, this);
    setCentralWidget(contractTypeWidget);
    setWindowTitle(tr("Contract Application - Contract Type reference"));
    contractTypeAction->setDisabled(true);
} // createContractTypeReference()

// слот создает и отображает справочник состояний договоров
void MainWindow::createContractStateReference()
{
    deleteAllWidgets();
    contractStateWidget = new ContractStateWidget(settings, dm, this);
    setCentralWidget(contractStateWidget);
    setWindowTitle(tr("Contract Application - Contract State reference"));
    contractStateAction->setDisabled(true);
} // createContractStateReference()

// слот создает и отображает справочник контрагентов
void MainWindow::createPartnerReference()
{
    deleteAllWidgets();
    partnerWidget = new PartnerWidget(settings, dm, this);
    setCentralWidget(partnerWidget);
    setWindowTitle(tr("Contract Application - Partner reference"));
    partnerAction->setDisabled(true);
} // createPartnerReference()

// слот отображает диалоговое окно "о программе"
void MainWindow::showAboutDialog()
{
    QMessageBox::about(this, tr("About Application"),
             tr("The \"Contract\" application demonstrates how to "
                "write GUI / SQL applications using Qt Widgets.<br><br>"
                "Author: Andrey V. Lukashenko.<br>"));
} // showAboutDialog()

// слот отображает диалоговое окно "о Qt"
void MainWindow::showAboutQtDialog()
{
    qApp->aboutQt();

} // showAboutQtDialog()

// слот отображает главное окно подсказки
void MainWindow::showHelpDialog()
{
    showHelp();

} // showHelpDialog()

// показать подсказку
void MainWindow::showHelp(const QString keyword)
{
    if (helpDialog == nullptr) {

        helpDialog = new QDialog(this);
        helpDialog->setWindowTitle(tr("Help"));
        helpBrowser = new QTextBrowser(helpDialog);

        if (helpEngine == nullptr) {
            helpEngine = new QHelpEngineCore(
                        "/home/andrey/projects/contract/help/contract.qhc");
        }

        connect(helpBrowser, &QTextBrowser::anchorClicked,
                this, &MainWindow::helpUrlChanged);

        // получить все ссылки для главного окна подсказки
        QMap<QString, QUrl> links =
            helpEngine->linksForIdentifier(QLatin1String(keyword.toLatin1()));

        // Если есть подсказка для указанного ключа ("main")
        // получить данные для первой ссылки
        if (links.count()) {
            QByteArray helpData =
                    helpEngine->fileData(links.constBegin().value());
            // показать справку
            if (!helpData.isEmpty()) {
                QString helpString(helpData);
                helpBrowser->setHtml(helpString);
                helpBrowser->resize(800, 600);
                helpDialog->exec();
            }
        }
        if (helpEngine != nullptr) {
            delete helpEngine;
            helpEngine = nullptr;
        }
        delete helpDialog;
        helpDialog = nullptr;
    }
} // showHelp()

// слот смены страницы справки
void MainWindow::helpUrlChanged(const QUrl &link)
{
    QString strIdentifier = QString(link.url());
    strIdentifier.truncate(strIdentifier.indexOf('.'));

    QMap<QString, QUrl> links =
        helpEngine->linksForIdentifier(strIdentifier);
    if (links.count()) {
        QByteArray helpData = helpEngine->fileData(links.constBegin().value());
        // показать справку
        if (!helpData.isEmpty()) {
            QString helpString(helpData);
            helpBrowser->setHtml(helpString);
        }
    }
} // helpUrlChanged()

// обработка событий нажатия клавиш клавиатуры
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_F1:
        // при нажатии клавиши F1 показать контекстную справку
        if (startWidget != nullptr) {
            showHelp("start");
        } else if (contractWidget != nullptr) {
            showHelp("contracts");
        } else if (reportWidget != nullptr) {
            showHelp("report");
        } else if (contractTypeWidget != nullptr) {
            showHelp("types");
        } else if (contractStateWidget != nullptr) {
            showHelp("states");
        } else if (partnerWidget != nullptr) {
            showHelp("partners");
        } else {
            showHelp("main");
        }
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }
} // keyPressEvent()

// получить указатель на модуль данных
const DataModule* MainWindow::dataModule()
{
    return dm;
}   // dataModule()

// сохранить установки
void MainWindow::writeSettings()
{
    settings->setValue("pos", pos());
} // writeSettings()
