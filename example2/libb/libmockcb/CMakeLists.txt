cmake_minimum_required(VERSION 3.0)
project( libmockcb C )

# Set include dirs
include_directories( "include" )

# Build executable
add_library(${PROJECT_NAME} STATIC src/mockc.c)