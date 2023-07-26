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

#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QtWidgets>
#include <QtSql>

#include "datamodule.h"

/*
 * виджет "начало"
 */
class StartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StartWidget(QSettings *p_settings,
                         DataModule *p_dm,
                         QWidget *p_parent = nullptr);
    virtual ~StartWidget();

signals:
    void dbOpened(); // после подключения к базе данных в главное окно
                     // приложения передается сигнал для установки "действий"

private slots:
    void openDatabase();
    void setTheme(QString name);
    void setLanguage(int index);

private:
    const int   WIDGET_WIDTH {600};
    const int   WIDGET_HEIGHT {700};

    void setDbBoxItems(bool p_connected);
    void readDbConnectionInfo();
    void writeDbConnectionInfo();

    QGroupBox   *mainStartBox {nullptr};
    QVBoxLayout *mainStartLayout {nullptr};

    QGroupBox   *dbBox {nullptr};
    QFormLayout *dbLayout {nullptr};
    QLabel      *hostNameLabel {nullptr};
    QLabel      *portLabel {nullptr};
    QLabel      *databaseNameLabel {nullptr};
    QLabel      *schemaLabel {nullptr};
    QLabel      *userNameLabel {nullptr};
    QLabel      *passwordLabel {nullptr};
    QLineEdit   *hostNameEdit {nullptr};
    QLineEdit   *portEdit {nullptr};
    QLineEdit   *databaseNameEdit {nullptr};
    QLineEdit   *schemaEdit {nullptr};
    QLineEdit   *userNameEdit {nullptr};
    QLineEdit   *passwordEdit {nullptr};
    QPushButton *connectButton {nullptr};

    QGroupBox   *optionBox {nullptr};
    QFormLayout *optionLayout {nullptr};
    QLabel      *themeLabel {nullptr};
    QComboBox   *themeCombo {nullptr};
    QLabel      *langLabel {nullptr};
    QComboBox   *languageCombo {nullptr};

    QSettings   *settings {nullptr};
    DataModule  *dm {nullptr};
};

#endif // STARTWIDGET_H
