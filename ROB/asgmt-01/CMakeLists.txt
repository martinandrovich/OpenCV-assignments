cmake_minimum_required(VERSION 2.8)
project(vis-asgmt-01)

file(GLOB_RECURSE vis-asgmt-01_SOURCES "src/*.cpp")
file(GLOB_RECURSE vis-asgmt-01_HEADERS "src/*.hpp")
file(GLOB_RECURSE vis-asgmt-01_HEADERS "src/*.h")

set(vis-asgmt-01_INCLUDE_DIRS "")

foreach(_headerFile ${vis-asgmt-01_HEADERS})
  get_filename_component(_dir ${_headerFile} PATH)
  list (APPEND vis-asgmt-01_INCLUDE_DIRS ${_dir})
endforeach()

list(REMOVE_DUPLICATES vis-asgmt-01_INCLUDE_DIRS)

find_package( OpenCV REQUIRED)

add_executable( vis-asgmt-01 main.cpp ${vis-asgmt-01_SOURCES} )
target_link_libraries( vis-asgmt-01 PRIVATE ${OpenCV_LIBS} )
