#-------------------------------------------------
#
# Project created by QtCreator 2017-04-16T16:07:56
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Poker
TEMPLATE = app


SOURCES += main.cpp\
    login.cpp \
    roomlist.cpp \
    room.cpp \
    player.cpp

HEADERS  += \
    login.h \
    roomlist.h \
    room.h \
    player.h

FORMS    += \
    login.ui \
    roomlist.ui \
    room.ui

RESOURCES += \
    background.qrc \
    icon.qrc \
    allpoker.qrc

DISTFILES +=

CONFIG += c++11
