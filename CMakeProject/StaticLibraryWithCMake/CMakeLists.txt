cmake_minimum_required(VERSION 3.0.2)

## The set(CMAKE_BUILD_TYPE Release) function is used to set the build type to be a release build.

set(CMAKE_BUILD_TYPE debug)
## Project Name
project(Hello_World_Folder_Structures)

## The include_directories() function is used to bring the header files into the build environment.
include_directories(./include)

## The file() command is used to add the source files to the project. GLOB (or GLOB_RECURSE) is used to create a list of all of the files that meet the globbing expression (i.e., “src/*.cpp“) and add them to a variable SOURCES.

file( GLOB SOURCES "src/*.cpp" )


## Instead of the add_executable() function that is used in previous examples, this example uses the add_library() function. The library is built as a shared library using the SHARED flag (other options are: STATIC or MODULE) , and the Hello_World name is used as the name of the shared library

add_library(Hello_World STATIC ${SOURCES} )

## to make sure the installation location

install (TARGETS Hello_World DESTINATION ../library/usr/lib )


