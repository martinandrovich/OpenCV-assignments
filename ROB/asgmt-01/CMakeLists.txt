cmake_minimum_required(VERSION 2.8)
project(ROB-asgmt-01)

file(GLOB_RECURSE ROB-asgmt-01_SOURCES "src/*.cpp")
file(GLOB_RECURSE ROB-asgmt-01_HEADERS "src/*.hpp")
file(GLOB_RECURSE ROB-asgmt-01_HEADERS "src/*.h")

set(ROB-asgmt-01_INCLUDE_DIRS "")

foreach(_headerFile ${ROB-asgmt-01_HEADERS})
  get_filename_component(_dir ${_headerFile} PATH)
  list (APPEND ROB-asgmt-01_INCLUDE_DIRS ${_dir})
endforeach()

list(REMOVE_DUPLICATES ROB-asgmt-01_INCLUDE_DIRS)

find_package( OpenCV REQUIRED)

add_executable( ROB-asgmt-01 main.cpp ${ROB-asgmt-01_SOURCES} )
target_link_libraries( ROB-asgmt-01 PRIVATE ${OpenCV_LIBS} )
