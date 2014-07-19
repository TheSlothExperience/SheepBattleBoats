INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
LIBS += -lOpenCL

HEADERS += Util/CL/Common.h \
		   Util/CL/CTimer.h \
		   Util/CL/Reduction.h

SOURCES += Util/CL/Common.cpp \
		   Util/CL/CTimer.cpp \
		   Util/CL/Reduction.cpp
