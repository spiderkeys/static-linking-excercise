cmake_minimum_required(VERSION 3.0)
project( statlink C )

# Link to lib_a
include_directories( "${PROJECT_SOURCE_DIR}/liba/include" )
add_subdirectory( liba )

# Link to lib_b
include_directories( "${PROJECT_SOURCE_DIR}/libb/include" )
add_subdirectory( libb )

# Build executable
add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} liba libb )