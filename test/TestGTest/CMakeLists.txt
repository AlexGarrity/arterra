# Set the `TEST_NAME` variable to the project name + `TestGTest`
set (TEST_NAME ${PROJECT_NAME}TestGTest)

# Set TEST_SRC to Main.cpp
file(GLOB_RECURSE TEST_SRC
    Main.cpp
)

# Create the test executable from `TEST_SRC`
add_executable(${TEST_NAME} ${TEST_SRC})

# Link gtest and gmock to the test executable
target_link_libraries(${TEST_NAME} libgtest libgmock) 

# Find all tests in the test executable (part of the CMake GoogleTest module)
gtest_discover_tests(${TEST_NAME})