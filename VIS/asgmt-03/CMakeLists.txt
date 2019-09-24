cmake_minimum_required(VERSION 2.8)

# project settings
project(vis-asgmt-03)
set (CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(vis-asgmt-03_INCLUDE_DIRS "")

# warning levels
# set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

# source folder (src/)
file(GLOB_RECURSE vis-asgmt-03_SOURCES "src/*.cpp")
file(GLOB_RECURSE vis-asgmt-03_HEADERS "src/*.hpp")
file(GLOB_RECURSE vis-asgmt-03_HEADERS "src/*.h")

foreach(_headerFile ${vis-asgmt-03_HEADERS})
  get_filename_component(_dir ${_headerFile} PATH)
  list (APPEND vis-asgmt-03_INCLUDE_DIRS ${_dir})
endforeach()

# dependencies
find_package(OpenCV REQUIRED)
find_package(PythonLibs 2.7)

# executable
add_executable(vis-asgmt-03 main.cpp ${vis-asgmt-03_SOURCES})

# link libraries
target_link_libraries(vis-asgmt-03 PRIVATE ${OpenCV_LIBS})
target_include_directories(vis-asgmt-03 PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(vis-asgmt-03 ${PYTHON_LIBRARIES})

list(REMOVE_DUPLICATES vis-asgmt-03_INCLUDE_DIRS)