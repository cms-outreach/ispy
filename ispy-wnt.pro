#!qmake
#
# iSpy qmake project for WNT: qmake -tp vc ispy-wnt.pro
#

TEMPLATE = vcapp

CONFIG += debug_and_release


# Change any of these in order to match your externals.
ISPY_SDK_BASE = C:/Graphics/ispy-git
COIN3D_BASE = $$(COINDIR)
SOQT_BASE = $$(COINDIR)
ZLIB_BASE = C:/Graphics/zlib-1.2.5

win:DESTDIR = C:\Graphics\ispy-build

OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = ui

# Due to CMSSW requirements, we need to keep the
# qmake file in a subdirectory, therefore we need
# to find the sources a few levels up.
SOURCES += $$ISPY_SDK_BASE/src/Framework/*.cc
SOURCES += $$ISPY_SDK_BASE/src/QtGUI/*.cc
SOURCES += $$ISPY_SDK_BASE/src/QtGUI/bin/ispy.cpp
# SOURCES += $$ZLIB_BASE/contrib/minizip/unzip.c
HEADERS += $$ISPY_SDK_BASE/src/QtGUI/*.h
HEADERS += $$ISPY_SDK_BASE/src/Framework/*.h
RESOURCES = $$ISPY_SDK_BASE/src/QtGUI/resources/ispy.qrc
FORMS = $$ISPY_SDK_BASE/src/QtGUI/*.ui

INCLUDEPATH += $$ISPY_SDK_BASE
INCLUDEPATH += $$ISPY_SDK_BASE/src
INCLUDEPATH += $$COIN3D_BASE/include
INCLUDEPATH += $$SOQT_BASE/include
INCLUDEPATH += $$ZLIB_BASE/contrib/minizip
INCLUDEPATH += $$ZLIB_BASE/contrib
INCLUDEPATH += $$ZLIB_BASE


LIBS += -L$$ISPY_SDK_BASE/lib
LIBS += -L$$ISPY_SDK_BASE/lib64


CONFIG(debug, debug|release): #debug settings
{
   LIBS += -L$$ZLIB_BASE/contrib/vstudio/vc9/x86/ZlibStatDebug -lzlibstat
   LIBS += -L$$SOQT_BASE/lib -lSoQt1d
   LIBS += -L$$COIN3D_BASE/lib -lCoin3d
} else { # release settings
   LIBS += -L$$ZLIB_BASE/contrib/vstudio/vc9.x86/ZlibStatRelease -lzlibstat
   LIBS += -L$$SOQT_BASE/lib -lSoQt1
   LIBS += -L$$COIN3D_BASE/lib -lCoin3
}

QT += network opengl

#suppress warnings for C4305 (trunc from double to float) and C4996 (prefer sprintf_s to sprintf)
win32 { QMAKE_CXXFLAGS = -DCOIN_DLL -DSOQT_DLL -TP /wd4305 /wd4996 -DZLIB_WINAPI}

