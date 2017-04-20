cmake_minimum_required(VERSION 3.0.2)

## Project Name
project(Hello_World_Folder_Structures)

## The include_directories() function is used to bring the header files into the build environment.
include_directories(./include)

## The file() command is used to add the source files to the project. GLOB (or GLOB_RECURSE) is used to create a list of all of the files that meet the globbing expression (i.e., “src/*.cpp“) and add them to a variable SOURCES.

file( GLOB SOURCES "src/*.cpp" )


## The add_executable() function uses the SOURCES variable, rather than an explicit reference to each source file, in order to build the testStudent executable program.

add_executable(Hello_World ${SOURCES})


