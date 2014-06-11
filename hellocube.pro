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


SOURCES += main.cpp\
        mainwindow.cpp\
        tfEditor.cpp \
        tfDisplay.cpp \
        glwidget.cpp\
        glwidgetcontext.cpp\
        scene.cpp\
        scenegraph.cpp\
        lightnode.cpp \
        volumenode.cpp \
        perspectivecamera.cpp\
        orthocamera.cpp\
        cube.cpp \
        cone.cpp \
        sphere.cpp \
        torus.cpp \
        cylinder.cpp \
        volume.cpp \
        light.cpp

HEADERS  += mainwindow.h\
        glwidget.h\
        glwidgetcontext.h\
        tfEditor.h \
        tfDisplay.h \
        scene.h\
        scenegraph.h\
        lightnode.h\
        volumenode.h\
        camera.h\
        perspectivecamera.h\
        orthocamera.h\
        primitive.h\
        cube.h \
        cone.h \
        sphere.h \
        torus.h \
        cylinder.h\
        volume.h\
        light.h	

RESOURCES += \
    hellocube.qrc
