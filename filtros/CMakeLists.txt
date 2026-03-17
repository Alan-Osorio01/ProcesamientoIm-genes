cmake_minimum_required(VERSION 3.10)
project(FILTROS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

include_directories(include ${OpenCV_INCLUDE_DIRS})

add_executable(FILTROS
    main.cpp
    src/utils.cpp
    src/gray.cpp
    src/log.cpp
    src/zero_crossing.cpp
    src/scharr.cpp
    src/laplacian.cpp
    src/sobel_magnitude.cpp
    src/canny.cpp
)

target_link_libraries(FILTROS ${OpenCV_LIBS})