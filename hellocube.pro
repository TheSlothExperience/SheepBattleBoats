#-------------------------------------------------
#
# Project created by QtCreator 2014-04-24T19:46:40
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hellocube
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

LIBS += -lglut -lGLU

SOURCES += main.cpp\
        mainwindow.cpp\
        glwidget.cpp\
        scene.cpp\
        scenegraph.cpp\
        cube.cpp

HEADERS  += mainwindow.h\
        glwidget.h\
        scene.h\
        scenegraph.h\
        cube.h

RESOURCES += \
    hellocube.qrc
