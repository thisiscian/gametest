cmake_minimum_required (VERSION 2.6)
project (Game)
# The version number.
set (Game_VERSION_MAJOR 0)
set (Game_VERSION_MINOR 0)
set (Game_TITLE "\"GAMETEST\"")
set (CMAKE_CXX_FLAGS "-lboost_program_options -lglfw -lGL -lGLEW")

find_package(OpenGL)
if(OPENGL_FOUND) 
	message("Game: found opengl: ${OPENGL_INCLUDE_DIR}")
	include_directories(${OPENGL_INCLUDE_DIR})
else (OPENGL_FOUND)
	message("Game: could not find opengl")
endif(OPENGL_FOUND)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/include/GameConfig.h.in"
  "${PROJECT_BINARY_DIR}/GameConfig.h"
)

# add the binary tree to the search path for include files
# so that we will find GameConfig.h
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")
 
# add the executable
add_executable(Game
	src/game.cpp
	src/shader.cpp
	shaders/simplevert.sdr
	shaders/simplefrag.sdr
)
