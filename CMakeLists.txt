cmake_minimum_required(VERSION 2.8)
project(iSpy)
#include(FindQt4)
enable_testing()

set(LIBRARY_OUTPUT_PATH build)
set(EXECUTABLE_OUTPUT_PATH build)
set(CMAKE_CURRENT_BINARY_DIR build)
set(CMAKE_OSX_ARCHITECTURES "x86_64")
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.5)
#set(CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)

# My QT detection code. FindQt4 behaves weirdly and picks up
# unwanted versions from the system, if found. I prefer to hard
# code all I need.
set(QT_USE_FRAMEWORKS OFF)
set(QT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/externals/sw)
set(QT_MOC_EXECUTABLE ${QT_ROOT}/bin/moc)
set(QT_UIC_EXECUTABLE ${QT_ROOT}/bin/uic)
set(QT_RCC_EXECUTABLE ${QT_ROOT}/bin/rcc)
set(QT_LIBRARY_DIR ${QT_ROOT}/lib)

find_path(QT_CORE_HEADERS QtCore PATHS ${QT_ROOT}/include/QtCore NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)                            
find_path(QT_GUI_HEADERS QtGui PATHS ${QT_ROOT}/include/QtGui NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)              
find_path(QT_NETWORK_HEADERS QtNetwork PATHS ${QT_ROOT}/include/QtNetwork NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)             

find_library(QT_NETWORK_LIBRARY libQtNetwork.a QtNetwork ${QT_ROOT}/lib NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(QT_GUI_LIBRARY libQtGUI.a QtGui ${QT_ROOT}/lib NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(QT_CORE_LIBRARY libQtCore.a QtCore ${QT_ROOT}/lib NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(QT_OPENGL_LIBRARY libQtOpenGL.a QtOpenGL ${QT_ROOT}/lib NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
message(${QT_GUI_LIBRARY})

set(QT_INCLUDE_DIRS ${QT_ROOT}/include ${QT_CORE_HEADERS} ${QT_GUI_HEADERS} ${QT_NETWORK_HEADERS})

# Improved wrap ui macro. Now output files are in ui/
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ui)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/moc)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/rsrc)

MACRO (ISPY_WRAP_UI outfiles )

  FOREACH (it ${ARGN})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/ui/ui_${outfile}.h)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_UIC_EXECUTABLE}
      ARGS -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH (it)

ENDMACRO (ISPY_WRAP_UI)

MACRO (ISPY_WRAP_MOC outfiles)

  FOREACH (it ${ARGN})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/moc/moc_${outfile}.cc)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_MOC_EXECUTABLE}
      ARGS -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH (it)

ENDMACRO (ISPY_WRAP_MOC)

# Poor man resource handling. This will require to touch the qrc file every
# time you want to have a new one.
MACRO(ISPY_WRAP_RSRC outfiles)
  MESSAGE(${outfiles})
  FOREACH(it ${ARGN})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/rsrc/rsrc_${outfile}.cc)
    MESSAGE(${outfile})
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_RCC_EXECUTABLE}
      ARGS -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH(it)
ENDMACRO(ISPY_WRAP_RSRC)

# By default we assume that the place where qt is the one that matters.
set(ISPY_EXTERNALS_PATH ${QT_ROOT})
set(CMAKE_LIBRARY_PATH ${ISPY_EXTERNALS_PATH}/lib)
set(CMAKE_INCLUDE_PATH ${ISPY_EXTERNALS_PATH}/include)


