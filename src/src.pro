#!qmake
#
# iSpy qmake project
# 

TEMPLATE = app
mac:TARGET = iSpy
unix:TARGET= ispy
win32:TARGET = iSpy

CONFIG += release

mac:QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
mac:CONFIG += x86 ppc

# Change any of these in order to match your externals.
ISPY_SDK_BASE = c:/Programs/Libs/iSpySdk
COIN3D_BASE = $$(COINDIR)
SOQT_BASE = $$(SOQTDIR)
!win32 {
  PCRE_BASE = $$ISPY_SDK_BASE
  CLASSLIB_BASE = $$ISPY_SDK_BASE
  FREETYPE_BASE = $$ISPY_SDK_BASE
  FONTCONFIG_BASE = $$ISPY_SDK_BASE
  EXPAT_BASE = $$ISPY_SDK_BASE
}

mac:DESTDIR = . 
unix:DESTDIR = .
win32:DESTDIR = .

OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = ui

# Due to CMSSW requirements, we need to keep the
# qmake file in a subdirectory, therefore we need 
# to find the sources a few levels up.
SOURCES += Framework/*.cc
SOURCES += Framework/*.c
SOURCES += QtGUI/*.cc
SOURCES += QtGUI/bin/ispy.cpp
HEADERS += QtGUI/*.h
HEADERS += Framework/*.h
RESOURCES = QtGUI/resources/ispy.qrc
FORMS = QtGUI/*.ui

INCLUDEPATH += $$ISPY_SDK_BASE/include
INCLUDEPATH += .
INCLUDEPATH += $$COIN3D_BASE/include
INCLUDEPATH += $$SOQT_BASE/include
INCLUDEPATH += $$SOQT_BASE/src
INCLUDEPATH += $$ISPY_SDK_BASE/include
!win32 {
  #INCLUDEPATH += $$CLASSLIB_BASE/include
  #INCLUDEPATH += $$FONTCONFIG_BASE/include
  #INCLUDEPATH += $$FREETYPE_BASE/include
  #INCLUDEPATH += $$EXPAT_BASE/include
}

LIBS += -L$$ISPY_SDK_BASE/lib -lzlibstat
LIBS += -L$$ISPY_SDK_BASE/lib64

!win32 {
  LIBS += -L$$SOQT_BASE/lib -lSoQt
  LIBS += -L$$COIN3D_BASE/lib -lCoin
  LIBS += -L$$PCRE_BASE/lib -lpcre
  LIBS += -L$$CLASSLIB_BASE/lib -lclasslib
  LIBS += -L$$FONTCONFIG_BASE/lib -lfontconfig
  LIBS += -L$$FREETYPE_BASE/lib -lfreetype
}

win32 {
  LIBS += -L$$COIN3D_BASE/lib -lcoin3s
  LIBS += -L$$SOQT_BASE/lib -lsoqt1s    
}


mac { 
LIBS += -L$$EXPAT_BASE/lib -lexpatstatic
}

win32:DEFINES += WIN32 COIN_NOT_DLL SOQT_NOT_DLL 

unix:!mac { LIBS += -L$$EXPAT_BASE/lib -lexpat }

!win32 {
  QMAKE_CXXFLAGS_RELEASE="-O0 -ggdb"
}

mac {
LIBS += -framework QuickTime
ICON = src/QtGUI/resources/ispy.icns
QMAKE_INFO_PLIST = src/QtGUI/resources/Info.plist
}

QT += network opengl

#QMAKE_CXXFLAGS_RELEASE = -O0 -ggdb

unix:!mac:!win32 {
# Statically link stdc++ and libgcc.
LIBS += -Wl,-dn -lstdc++ -Wl,-dy -static-libgcc
LIBS += -lXi
LIBS += -lfontconfig -lexpat -lfreetype
}

message("Now please type make")
