cmake_minimum_required(VERSION 3.12)
project(imgc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(kmeans_imgc
    main.cpp
    image_processor.cpp
    lloyd.cpp
)

# headers are alongside sources in this directory
target_include_directories(kmeans_imgc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(kmeans_imgc PRIVATE ${OpenCV_LIBS})
