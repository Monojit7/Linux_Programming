# Presentation Control root CMake
# 20th June 2016. 

cmake_minimum_required(VERSION 2.6)

#Ensure library paths are handled correctly. 
cmake_policy(SET CMP0015 NEW)


include_directories(./inc)

IF (WIN32)
  include_directories(../deliveries/commonapi/include)
  include_directories(../deliveries/dbus/include)
  add_definitions(-D PLATFORM_WIN32 -D WIN32_LEAN_AND_MEAN)
ELSE()
  IF (NOT DEFINED CMAKE_SYSROOT) 
	message(FATAL_ERROR "CMAKE_SYSROOT not defined ! can't proceed")
  ENDIF()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -std=c++11") 
  include_directories(${CMAKE_SYSROOT}/usr/include)
  include_directories(${CMAKE_SYSROOT}/usr/include/CommonAPI-3.1)
  include_directories(${CMAKE_SYSROOT}/usr/include/dbus-1.0)
  include_directories(${CMAKE_SYSROOT}/usr/lib/dbus-1.0/include)  
  include_directories(${CMAKE_SYSROOT}/usr/lib64/dbus-1.0/include)
ENDIF()


IF (WIN32)
	link_directories(../deploy/win/lib)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj -Wall -std=c++11")
ELSE()
	link_directories(${CMAKE_SYSROOT}/usr/lib)
ENDIF()

add_subdirectory(corelib .)
add_subdirectory(CoreTest .)
