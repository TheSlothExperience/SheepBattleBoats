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
    Geometry/levelobject.h \
    Geometry/boundingbox.h \
    Geometry/collisiondetecion.h \
    Geometry/projectile.h \
    Geometry/mothersheep.h

SOURCES  += Geometry/cube.cpp \
		 	Geometry/cone.cpp \
			Geometry/cylinder.cpp \
			Geometry/sphere.cpp \
			Geometry/torus.cpp \
			Geometry/light.cpp \
    Geometry/object3d.cpp \
    Geometry/levelobject.cpp \
    Geometry/boundingbox.cpp \
    Geometry/collisiondetecion.cpp \
    Geometry/projectile.cpp \
    Geometry/mothersheep.cpp
