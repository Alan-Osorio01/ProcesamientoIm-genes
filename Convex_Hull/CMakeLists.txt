cmake_minimum_required(VERSION 3.10)
project(convex)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

add_executable(convex
    main.cpp
    src/hull.cpp
)

target_link_libraries(convex ${OpenCV_LIBS})
