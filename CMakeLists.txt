# TBD: Build tests and examples only if this is the top-level project.
# if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
#     # We are the top-level project.
# else()
#     # We are being included via add_subdirectory().
# endif()
#

cmake_minimum_required(VERSION 3.16)
project(strqueue
    VERSION 1.0
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library without object code.
add_library(strqueue INTERFACE)

# Include directories for the library
target_include_directories(strqueue
    INTERFACE
       $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
       $<INSTALL_INTERFACE:include/strqueue>
)

install(DIRECTORY include/ DESTINATION include/strqueue)
# Install the library and create package 'export set' descriptor strqueueTargets.
install(TARGETS strqueue EXPORT strqueueTargets)
# Write package 'export set' to a file. It allows clients to import the package.
install(EXPORT strqueueTargets
        FILE strqueueTargets.cmake
        NAMESPACE strqueue::
        DESTINATION lib/cmake/strqueue)
# Write package configuration file; it allows clients to find the package descriptor file.
install(FILES cmake/strqueueConfig.cmake
        DESTINATION lib/cmake/strqueue)

add_subdirectory(test)
add_subdirectory(example)
