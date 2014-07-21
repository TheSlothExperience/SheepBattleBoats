INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS  += Geometry/primitive.h \
			Geometry/cube.h \
		 	Geometry/cone.h \
			Geometry/cylinder.h \
			Geometry/sphere.h \
			Geometry/torus.h \
			Geometry/light.h \
			Geometry/sea.h \
			Geometry/object3d.h \
            Geometry/levelobject.h \
            Geometry/mothersheep.h \
            Geometry/projectile.h \
    Geometry/particlegenerator.h

SOURCES  += Geometry/cube.cpp \
		 	Geometry/cone.cpp \
			Geometry/cylinder.cpp \
			Geometry/sphere.cpp \
			Geometry/torus.cpp \
			Geometry/light.cpp \
			Geometry/sea.cpp \
            Geometry/object3d.cpp \
            Geometry/levelobject.cpp \
            Geometry/mothersheep.cpp \
            Geometry/projectile.cpp \
    Geometry/particlegenerator.cpp
