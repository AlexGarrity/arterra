# Add more subdirectories but with settings this time

### set syntax (roughly):
# set (VARIABLE_NAME VALUE CACHE TYPE DESCRIPTION [FORCE])
# CACHE saves the value to a file, TYPE can be STRING, BOOL, some other stuff...
# DESCRIPTION shows up in the GUI and CMake interactive mode
# FORCE forces it to change

### GLAD + Settings
# Core profile = modern OGL
set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile" FORCE)
# Set API level to 4.0
set(GLAD_API "gl=4.0" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest" FORCE)
# Spec is GL
set(GLAD_SPEC "gl" CACHE STRING "Name of the spec" FORCE)
# Don't generate install target
option(GLAD_INSTALL "Generate installation target" OFF)
# Export so we can use it
option(GLAD_EXPORT "Set export variables for external project" ON)
# Add subdir
add_subdirectory(glad)


### GLFW + Settings
# Don't build tests
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
# Don't build docs
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
# Don't generate install target
option(GLFW_INSTALL "Generate installation target" ON)
# Not using Vulkan (yet)
option(GLFW_VULKAN_STATIC "Assume the Vulkan loader is linked with the application" OFF)
# Add subdir
add_subdirectory(glfw)

### GLM (No settings)
add_subdirectory(glm)

### ImGUI
add_subdirectory(imgui)
