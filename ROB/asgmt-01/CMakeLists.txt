cmake_minimum_required(VERSION 2.8)

# project settings
project(rob-asgmt-01)
set (CMAKE_CXX_STANDARD 17)
#set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(rob-asgmt-01_INCLUDE_DIRS "")

# disable optimizations
SET(CMAKE_CXX_FLAGS "-O0")
SET(CMAKE_C_FLAGS "-O0")

# source folder (src/)
file(GLOB_RECURSE rob-asgmt-01_SOURCES "src/*.cpp")
file(GLOB_RECURSE rob-asgmt-01_HEADERS "src/*.hpp")
file(GLOB_RECURSE rob-asgmt-01_HEADERS "src/*.h")

foreach(_headerFile ${rob-asgmt-01_HEADERS})
  get_filename_component(_dir ${_headerFile} PATH)
  list (APPEND rob-asgmt-01_INCLUDE_DIRS ${_dir})
endforeach()

list(REMOVE_DUPLICATES rob-asgmt-01_INCLUDE_DIRS)

# dependencies
find_package(OpenCV REQUIRED)

#
add_executable(rob-asgmt-01 main.cpp ${rob-asgmt-01_SOURCES})
target_link_libraries(rob-asgmt-01 PRIVATE ${OpenCV_LIBS})