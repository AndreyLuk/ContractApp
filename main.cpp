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

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(contract);

    QApplication app(argc, argv);

    const QString ORGANIZATION_NAME {"MyCompany"};
    const QString APPLICATION_NAME {"ContractApp"};
    QSettings *settings = new QSettings(ORGANIZATION_NAME, APPLICATION_NAME);
    int languageIndex = settings->value("languageIndex", 1).toInt();
    delete settings;

    QTranslator translator;
    // установить русский язык, если индекс = 1 (по умолчанию)
    // или английский, если индекс = 0
    if (languageIndex && translator.load(":contract_ru", "/"))
        app.installTranslator(&translator);

    MainWindow win;
    win.show();
    return app.exec();
}
