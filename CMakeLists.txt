cmake_minimum_required(VERSION 2.8)
project(iSpy)
include(FindQt4)
enable_testing()

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


file(GLOB FRAMEWORK_SOURCES "src/Framework/*.cc")
include_directories("src" "src/Framework")
add_library(ISpyFramework STATIC ${FRAMEWORK_SOURCES})
find_package(Qt4 4.5.0)
# By default CMake uses the framework installed from the official package.
# We want to use the static library we compiled.
if(APPLE)
   find_path(COIN3D_INCLUDE_DIRS Inventor/So.h)
   find_library(COIN3D_LIBRARIES Coin)
else(APPLE)
   find_package(Coin3d REQUIRED)
endif(APPLE)

find_package(OpenGL REQUIRED)

find_library(PCRE_LIBRARY pcre REQUIRED)
find_library(CLASSLIB_LIBRARY classlib REQUIRED)
find_library(FREETYPE_LIBRARY freetype REQUIRED)
find_library(FONTCONFIG_LIBRARY fontconfig REQUIRED)
find_library(SOQT_LIBRARY SoQt REQUIRED)
find_library(ZLIB_LIBRARY z REQUIRED)
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
message(${QTGUI_UIC_PROCESSED_FILES})
include_directories("src" "src/QtGUI" ${QT_INCLUDE_DIR} 
                    ${QT_QTNETWORK_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
add_executable(iSpy "src/QtGUI/bin/ispy.cpp" ${QTGUI_SOURCES} 
                    ${QTGUI_RESOURCES} 
                    ${QTGUI_UIC_PROCESSED_FILES}
                    ${QTGUI_MOC_PROCESSED_FILES})
target_link_libraries(iSpy ISpyFramework ${QT_LIBRARIES} 
                      ${QT_QTNETWORK_LIBRARIES}
                      ${QT_QTOPENGL_LIBRARIES} ${PCRE_LIBRARY}
                      ${COIN3D_LIBRARIES} ${SOQT_LIBRARY} ${OPENGL_LIBRARIES}
                      ${FREETYPE_LIBRARY} ${FONTCONFIG_LIBRARY} 
                      ${CLASSLIB_LIBRARY})


# Build unit tests for Framework.
file(GLOB FRAMEWORK_TESTS_SOURCES "src/Framework/tests/*.cpp")
qt4_wrap_cpp(FRAMEWORK_TESTS_MOCS src/Framework/tests/test_driver.cpp)
add_executable(ISpyFrameworkTests ${FRAMEWORK_TESTS_SOURCES})
target_link_libraries(ISpyFrameworkTests ISpyFramework ${QT_LIBRARIES} 
                      ${QT_QTTEST_LIBRARIES} ${SECURITY_LIBRARY})

add_test(SimpleTest ${EXECUTABLE_OUTPUT_PATH}/ISpyFrameworkTests)