########################################################################
# This project is a Qt Widgets client/server application.
#
# It's a working demo, simplified, not a production level.
# Tested with Qt 5.11 on Astra Linux.
# etc/db_contracts_en.sql - a script for creating and populating
#     PostgreSQL database schema with english data.
# etc/db_contracts_ru.sql - a script for creating and populating
#     PostgreSQL database schema with russian data.
#
# Author: Andrey V. Lukashenko.
#
# Anyone can use parts of this program without any warranty
# and without a reference to author.
########################################################################

TARGET = contract
TEMPLATE = app

QT += widgets sql printsupport help

CONFIG += c++11

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    datamodule.cpp \
    startwidget.cpp \
    contractwidget.cpp \
    contracttypewidget.cpp \
    contractstatewidget.cpp \
    partnerwidget.cpp \
    contractdelegate.cpp \
    reportwidget.cpp

HEADERS  += \
    mainwindow.h \
    datamodule.h \
    startwidget.h \
    contractwidget.h \
    contracttypewidget.h \
    contractstatewidget.h \
    partnerwidget.h \
    contractdelegate.h \
    reportwidget.h

FORMS += \
    partnerwidget.ui

TRANSLATIONS += contract_ru.ts

RESOURCES += contract.qrc

DISTFILES += \
    database/db_contracts_en.sql \
    database/db_contracts_ru.sql \
    help/contract.qch \
    help/contract.qhc \
    help/contract.qhcp \
    help/contract.qhp \
    help/contracts.html \
    help/main.html \
    help/make-help.txt \
    help/partners.html \
    help/report.html \
    help/script.html \
    help/start.html \
    help/states.html \
    help/types.html
