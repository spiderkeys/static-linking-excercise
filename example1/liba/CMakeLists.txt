cmake_minimum_required(VERSION 3.0)
project( liba C )

# Set include dirs
include_directories( "include" )

# Link to libmockc
include_directories( "${PROJECT_SOURCE_DIR}/libmockca/include" )
add_subdirectory( libmockca )

# Build executable
add_library(${PROJECT_NAME} STATIC src/a.c)
target_link_libraries(${PROJECT_NAME} libmockca "-Wl,-Map=output.map" )