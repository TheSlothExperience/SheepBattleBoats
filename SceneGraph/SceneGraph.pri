INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += SceneGraph/scene.h \
		   SceneGraph/scenegraph.h \
		   SceneGraph/seanode.h \
           SceneGraph/projectilenode.h \
           SceneGraph/sheepnode.h \
           SceneGraph/projectilenode.h \
		   SceneGraph/particleexplosionnode.h \
		   SceneGraph/particleexplosionnode2.h \
		   SceneGraph/targetnode.h

SOURCES += SceneGraph/scene.cpp \
		   SceneGraph/scenegraph.cpp \
		   SceneGraph/seanode.cpp \
           SceneGraph/sheepnode.cpp \
		   SceneGraph/particleexplosionnode.cpp \
		   SceneGraph/particleexplosionnode2.cpp \
		   SceneGraph/targetnode.cpp \
           SceneGraph/projectilenode.cpp
