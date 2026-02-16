cmake_minimum_required(VERSION 3.16)
project(motion_detection_manual LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(app
    main.cpp
    src/Camera.cpp
    src/FrameDifferencing.cpp
    src/RunningAverage.cpp
)

target_include_directories(app PRIVATE
    ${OpenCV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(app PRIVATE ${OpenCV_LIBS})
