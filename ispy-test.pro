SOURCES += Iguana/Framework/src/IgCollection.cc
SOURCES += Iguana/Framework/test/test_CreateSimpleCollection.cpp 
SOURCES += Iguana/Framework/test/test_CreateSimpleAssociations.cpp
SOURCES += Iguana/Framework/test/test_CreateSimpleAssociations02.cpp
SOURCES += Iguana/Framework/test/test_NavigateAssociations.cpp
SOURCES += Iguana/Framework/test/test_CreateWriteReadWriteReadPrintNoAssociations.cpp
SOURCES += Iguana/Framework/test/test_CreateWriteReadWriteReadPrint.cpp

SOURCES += Iguana/Framework/test/test_driver.cpp 
INCLUDEPATH += Iguana/Framework/interface

QMAKE_CXXFLAGS_RELEASE = -O0 -ggdb

mac:QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
mac:CONFIG += x86 ppc

OBJECTS_DIR = tests/tmp
MOC_DIR = tests/tmp
RCC_DIR = tests/tmp
UI_DIR = tests/ui

CONFIG  += qtestlib
