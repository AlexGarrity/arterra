cmake_minimum_required(VERSION 3.12)
project(arterra)

set(CMAKE_CXX_STANDARD 17)

set(TEST_ENABLE FALSE CACHE BOOL "Enable unit tests")
set(TEST_GTEST FALSE CACHE BOOL "Test that gtest is working")

find_package(OpenGL REQUIRED)

if (${TEST_ENABLE})
    add_subdirectory(test)
endif()

add_subdirectory(ext)
add_subdirectory(arterra)