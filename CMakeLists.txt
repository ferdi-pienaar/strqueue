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
       $<INSTALL_INTERFACE:include>
)

#install(DIRECTORY include/ DESTINATION include)
# Create export set strqueueTargets containing strqueue
#install(TARGETS strqueue EXPORT strqueueTargets)
# Write strqueueTargets to disk
#install(EXPORT strqueueTargets
#        FILE strqueue.cmake
#        NAMESPACE strqueue::
#        DESTINATION lib/cmake/strqueue)
#
#install(FILES cmake/strqueueConfig.cmake
#        DESTINATION lib/cmake/strqueue)

add_subdirectory(test)
add_subdirectory(example)
