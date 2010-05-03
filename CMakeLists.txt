cmake_minimum_required(VERSION 2.8)
project(iSpy)
file(GLOB FRAMEWORK_SOURCES "src/Framework/*.cc")
include_directories("src" "src/Framework")
add_library(ISpyFramework STATIC ${FRAMEWORK_SOURCES})

add_executable(igdumpstats src/Framework/bin/igdumpstats.cpp)
target_link_libraries(igdumpstats ISpyFramework)