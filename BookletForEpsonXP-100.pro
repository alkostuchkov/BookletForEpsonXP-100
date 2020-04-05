#-------------------------------------------------
#
# Project created by QtCreator 2016-02-07T14:59:14
#
#-------------------------------------------------

QT       += core gui widgets

VERSION = 1.0.0.1

QMAKE_TARGET_COMPANY = Home Production
QMAKE_TARGET_PRODUCT = Calculating pages for printing booklet for Epson XP-100
QMAKE_TARGET_DESCRIPTION = Booklet for Epson XP-100
QMAKE_TARGET_COPYRIGHT = Alexander Kostuchkov

TARGET = BookletForEpsonXP-100
TEMPLATE = app


SOURCES += main.cpp\
        bookletforepsonxp100.cpp

HEADERS  += bookletforepsonxp100.h

FORMS    += bookletforepsonxp100.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS += booklet_ru.ts \
                booklet_en.ts

CODECFORTR = UTF-8

RC_ICONS = ./images/booklet_1.ico

OTHER_FILES +=

DISTFILES += \
    booklet_ru.ts \
    booklet_en.ts
