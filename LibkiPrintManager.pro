#-------------------------------------------------
#
# Project created by QtCreator 2020-01-03T13:09:02
#
#-------------------------------------------------

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LibkiPrintManager
TEMPLATE = app

SOURCES += main.cpp\
        networkclient.cpp \
        systemtray.cpp 

HEADERS  += \
         networkclient.h \
         systemtray.h 

FORMS    += mainwindow.ui

RESOURCES += libki.qrc

RC_FILE += libki_print_manager.rc
