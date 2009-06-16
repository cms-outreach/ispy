TEMPLATE = app
TARGET = iSpy

ISPY_SDK = /Users/ktf/.nix-profile
COIN3D_BASE=$$system(scram tool info coin | grep COIN3D_BASE | cut -f2 -d=)
SOQT_BASE=$$system(scram tool info soqt | grep SOQT_BASE | cut -f2 -d=)
PCRE_BASE=$$system(scram tool info pcre | grep PCRE_BASE | cut -f2 -d=)
UUID_BASE=$$system(scram tool info uuid | grep UUID_BASE | cut -f2 -d=)
CLASSLIB_BASE=$$system(scram tool info classlib | grep CLASSLIB_BASE | cut -f2 -d=)


DESTDIR = /Application
OBJECTS_DIR = tmp
MOC_DIR = tmp

SOURCES += Iguana/QtGui/src/*.cc
SOURCES += Iguana/QtGui/src/*.c
SOURCES += Iguana/Framework/src/*.cc
SOURCES += Iguana/Inventor/src/*.cc
SOURCES += Iguana/QtGui/bin/ispy.cpp
HEADERS += Iguana/QtGui/interface/*.h
HEADERS += Iguana/Framework/interface/*.h
INCLUDEPATH += $$ISPY_SDK/include
INCLUDEPATH += Iguana/Framework/interface
INCLUDEPATH += $$COIN3D_BASE/include
INCLUDEPATH += $$SOQT_BASE/include
INCLUDEPATH += $$CLASSLIB_BASE/include
LIBS += -L$$ISPY_SDK/lib -L$$COIN3D_BASE/lib -L$$SOQT_BASE/lib -lclasslib  -lcoin -lSoQt
LIBS += -L$$PCRE_BASE/lib -lpcre
LIBS += -L$$UUID_BASE/lib -luuid
LIBS += -L$$CLASSLIB_BASE/lib -lclasslib
message ($$INCLUDEPATH)
QT += network opengl
