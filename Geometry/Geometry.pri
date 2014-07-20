INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS  += Geometry/primitive.h \
			Geometry/cube.h \
		 	Geometry/cone.h \
			Geometry/cylinder.h \
			Geometry/sphere.h \
			Geometry/torus.h \
			Geometry/light.h \
			Geometry/object3d.h \
            Geometry/texture.h \
			Geometry/sea.h \
            Geometry/levelobject.h \
            Geometry/mothersheep.h \
            Geometry/projectile.h

SOURCES  += Geometry/cube.cpp \
		 	Geometry/cone.cpp \
			Geometry/cylinder.cpp \
			Geometry/sphere.cpp \
			Geometry/torus.cpp \
			Geometry/light.cpp \
            Geometry/object3d.cpp \
            Geometry/texture.cpp \
			Geometry/sea.cpp \
            Geometry/levelobject.cpp \
            Geometry/mothersheep.cpp \
            Geometry/projectile.cpp
