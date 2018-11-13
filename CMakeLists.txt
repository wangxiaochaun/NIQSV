cmake_minimum_required(VERSION 3.12)
project(exercise)

find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES main.cpp)

add_executable(exercise main.cpp model.cpp model.h)
target_link_libraries(exercise ${OpenCV_LIBS})