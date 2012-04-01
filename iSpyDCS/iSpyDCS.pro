TEMPLATE        = lib
TARGET          = iSpyDCS
CONFIG += qt \
    dll \
    opengl \
    warn_off \
    release \
    thread

ISPY_SDK_BASE = c:/Programs/Libs/iSpySdk
OBJECTS_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
UI_DIR = ui

INCLUDEPATH    += ../src
INCLUDEPATH    += $$(PVSS_II_HOME)/api/include/EWO
INCLUDEPATH    += $$(COINDIR)/include
INCLUDEPATH    += $$(SOQTDIR)/src
INCLUDEPATH    += $$ISPY_SDK_BASE/include

win32:DEFINES += WIN32 \
    COIN_NOT_DLL \
    SOQT_NOT_DLL

LIBS += -L$$ISPY_SDK_BASE/lib -lzlibstat

win32 {
  LIBS += -L$$(PVSS_II_HOME)/api/lib.winnt -lewo
  LIBS += -L$$(COINDIR)/lib -lcoin3s
  LIBS += -L$$(SOQTDIR)/lib -lsoqt1s
}

HEADERS         = ISpyDCSInterface.h \
		  ISpyDCSWidget.h

SOURCES         = ISpyDCSInterface.cpp \
		  ISpyDCSWidget.cpp

SOURCES += ../src/Framework/*.cc
SOURCES += ../src/Framework/*.c
SOURCES += ../src/QtGUI/*.cc
SOURCES += ../src/QtGUI/bin/ispy.cpp
HEADERS += ../src/QtGUI/*.h
HEADERS += ../src/Framework/*.h
RESOURCES = ../src/QtGUI/resources/ispy.qrc
FORMS = ../src/QtGUI/*.ui

QT += network opengl    
