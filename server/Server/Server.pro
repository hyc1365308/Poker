#-------------------------------------------------
#
# Project created by QtCreator 2017-04-23T18:54:21
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app


SOURCES += main.cpp\
        server.cpp \
    sqloperation.cpp \
    room.cpp

HEADERS  += server.h \
    sqloperation.h \
    room.h

FORMS    += server.ui
