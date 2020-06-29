set(TEST_ENABLE FALSE CACHE BOOL "Enable unit tests")
set(TEST_GTEST FALSE CACHE BOOL "Test that gtest is working")

# Only add GTest if enabled
if (${TEST_ENABLE})
    # Require threads package
    find_package(Threads REQUIRED)
    # Add `ExternalProject` module
    include(ExternalProject)
    # Add `GoogleTest` module
    include(GoogleTest)

    # Add an external project with the specified repo URL
    ExternalProject_Add(
        gtest
        URL https://github.com/google/googletest/archive/release-1.10.0.zip
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
        # Disable install step
        INSTALL_COMMAND ""
    )

    # Get some properties from the external project
    ExternalProject_Get_Property(gtest source_dir binary_dir)

    # Create a library to import the remote to
    add_library(libgtest IMPORTED STATIC GLOBAL)
    # Add gtest as a dependency of libgtest
    add_dependencies(libgtest gtest)

    # If we're running Windows
    if(WIN32)
        # Set some properties (Windows uses .lib libraries)
        set_target_properties(libgtest PROPERTIES
            "IMPORTED_LOCATION" "${binary_dir}/lib/Debug/gtestd.lib"
            "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")
    else()
        # Set some properties but differently (Linux uses .a libraries)
        set_target_properties(libgtest PROPERTIES
            "IMPORTED_LOCATION" "${binary_dir}/lib/libgtest.a"
            "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")
    endif(WIN32)

    # Create another library to import to
    add_library(libgmock IMPORTED STATIC GLOBAL)
    # Add gtest as a dependency of libgmock
    add_dependencies(libgmock gtest)

    # Same thing as before but for gmock
    if(WIN32)
        set_target_properties(libgmock PROPERTIES
            "IMPORTED_LOCATION" "${binary_dir}/lib/Debug/gmockd.lib"
            "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")
    else()
        set_target_properties(libgmock PROPERTIES
            "IMPORTED_LOCATION" "${binary_dir}/lib/libgmock.a"
            "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")
    endif(WIN32)

    # Add googletest and googlemock include directories to root project
    include_directories(
        "${source_dir}/googletest/include"
        "${source_dir}/googlemock/include"
    )

    # If enabled, add unit tests for Google Test
    if (${TEST_GTEST})
        add_subdirectory(TestGTest)
    endif()
endif()
