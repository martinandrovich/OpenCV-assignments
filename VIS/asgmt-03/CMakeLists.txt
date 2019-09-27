cmake_minimum_required(VERSION 3.13)

# project name
project(vis-asgmt-03)

# project settings
set (CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

# warning levels
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

# source folder (src/)
file(GLOB SOURCES src/*.cpp)

# dependencies
find_package(OpenCV REQUIRED)
find_package(PythonLibs 2.7)

# executable
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

# header files
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PYTHON_INCLUDE_DIRS})

# link libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})