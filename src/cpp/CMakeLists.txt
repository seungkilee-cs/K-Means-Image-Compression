cmake_minimum_required(VERSION 3.16)
project(imgc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_library(imgc_core
    src/image_preprocessor.cpp
    src/image_io.cpp
    src/timing.cpp
    src/cli.cpp
    src/algorithms/KMeansCompressor.cpp
)

target_include_directories(imgc_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(imgc_core
    PUBLIC
        ${OpenCV_LIBS}
)

add_executable(kmeans_imgc app/main.cpp)
target_link_libraries(kmeans_imgc PRIVATE imgc_core)

include(FetchContent)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(imgc_tests tests/image_preprocessor_test.cpp)
target_link_libraries(imgc_tests PRIVATE imgc_core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(imgc_tests)
