cmake_minimum_required(VERSION 3.12)
project(template)

set(CMAKE_CXX_STANDARD 17)

set(TEST_ENABLE FALSE CACHE BOOL "Enable unit tests")
set(TEST_GTEST FALSE CACHE BOOL "Test that gtest is working")

if (${TEST_ENABLE})
    add_subdirectory(test)
endif()

add_subdirectory(template)