file(GLOB FRAMEWORK_SOURCES "src/Framework/*.cc" "src/Framework/*.c")
include_directories("src" "src/Framework")
add_library(ISpyFramework STATIC ${FRAMEWORK_SOURCES})
#find_package(Qt4)
# By default CMake uses the framework installed from the official package.
# We want to use the static library we compiled.
if(APPLE)
   find_library(COIN3D_LIBRARIES Coin)
   find_path(COIN3D_INCLUDE_DIRS Inventor/So.h NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
else(APPLE)
   find_package(Coin3D REQUIRED)
endif(APPLE)

find_package(OpenGL REQUIRED)

find_library(FREETYPE_LIBRARY libfreetype.a freetype REQUIRED)
find_library(FONTCONFIG_LIBRARY libfontconfig.a fontconfig REQUIRED)
find_library(SOQT_LIBRARY libSoQt.a SoQt REQUIRED)
find_library(ZLIB_LIBRARY libz.a z REQUIRED NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
find_library(ICONV_LIBRARY iconv REQUIRED)
if(APPLE)
   find_library(EXPAT_LIBRARY libexpatstatic.a expatstatic REQUIRED)
   find_library(SECURITY_LIBRARY Security)
else(APPLE)
   find_library(EXPAT_LIBRARY libexpat.a expat REQUIRED)
endif(APPLE)

#include(${QT_USE_FILE})

# Build igdumpstats
add_executable(igdumpstats src/Framework/bin/igdumpstats.cpp)
target_link_libraries(igdumpstats ISpyFramework)

# Build iSpy Qt client.
file(GLOB QTRC_FILES "src/QtGUI/resources/*.qrc")
file(GLOB QTGUI_SOURCES "src/QtGUI/*.cc")
file(GLOB QTGUI_UIC_FILES "src/QtGUI/*.ui")
file(GLOB QTGUI_MOC_FILES "src/QtGUI/*.h")
file(GLOB QTGUI_RCC_FILES "src/QtGUI/resources/*.qrc")

#qt4_add_resources(QTGUI_RESOURCES ${QTRC_FILES})
ispy_wrap_ui(QTGUI_UIC_PROCESSED_FILES ${QTGUI_UIC_FILES})
ispy_wrap_moc(QTGUI_MOC_PROCESSED_FILES ${QTGUI_MOC_FILES})
ispy_wrap_rsrc(QTGUI_RSRC_PROCESSED_FILES ${QTGUI_RCC_FILES})
#message(${QT_INCLUDE_DIR})
include_directories("src" "src/QtGUI" ${QT_INCLUDE_DIRS} 
                    ${QT_NETWORK_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
set (ISPY_SOURCES "src/QtGUI/bin/ispy.cpp" ${QTGUI_SOURCES} 
                  ${QTGUI_RESOURCES} 
                  ${QTGUI_UIC_PROCESSED_FILES}
                  ${QTGUI_MOC_PROCESSED_FILES}
                  ${QTGUI_RSRC_PROCESSED_FILES})

if(APPLE)
  set(ISPY_NAME iSpy)
else(APPLE)
  set(ISPY_NAME ispy)
endif(APPLE)

if(APPLE)
   add_executable(${ISPY_NAME} MACOSX_BUNDLE ${ISPY_SOURCES})
   set_target_properties(${ISPY_NAME} PROPERTIES 
                              MACOSX_BUNDLE_INFO_PLIST "src/QtGUI/resources/Info.plist")
else(APPLE)
   add_executable(${ISPY_NAME} ${ISPY_SOURCES})
endif(APPLE)

if (APPLE)
    find_library(OPENGL_FRAMEWORK OpenGL)
    find_library(FOUNDATION_FRAMEWORK Foundation)
    find_library(SSL_LIBRARY ssl)
    find_library(CRYPTO_LIBRARY crypto)
    find_library(APPLICATION_SERVICES_FRAMEWORK ApplicationServices)
    find_library(APPKIT_FRAMEWORK AppKit)
    find_library(CARBON_FRAMEWORK Carbon)
    find_library(SYSTEMCONFIGURATION_FRAMEWORK SystemConfiguration)
    set(ADDITIONAL_LIBS ${OPENGL_FRAMEWORK} ${SSL_LIBRARY} 
                        ${CRYPTO_LIBRARY}
                        ${FOUNDATION_FRAMEWORK} 
                        ${APPLICATION_SERVICES_FRAMEWORK} 
                        ${APPKIT_FRAMEWORK}
                        ${CARBON_FRAMEWORK}
                        ${SYSTEMCONFIGURATION_FRAMEWORK})
else(APPLE)
    find_library(SSL_LIBRARY ssl)
    find_library(XRENDER_LIBRARY Xrender)
    set(ADDITIONAL_LIBS ${SSL_LIBRARY} ${XRENDER_LIBRARY})
endif(APPLE)

target_link_libraries(${ISPY_NAME} ISpyFramework
                      ${SOQT_LIBRARY}
                      ${QT_NETWORK_LIBRARY}
                      ${QT_OPENGL_LIBRARY}
                      ${QT_GUI_LIBRARY}
                      ${QT_CORE_LIBRARY}
                      ${COIN3D_LIBRARIES} ${OPENGL_LIBRARIES}
                      ${FONTCONFIG_LIBRARY} ${ICONV_LIBRARY} ${FREETYPE_LIBRARY}
                      ${ZLIB_LIBRARY} ${ADDITIONAL_LIBS})

# Build unit tests for igfiles.
file(GLOB FRAMEWORK_TESTS_SOURCES "src/Framework/tests/test_igfiles-*.cpp")
#qt4_wrap_cpp(FRAMEWORK_TESTS_MOCS src/Framework/tests/test_igfiles-driver.h)
add_executable(test_igfiles ${FRAMEWORK_TESTS_SOURCES} ${FRAMEWORK_TESTS_MOCS})
target_link_libraries(test_igfiles ISpyFramework ${QT_LIBRARIES} 
                      ${QT_QTTEST_LIBRARIES} ${SECURITY_LIBRARY})

# Build unit tests for style parser.
add_executable(test_StyleParser "src/Framework/tests/test_StyleParser.cpp")
target_link_libraries(test_StyleParser ISpyFramework ${SECURITY_LIBRARY})
add_test(test_StyleParser ${EXECUTABLE_OUTPUT_PATH}/test_StyleParser)

# Build command line tool
#add_executable(jspy "src/JSClient/main.cc")
#target_link_libraries(jspy ISpyFramework ${SECURITY_LIBRARY})

# Build .app on mac.
if (APPLE)
set(APPS ${CMAKE_CURRENT_BINARY_DIR}/iSpy)  # paths to executables
set(DIRS ${CMAKE_CURRENT_BINARY_DIR})   # directories to search for prerequisites
INSTALL(CODE "
   include(BundleUtilities)
   fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
   " COMPONENT Runtime)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/externals/sw/src/qt/src/gui/mac/qt_menu.nib
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/iSpy.app/Contents/Resources/ )
endif(APPLE)

add_test(test_igfiles ${EXECUTABLE_OUTPUT_PATH}/test_igfiles)
