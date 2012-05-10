#!qmake
#
# iSpy qmake project
# 

TEMPLATE = app
mac:TARGET = iSpy
unix:TARGET= ispy

CONFIG += release

mac:QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
mac:CONFIG += 

# Change any of these in order to match your externals.
ISPY_SDK_BASE = $$QT_INSTALL_PREFIX
COIN3D_BASE = $$ISPY_SDK_BASE
SOQT_BASE = $$ISPY_SDK_BASE
PCRE_BASE = $$ISPY_SDK_BASE
ZLIB_BASE = $$ISPY_SDK_BASE
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
SOURCES += src/Framework/*.cc
SOURCES += src/Framework/*.c
SOURCES += src/QtGUI/*.cc
SOURCES += src/QtGUI/bin/ispy.cpp
HEADERS += src/QtGUI/*.h
HEADERS += src/Framework/*.h
RESOURCES = src/QtGUI/resources/ispy.qrc
FORMS = src/QtGUI/*.ui

INCLUDEPATH += $$ISPY_SDK_BASE/include
INCLUDEPATH += src
INCLUDEPATH += $$COIN3D_BASE/include
INCLUDEPATH += $$SOQT_BASE/include
INCLUDEPATH += $$FONTCONFIG_BASE/include
INCLUDEPATH += $$FREETYPE_BASE/include
INCLUDEPATH += $$EXPAT_BASE/include
INCLUDEPATH += $$ISPY_SDK_BASE/include
LIBS += -L$$ISPY_SDK_BASE/lib 
LIBS += -L$$ISPY_SDK_BASE/lib64
LIBS += -L$$SOQT_BASE/lib -lSoQt
LIBS += -L$$COIN3D_BASE/lib -lCoin
LIBS += -L$$PCRE_BASE/lib -lpcre
LIBS += -L$$ZLIB_BASE/lib -lz
LIBS += -L$$FONTCONFIG_BASE/lib -lfontconfig
LIBS += -L$$FREETYPE_BASE/lib -lfreetype
LIBS += -L$$ISPY_SDK_BASE/lib -lsimage
mac { 
LIBS += -L$$EXPAT_BASE/lib -lexpat-static
}

unix:!mac { LIBS += -L$$EXPAT_BASE/lib -lexpat }

#QMAKE_CXXFLAGS_RELEASE="-O0 -ggdb"

mac {
LIBS += -framework QuickTime
ICON = src/QtGUI/resources/ispy.icns
QMAKE_INFO_PLIST = src/QtGUI/resources/Info.plist
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
