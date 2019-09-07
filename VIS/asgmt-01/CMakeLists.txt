cmake_minimum_required(VERSION 2.8)

# project settings
project(vis-asgmt-01)
set (CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(vis-asgmt-01_INCLUDE_DIRS "")

# source folder (src/)
file(GLOB_RECURSE vis-asgmt-01_SOURCES "src/*.cpp")
file(GLOB_RECURSE vis-asgmt-01_HEADERS "src/*.hpp")
file(GLOB_RECURSE vis-asgmt-01_HEADERS "src/*.h")

foreach(_headerFile ${vis-asgmt-01_HEADERS})
  get_filename_component(_dir ${_headerFile} PATH)
  list (APPEND vis-asgmt-01_INCLUDE_DIRS ${_dir})
endforeach()

list(REMOVE_DUPLICATES vis-asgmt-01_INCLUDE_DIRS)

# dependencies
find_package(OpenCV REQUIRED)

#
add_executable(vis-asgmt-01 main.cpp ${vis-asgmt-01_SOURCES})
target_link_libraries(vis-asgmt-01 PRIVATE ${OpenCV_LIBS})
