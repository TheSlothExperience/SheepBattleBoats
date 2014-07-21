QT+= core gui opengl


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SheepBattleBoats
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 \
                  -Wno-deprecated-declarations

LIBS+= -L/usr/local/lib -lassimp

SOURCES += main.cpp

include(Camera/Camera.pri)
include(SceneGraph/SceneGraph.pri)
include(GLWidget/GLWidget.pri)
include(Geometry/Geometry.pri)
include(Util/Util.pri)
include(GameLogic/GameLogic.pri)


OTHER_FILES += shaders/* \
    shaders/xtoon.vert \
    shaders/xtoon.frag \
    shaders/toonborder.vert \
    shaders/toonborder.frag \
    shaders/xtoon.vert \
    shaders/xtoon.frag \
    shaders/intensity.frag

RESOURCES += SheepBattleBoats.qrc
