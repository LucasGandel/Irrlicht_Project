cmake_minimum_required(VERSION 2.6)

set( PROJECT_NAME Irrlicht-Project )
project( ${PROJECT_NAME} )

# Search for /usr/local/lib/libIrrlicht.a
find_library( IRRLICHT_LIBRARY NAMES Irrlicht
  PATHS $ENV{IRRDIR}/lib )

# Configure Project
set( CMAKE_BUILD_TYPE Debug )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( MEDIA_DIR "${CMAKE_SOURCE_DIR}/media/" )
configure_file( CMake/Project_Config.h.in Project_Config.h @ONLY )

# add_executable sources
file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.[ch]*)

# Include CMake Generated files ( ProjectConfig.h )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

# Specify g++ option
add_definitions( -Wall -Wextra -std=c++11 )

add_executable( ${PROJECT_NAME} ${SOURCES} ${HEADERS} )
target_link_libraries( ${PROJECT_NAME} ${IRRLICHT_LIBRARY} -lGL -lXxf86vm -lXext -lX11 -lXcursor )

