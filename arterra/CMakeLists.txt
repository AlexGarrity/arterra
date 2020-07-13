
# Require OpenGL, invoking `FindOpenGL.cmake`.  Exit if not found
find_package(OpenGL REQUIRED)

# Finds all files in `src` ending with `.cpp` and puts them into a variable called `SOURCES`
FILE(GLOB_RECURSE SOURCES src/**.cpp)

# Creates an executable out of all of the `SOURCES`
add_executable(arterra ${SOURCES})

# Pre-compile the pre-compiled header
target_precompile_headers(arterra PRIVATE include/PCH.hpp)

# Specifies include directories to compile using
target_include_directories(arterra PRIVATE include)

# Adds `OPENGL_INCLUDE_DIRS` variable created by `FindOpenGL.cmake` to executable
target_include_directories(arterra PRIVATE ${OPENGL_INCLUDE_DIRS})

# Link OpenGL to executable using `OPENGL_LIBRARIES` variable created by `FindOpenGL.cmake`
target_link_libraries(arterra ${OPENGL_LIBRARIES})

# Link other libraries to executable
target_link_libraries(arterra glad sfml-graphics glm yaml-cpp)

# Move the res directory next to the build
if (WIN32)
    
    foreach(config IN LISTS CMAKE_CONFIGURATION_TYPES)
        file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${config}")
        add_custom_command(TARGET arterra POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/res" "${CMAKE_CURRENT_BINARY_DIR}/${config}/res")
    endforeach()
elseif(UNIX)
    add_custom_command(TARGET arterra POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_SOURCE_DIR}/res" "${CMAKE_CURRENT_BUILD_DIR}/res")
endif()

# Export compile commands (for clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Install target does something
install(TARGETS arterra)
