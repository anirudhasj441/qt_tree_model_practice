# Build file for CREATE-QT-APP

#
# Please read the instructions in the comments and make requisite configurations

cmake_minimum_required(VERSION 3.21)

find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
    message( NOTICE "CCache is available. Will be used." )
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()


# Set the name of the project here as first argument.
project(tree_model_demo)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(src)