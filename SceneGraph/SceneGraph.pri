INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += SceneGraph/scene.h \
		   SceneGraph/scenegraph.h \
		   SceneGraph/seanode.h \
		   SceneGraph/lightnode.h \
           SceneGraph/levelobjectnode.h \
           SceneGraph/projectilenode.h

SOURCES += SceneGraph/scene.cpp \
		   SceneGraph/scenegraph.cpp \
		   SceneGraph/seanode.cpp \
		   SceneGraph/lightnode.cpp \
           SceneGraph/levelobjectnode.cpp \
           SceneGraph/projectilenode.cpp
