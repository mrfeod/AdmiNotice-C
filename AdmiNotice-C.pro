#-------------------------------------------------
#
# Project created by QtCreator 2016-05-25T21:46:09
#
#-------------------------------------------------

QT       += core gui network

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AdmiNotice-C
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MessageItem.cpp

HEADERS  += mainwindow.h \
    MessageItem.h

FORMS    += mainwindow.ui \
    MessageItem.ui

RESOURCES += \
    src.qrc
