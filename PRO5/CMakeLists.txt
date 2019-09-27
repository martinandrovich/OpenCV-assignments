cmake_minimum_required(VERSION 2.8)

# project settings
project(rb-pro5)
set (CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(rb-pro5_INCLUDE_DIRS "")

# warning levels
# set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

# source folder (src/)
file(GLOB_RECURSE rb-pro5_SOURCES "src/*.cpp")
file(GLOB_RECURSE rb-pro5_HEADERS "src/*.hpp")
file(GLOB_RECURSE rb-pro5_HEADERS "src/*.h")

foreach(_headerFile ${rb-pro5_HEADERS})
	get_filename_component(_dir ${_headerFile} PATH)
	list (APPEND rb-pro5_INCLUDE_DIRS ${_dir})
endforeach()

# library dirs
link_directories(${PROJECT_SOURCE_DIR}/lib/)

# dependencies
find_package(OpenCV REQUIRED)
find_package(PythonLibs 2.7)

#add_library(fuzzylite-static STATIC IMPORTED)
#set_property(TARGET fuzzylite-static PROPERTY IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/lib/fuzzylite/bin/libfuzzylite-static.a)

# executable
add_executable(rb-pro5 main.cpp ${rb-pro5_SOURCES})

# header files
target_include_directories(rb-pro5 PRIVATE ${PYTHON_INCLUDE_DIRS})
target_include_directories(rb-pro5 PRIVATE ${PROJECT_SOURCE_DIR}/lib/fuzzylite/)

# link libraries
target_link_libraries(rb-pro5 PRIVATE ${OpenCV_LIBS})
target_link_libraries(rb-pro5 PRIVATE ${PYTHON_LIBRARIES})
#target_link_libraries(rb-pro5 PRIVATE fuzzylite-static)
target_link_libraries(rb-pro5 PRIVATE ${PROJECT_SOURCE_DIR}/lib/fuzzylite/bin/libfuzzylite-static.a)

list(REMOVE_DUPLICATES rb-pro5_INCLUDE_DIRS)