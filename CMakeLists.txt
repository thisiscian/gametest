cmake_minimum_required (VERSION 2.8)
project (GAMETEST)
# The version number.
set (Game_VERSION_MAJOR 0)
set (Game_VERSION_MINOR 0)
set (Game_TITLE "${PROJECT_NAME}")
set (CMAKE_CXX_FLAGS "-O2 -lboost_program_options -lglfw -lGL -lGLEW")

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
  "${PROJECT_SOURCE_DIR}/include/settings.h.in"
  "${PROJECT_SOURCE_DIR}/include/settings.h"
)

# add the binary tree to the search path for include files
# so that we will find GameConfig.h
include_directories("${PROJECT_SOURCE_DIR}/include")

# add the executable
add_executable(
	${PROJECT_NAME}
	src/game.cpp
	src/misc.cpp
	src/drawer.cpp
	src/shader.cpp
	src/settings.cpp
)
