cmake_minimum_required(VERSION 2.8)
project ( lektion1 )

file(GLOB_RECURSE lektion1_SOURCES "src/*.cpp")
file(GLOB_RECURSE lektion1_HEADERS "src/*.hpp")
file(GLOB_RECURSE lektion1_HEADERS "src/*.h")

set (lektion1_INCLUDE_DIRS "")
foreach (_headerFile ${lektion1_HEADERS})
  get_filename_component(_dir ${_headerFile} PATH)
  list (APPEND lektion1_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES lektion1_INCLUDE_DIRS)

find_package( OpenCV REQUIRED)

add_executable( lektion1 main.cpp ${lektion1_SOURCES} )
target_link_libraries( lektion1 PRIVATE ${OpenCV_LIBS} )
