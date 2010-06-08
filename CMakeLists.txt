cmake_minimum_required(VERSION 2.8)
project(iSpy)
include(FindQt4)

find_package(Qt4 REQUIRED)

file(GLOB FRAMEWORK_SOURCES "src/Framework/*.cc")
include_directories("src" "src/Framework")
add_library(ISpyFramework STATIC ${FRAMEWORK_SOURCES})
find_package(Qt4 4.5.0)
include(${QT_USE_FILE})

# Build igdumpstats
add_executable(igdumpstats src/Framework/bin/igdumpstats.cpp)
target_link_libraries(igdumpstats ISpyFramework)

# Build iSpy Qt client.
file(GLOB QTGUI_SOURCES "src/QtGUI/*.cc")
include_directories("src" "src/QtGUI")
add_executable(iSpy ${QTGUI_SOURCES})
target_link_libraries(iSpy ISpyFramework ${QT_LIBRARIES})

# Build unit tests for Framework.
file(GLOB FRAMEWORK_TESTS_SOURCES "src/Framework/tests/*.cpp")
qt4_wrap_cpp(FRAMEWORK_TESTS_MOCS src/Framework/tests/test_driver.cpp)
include_directories(${QT_INCLUDE_DIR})
add_executable(ISpyFrameworkTests ${FRAMEWORK_TESTS_SOURCES})
target_link_libraries(ISpyFrameworkTests ISpyFramework ${QT_LIBRARIES} ${QT_QTTEST_LIBRARIES})

add_test(SimpleTest ${EXECUTABLE_OUTPUT_PATH}/ISpyFrameworkTests Hello)