#-------------------------------------------------
#
# Project created by QtCreator 2019-12-12T10:40:41
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestSofManager
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    login.cpp

HEADERS  += widget.h \
    login.h

FORMS    += widget.ui \
    login.ui

CONFIG   +=C++11

RESOURCES += \
    res.qrc
#DEFINES += QT_NO_DEBUG_OUTPUT
