cmake_minimum_required(VERSION 2.6)

project(BCFSoftwareAssignment)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

include_directories(../include)

file(GLOB SOURCES "../src/ThreadPool.cpp" "../src/DirectoryInspector.cpp" "tests.cpp")

add_executable(run_tests ${SOURCES})

target_link_libraries(run_tests ${GTEST_LIBRARIES} pthread)
