QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SheepBattleBoats
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp

include(Camera/Camera.pri)
include(GLWidget/GLWidget.pri)
include(Geometry/Geometry.pri)
include(SceneGraph/SceneGraph.pri)


OTHER_FILES += shaders/* \
    shaders/xtoon.vert \
    shaders/xtoon.frag \
    shaders/toonborder.vert \
    shaders/toonborder.frag \
    shaders/xtoon.vert \
    shaders/xtoon.frag \
    shaders/comicborder.vert \
    shaders/comicborder.frag

RESOURCES += SheepBattleBoats.qrc
