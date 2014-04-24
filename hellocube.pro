#-------------------------------------------------
#
# Project created by QtCreator 2014-04-24T19:46:40
#
#-------------------------------------------------

QT       += core gui opengl	

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hellocube
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        glwidget.cpp

HEADERS  += mainwindow.h\
        glwidget.h

RESOURCES += \
    hellocube.qrc
