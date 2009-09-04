#!qmake
#
# iSpy qmake project
# 

TEMPLATE = app
mac:TARGET = iSpy
linux:target = ispy

# Change any of these in order to match your externals.
ISPY_SDK_BASE = /Users/eulisse/tmp/sw
COIN3D_BASE=$$ISPY_SDK_BASE
SOQT_BASE=$$ISPY_SDK_BASE
PCRE_BASE=$$ISPY_SDK_BASE
UUID_BASE=$$ISPY_SDK_BASE
CLASSLIB_BASE=$$ISPY_SDK_BASE

mac:DESTDIR = /Applications
linux:DESTDIR = .

OBJECTS_DIR = tmp
MOC_DIR = tmp

# Due to CMSSW requirements, we need to keep the
# qmake file in a subdirectory, therefore we need 
# to find the sources a few levels up.
SOURCES += Iguana/QtGUI/src/*.cc
SOURCES += Iguana/Framework/src/*.cc
SOURCES += Iguana/Inventor/src/*.cc
SOURCES += Iguana/QtGUI/bin/ispy.cpp
HEADERS += Iguana/QtGUI/interface/*.h
HEADERS += Iguana/Framework/interface/*.h
INCLUDEPATH += $$ISPY_SDK/include
INCLUDEPATH += Iguana/Framework/interface
INCLUDEPATH += $$COIN3D_BASE/include
INCLUDEPATH += $$SOQT_BASE/include
INCLUDEPATH += $$CLASSLIB_BASE/include
LIBS += -L$$ISPY_SDK/lib 
LIBS += -L$$SOQT_BASE/lib -lSoQt
LIBS += -L$$COIN3D_BASE/lib -lcoin
LIBS += -L$$PCRE_BASE/lib -lpcre
LIBS += -L$$UUID_BASE/lib -luuid
LIBS += -L$$CLASSLIB_BASE/lib -lclasslib
QT += network opengl

linux {
  message("Now please type make")
}
