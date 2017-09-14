cmake_minimum_required(VERSION 3.0)
project( libb C )

# Set include dirs
include_directories( "include" )

# Link to libmockc
include_directories( "${PROJECT_SOURCE_DIR}/libmockcb/include" )
link_directories( "${PROJECT_SOURCE_DIR}/libmockcb" )

# Build executable
add_library(${PROJECT_NAME} STATIC src/b.c)
target_link_libraries(${PROJECT_NAME} -L${PROJECT_SOURCE_DIR}/libmockcb liblibmockcb.a )