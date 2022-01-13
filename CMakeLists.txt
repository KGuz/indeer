cmake_minimum_required(VERSION 2.6)

project(BCFSoftwareAssignment)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH bin)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(inspect_dir ${SOURCES})

target_link_libraries(inspect_dir ${GTEST_LIBRARIES} pthread)
