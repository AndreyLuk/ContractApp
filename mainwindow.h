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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QHelpEngine>

#include "datamodule.h"
#include "startwidget.h"
#include "contractwidget.h"
#include "reportwidget.h"
#include "contracttypewidget.h"
#include "contractstatewidget.h"
#include "partnerwidget.h"

/*
 * Главное окно приложения
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    void keyPressEvent(QKeyEvent *event) override;

    const DataModule* dataModule();
    const QSettings*  getSettings();

private slots:
    void createStartWidget();
    void createContractWidget();
    void createReportWidget();

    void createContractTypeReference();
    void createContractStateReference();
    void createPartnerReference();

    void showHelpDialog();
    void showAboutDialog();
    void showAboutQtDialog();
    void setActions();

    void helpUrlChanged(const QUrl &link);

private:
    const QString ORGANIZATION_NAME {"MyCompany"};
    const QString APPLICATION_NAME {"ContractApp"};
    const int MAINWINDOW_WIDTH {1800};
    const int MAINWINDOW_HEIGHT {900};

    void writeSettings();
    void createActions();
    void createMenus();
    void showStatusBarMessage(const QString& pStr);
    void deleteAllWidgets();
    void showHelp(const QString keyword = "main");

    QMenu               *activityMenu {nullptr};
    QAction             *startAction {nullptr};
    QAction             *activitySeparator1 {nullptr};
    QAction             *contractAction {nullptr};
    QAction             *reportAction {nullptr};
    QAction             *activitySeparator2 {nullptr};
    QAction             *exitAction {nullptr};

    QMenu               *referenceMenu {nullptr};
    QAction             *contractTypeAction {nullptr};
    QAction             *contractStateAction {nullptr};
    QAction             *partnerAction {nullptr};

    QMenu               *helpMenu {nullptr};
    QAction             *helpAction {nullptr};
    QAction             *aboutAction {nullptr};
    QAction             *aboutQtAction {nullptr};

    StartWidget         *startWidget {nullptr};
    ContractWidget      *contractWidget {nullptr};
    ReportWidget         *reportWidget {nullptr};
    ContractTypeWidget  *contractTypeWidget {nullptr};
    ContractStateWidget *contractStateWidget {nullptr};
    PartnerWidget       *partnerWidget {nullptr};

    QSettings           *settings {nullptr};
    DataModule          *dm {nullptr};

    QDialog             *helpDialog {nullptr};
    QHelpEngineCore     *helpEngine {nullptr};
    QTextBrowser        *helpBrowser {nullptr};
};

#endif // MAINWINDOW_H
