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


OTHER_FILES += shaders/*

RESOURCES += SheepBattleBoats.qrc
