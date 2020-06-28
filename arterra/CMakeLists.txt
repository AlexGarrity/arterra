
FILE(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(arterra ${SOURCES})

target_include_directories(arterra PRIVATE include)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

install(TARGETS arterra)