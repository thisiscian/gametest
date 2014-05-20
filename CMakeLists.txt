cmake_minimum_required (VERSION 2.6)
project (Game)
# The version number.
set (Game_VERSION_MAJOR 0)
set (Game_VERSION_MINOR 0)
set (CMAKE_CXX_FLAGS "-lboost_program_options")
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/headers/GameConfig.h.in"
  "${PROJECT_BINARY_DIR}/GameConfig.h"
)
 
# add the binary tree to the search path for include files
# so that we will find GameConfig.h
include_directories("${PROJECT_BINARY_DIR}")
 
# add the executable
add_executable(Game src/game.cpp)
