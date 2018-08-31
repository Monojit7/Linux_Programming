include_directories(../inc)
include_directories(../inc/lifecycle)
include_directories(autogencode/src-gen/consumes)

IF (WIN32)
	link_directories(./build/Debug)
ELSE()
	link_directories(./build)
ENDIF()

#add_subdirectory(autogencode .)	

add_executable(
CoreTest
CoreTest.cpp
)

target_link_libraries(
CoreTest
#keyprestest_keyprestest_types_static_proxy
CommonAPI
prescore
)

add_library(testplugin
SHARED 
testplugin.cpp
)