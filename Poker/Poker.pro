#-------------------------------------------------
#
# Project created by QtCreator 2017-04-16T16:07:56
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Poker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    roomlist.cpp

HEADERS  += mainwindow.h \
    login.h \
    roomlist.h

FORMS    += mainwindow.ui \
    login.ui \
    roomlist.ui

RESOURCES += \
    background.qrc \
    icon.qrc \
    allpoker.qrc

DISTFILES +=
