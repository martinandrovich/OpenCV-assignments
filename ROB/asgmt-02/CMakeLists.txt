cmake_minimum_required(VERSION 3.13)

# project name
project(rob-asgmt-02)

# project settings
set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# warning levels
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")

# source folder (.cpp + .h files)
file(GLOB SOURCES src/*.cpp)
include_directories(${PROJECT_SOURCE_DIR}/src)

# executable
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

# dependencies

## opencv
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

## python libs
find_package(PythonLibs 2.7)
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
