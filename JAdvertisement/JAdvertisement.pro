#-------------------------------------------------
#
# Project created by QtCreator 2018-12-19T13:37:53
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(debug, debug|release) {
    DESTDIR += $$PWD/../bin_debug
} else {
    DESTDIR += $$PWD/../bin
}

TARGET = JAdvertisement
TEMPLATE = app


SOURCES += main.cpp\
        JAdWidget.cpp

HEADERS  += JAdWidget.h

FORMS    += JAdWidget.ui

RESOURCES += \
    res.qrc
