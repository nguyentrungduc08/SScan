cmake_minimum_required (VERSION 2.6)

# set proecject name
project (SScan)

#set compiler
set (CMAKE_CXX_STANDARD 11)

# The version number.
set (SScan_VERSION_MAJOR 1)
set (SScan_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file (
#  "${PROJECT_SOURCE_DIR}/Resources"
#  "${PROJECT_BINARY_DIR}/Header"
#  )

# add the binary tree to the search path for include files
# so that we will find SScan.h
include_directories("${PROJECT_BINARY_DIR}")

set(SOURCES main.cpp Resources/ip.cpp Resources/Port.cpp Resources/sscan.cpp)

# add the executable
add_executable(SScan ${SOURCES})