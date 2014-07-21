INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += SceneGraph/scene.h \
		   SceneGraph/scenegraph.h \
		   SceneGraph/seanode.h \
		   SceneGraph/lightnode.h \
           SceneGraph/levelobjectnode.h \
           SceneGraph/projectilenode.h \
    SceneGraph/particleexplosionnode.h \
    SceneGraph/particleexplosionnode2.h \
    SceneGraph/targetnode.h

SOURCES += SceneGraph/scene.cpp \
		   SceneGraph/scenegraph.cpp \
		   SceneGraph/seanode.cpp \
		   SceneGraph/lightnode.cpp \
           SceneGraph/levelobjectnode.cpp \
           SceneGraph/projectilenode.cpp \
    SceneGraph/particleexplosionnode.cpp \
    SceneGraph/particleexplosionnode2.cpp \
    SceneGraph/targetnode.cpp
