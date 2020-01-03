#-------------------------------------------------
#
# Project created by QtCreator 2020-01-03T13:09:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LibkiPrintManager
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    networkclient.cpp \
    systemtray.cpp

HEADERS  += mainwindow.h \
    networkclient.h \
    systemtray.h

FORMS    += mainwindow.ui

RESOURCES += libki.qrc
