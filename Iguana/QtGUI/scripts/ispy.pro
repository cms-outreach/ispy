#!qmake
#
# iSpy qmake project
# 

TEMPLATE = app
mac:TARGET = iSpy
unix:TARGET= ispy

CONFIG += release

mac:QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
mac:CONFIG += x86 ppc

# Change any of these in order to match your externals.
ISPY_SDK_BASE = $$QT_INSTALL_PREFIX
COIN3D_BASE = $$ISPY_SDK_BASE
SOQT_BASE = $$ISPY_SDK_BASE
PCRE_BASE = $$ISPY_SDK_BASE
CLASSLIB_BASE = $$ISPY_SDK_BASE
FREETYPE_BASE = $$ISPY_SDK_BASE
FONTCONFIG_BASE = $$ISPY_SDK_BASE
EXPAT_BASE = $$ISPY_SDK_BASE

mac:DESTDIR = . 
unix:DESTDIR = .

OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = ui

# Due to CMSSW requirements, we need to keep the
# qmake file in a subdirectory, therefore we need 
# to find the sources a few levels up.
SOURCES += Iguana/QtGUI/src/*.cc
SOURCES += Iguana/Framework/src/*.cc
SOURCES += Iguana/Inventor/src/*.cc
SOURCES += Iguana/QtGUI/bin/ispy.cpp
HEADERS += Iguana/QtGUI/interface/*.h
HEADERS += Iguana/QtGUI/src/*.h
HEADERS += Iguana/Framework/interface/*.h
RESOURCES = Iguana/QtGUI/interface/ispy.qrc
FORMS = Iguana/QtGUI/src/*.ui

INCLUDEPATH += $$ISPY_SDK_BASE/include
INCLUDEPATH += Iguana/Framework/interface
INCLUDEPATH += $$COIN3D_BASE/include
INCLUDEPATH += $$SOQT_BASE/include
INCLUDEPATH += $$CLASSLIB_BASE/include
INCLUDEPATH += $$FONTCONFIG_BASE/include
INCLUDEPATH += $$FREETYPE_BASE/include
INCLUDEPATH += $$EXPAT_BASE/include
INCLUDEPATH += $$ISPY_SDK_BASE/include
LIBS += -L$$ISPY_SDK_BASE/lib 
LIBS += -L$$ISPY_SDK_BASE/lib64
LIBS += -L$$SOQT_BASE/lib -lSoQt
LIBS += -L$$COIN3D_BASE/lib -lCoin
LIBS += -L$$PCRE_BASE/lib -lpcre
LIBS += -L$$CLASSLIB_BASE/lib -lclasslib
LIBS += -L$$FONTCONFIG_BASE/lib -lfontconfig
LIBS += -L$$FREETYPE_BASE/lib -lfreetype
mac { 
LIBS += -L$$EXPAT_BASE/lib -lexpatstatic
}

unix:!mac { LIBS += -L$$EXPAT_BASE/lib -lexpat }

mac {
LIBS += -framework QuickTime
ICON = Iguana/QtGUI/src/ispy.icns
QMAKE_INFO_PLIST = Iguana/QtGUI/src/Info.plist
}

QT += network opengl

#QMAKE_CXXFLAGS_RELEASE = -O0 -ggdb

unix:!mac {
# Statically link stdc++ and libgcc.
LIBS += -Wl,-dn -lstdc++ -Wl,-dy -static-libgcc
LIBS += -lXi
LIBS += -lfontconfig -lexpat -lfreetype
}

message("Now please type make")
