include_directories(autogencode/src-gen/consumes)
include_directories(autogencode/src-gen/provides)

include_directories(./remotecdcomponent/includes)
include_directories(./remotecdcomponent/includes/browse)
include_directories(./remotecdcomponent/includes/cache)
include_directories(./remotecdcomponent/includes/util)


add_subdirectory(autogencode .)
include_directories(.)

file(GLOB TMP_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/audiomanagerinterface/*
	${CMAKE_CURRENT_SOURCE_DIR}/browseengine/*
	${CMAKE_CURRENT_SOURCE_DIR}/browseengine/session/*
	${CMAKE_CURRENT_SOURCE_DIR}/config/*
	${CMAKE_CURRENT_SOURCE_DIR}/detectionengine/*
	${CMAKE_CURRENT_SOURCE_DIR}/device/*
    ${CMAKE_CURRENT_SOURCE_DIR}/playengine/*	
    ${CMAKE_CURRENT_SOURCE_DIR}/statemachine/*	
    ${CMAKE_CURRENT_SOURCE_DIR}/remotecdcomponent/includes/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/remotecdcomponent/includes/browse/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/remotecdcomponent/includes/cache/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/remotecdcomponent/includes/util/*.hpp
)


#For generating grouping in VS 2015 Solution Files. 
IF (WIN32)
foreach(FILE ${TMP_FILES}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    # Group into "Source Files" and "Header Files"
    if ("${FILE}" MATCHES ".*\\.cpp")
       set(GROUP "Source Files${GROUP}")
    elseif("${FILE}" MATCHES ".*\\.h")
       set(GROUP "Header Files${GROUP}")
	elseif("${FILE}" MATCHES ".*\\.hpp")
       set(GROUP "Header Files${GROUP}")   
    endif()

    source_group("${GROUP}" FILES "${FILE}")
endforeach()
ELSEIF (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall  -std=c++11 -g -ggdb -feliminate-unused-debug-symbols -rdynamic -ggdb  -fPIC")
ENDIF()

add_executable(
MediaPresCtrl
${TMP_FILES}
)

target_link_libraries(
MediaPresCtrl
mmpres_mmpres_types_static_stub
prescore
CommonAPI
jsoncpp
persistence_client_library
ambtpclient
dlt

)

IF (UNIX)
target_link_libraries(
MediaPresCtrl
dl
systemd
)
ENDIF()

add_dependencies(MediaPresCtrl mmpres-dbus)
