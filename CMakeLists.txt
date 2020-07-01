# Set minimum version to 3.16 (required for pre-compiled header)
cmake_minimum_required(VERSION 3.16)

# Define the project
project(arterra)

# Set C++ standard (specification) to C++17
set(CMAKE_CXX_STANDARD 17)

# Add (CMakeLists.txt files in) subdirectories
add_subdirectory(test)
add_subdirectory(ext)
add_subdirectory(arterra)
