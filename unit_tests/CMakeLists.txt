cmake_minimum_required(VERSION 2.6)

project(indeer)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 17)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

include_directories(../include)

file(GLOB SOURCES "../src/ThreadPool.cpp" "../src/DirectoryInspector.cpp" "tests.cpp")

add_executable(run_tests ${SOURCES})

target_link_libraries(run_tests ${GTEST_LIBRARIES} pthread)
