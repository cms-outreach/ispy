cmake_minimum_required(VERSION 2.8)
project(iSpy)
include(FindQt4)
enable_testing()

set(CMAKE_OSX_ARCHITECTURES "ppc;i386")
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.5)
set(CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)

# Improved wrap ui macro. Now output files are in ui/
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/ui)
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


find_package(Qt4 REQUIRED)

# By default we assume that the place where qt is the one that matters.
set(ISPY_EXTERNALS_PATH ${QT_LIBRARY_DIR}/..)
set(CMAKE_LIBRARY_PATH ${ISPY_EXTERNALS_PATH}/lib)
set(CMAKE_INCLUDE_PATH ${ISPY_EXTERNALS_PATH}/include)


file(GLOB FRAMEWORK_SOURCES "src/Framework/*.cc" "src/Framework/*.c")
include_directories("src" "src/Framework")
add_library(ISpyFramework STATIC ${FRAMEWORK_SOURCES})
find_package(Qt4 4.5.0)
# By default CMake uses the framework installed from the official package.
# We want to use the static library we compiled.
if(APPLE)
   find_library(COIN3D_LIBRARIES Coin)
   find_path(COIN3D_INCLUDE_DIRS Inventor/So.h NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
else(APPLE)
   find_package(Coin3D REQUIRED)
endif(APPLE)

find_package(OpenGL REQUIRED)

find_library(PCRE_LIBRARY pcre REQUIRED)
find_library(FREETYPE_LIBRARY freetype REQUIRED)
find_library(FONTCONFIG_LIBRARY fontconfig REQUIRED)
find_library(SOQT_LIBRARY SoQt REQUIRED)
find_library(ZLIB_LIBRARY z REQUIRED NO_SYSTEM_ENVIRONMENT_PATH NO_CMAKE_SYSTEM_PATH)
if(APPLE)
   find_library(EXPAT_LIBRARY expatstatic REQUIRED)
   find_library(SECURITY_LIBRARY Security)
else(APPLE)
   find_library(EXPAT_LIBRARY expat REQUIRED)
endif(APPLE)

include(${QT_USE_FILE})

# Build igdumpstats
add_executable(igdumpstats src/Framework/bin/igdumpstats.cpp)
target_link_libraries(igdumpstats ISpyFramework)

# Build iSpy Qt client.
file(GLOB QTRC_FILES "src/QtGUI/resources/*.qrc")
file(GLOB QTGUI_SOURCES "src/QtGUI/*.cc")
file(GLOB QTGUI_UIC_FILES "src/QtGUI/*.ui")
file(GLOB QTGUI_MOC_FILES "src/QtGUI/*.h")

qt4_add_resources(QTGUI_RESOURCES ${QTRC_FILES})
ispy_wrap_ui(QTGUI_UIC_PROCESSED_FILES ${QTGUI_UIC_FILES})
qt4_wrap_cpp(QTGUI_MOC_PROCESSED_FILES ${QTGUI_MOC_FILES})
include_directories("src" "src/QtGUI" ${QT_INCLUDE_DIR} 
                    ${QT_QTNETWORK_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
set (ISPY_SOURCES "src/QtGUI/bin/ispy.cpp" ${QTGUI_SOURCES} 
                  ${QTGUI_RESOURCES} 
                  ${QTGUI_UIC_PROCESSED_FILES}
                  ${QTGUI_MOC_PROCESSED_FILES})

if(APPLE)
   add_executable(iSpy MACOSX_BUNDLE ${ISPY_SOURCES})
   set_target_properties(iSpy PROPERTIES 
                              MACOSX_BUNDLE_INFO_PLIST "src/QtGUI/resources/Info.plist")
else(APPLE)
   add_executable(ispy ${ISPY_SOURCES})
endif(APPLE)

target_link_libraries(ispy ISpyFramework ${QT_LIBRARIES} 
                      ${QT_QTNETWORK_LIBRARIES}
                      ${QT_QTOPENGL_LIBRARIES} ${PCRE_LIBRARY}
                      ${COIN3D_LIBRARIES} ${SOQT_LIBRARY} ${OPENGL_LIBRARIES}
                      ${FREETYPE_LIBRARY} ${FONTCONFIG_LIBRARY} 
                      ${ZLIB_LIBRARY})

# Build unit tests for igfiles.
file(GLOB FRAMEWORK_TESTS_SOURCES "src/Framework/tests/test_igfiles-*.cpp")
qt4_wrap_cpp(FRAMEWORK_TESTS_MOCS src/Framework/tests/test_igfiles-driver.h)
add_executable(test_igfiles ${FRAMEWORK_TESTS_SOURCES} ${FRAMEWORK_TESTS_MOCS})
target_link_libraries(test_igfiles ISpyFramework ${QT_LIBRARIES} 
                      ${QT_QTTEST_LIBRARIES} ${SECURITY_LIBRARY})

# Build unit tests for style parser.
add_executable(test_StyleParser "src/Framework/tests/test_StyleParser.cpp")
target_link_libraries(test_StyleParser ISpyFramework ${SECURITY_LIBRARY})
add_test(test_StyleParser ${EXECUTABLE_OUTPUT_PATH}/test_StyleParser)

# Build .app on mac.
if (APPLE)
set(APPS ${CMAKE_CURRENT_BINARY_DIR}/iSpy)  # paths to executables
set(DIRS ${CMAKE_CURRENT_BINARY_DIR})   # directories to search for prerequisites
INSTALL(CODE "
   include(BundleUtilities)
   fixup_bundle(\"${APPS}\"   \"\"   \"${DIRS}\")
   " COMPONENT Runtime)
endif(APPLE)

add_test(test_igfiles ${EXECUTABLE_OUTPUT_PATH}/test_igfiles)
