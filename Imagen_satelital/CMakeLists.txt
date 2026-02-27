cmake_minimum_required(VERSION 3.10)
project(satellite)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

add_executable(satellite
    main.cpp
    src/processor.cpp
)

target_link_libraries(satellite ${OpenCV_LIBS})
