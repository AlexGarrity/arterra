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


### SFML + Settings
set(SFML_BUILD_WINDOW TRUE CACHE BOOL "TRUE to build SFML's Window module. This setting is ignored, if the graphics module is built." FORCE)
set(SFML_BUILD_GRAPHICS TRUE CACHE BOOL "TRUE to build SFML's Graphics module." FORCE)
set(SFML_BUILD_AUDIO FALSE CACHE BOOL "TRUE to build SFML's Audio module." FORCE)
set(SFML_BUILD_NETWORK FALSE CACHE BOOL "TRUE to build SFML's Network module." FORCE)
# Add subdir
add_subdirectory(SFML)

### GLM (No settings)
add_subdirectory(glm)