include_directories(autogencode/gen/dbus/consumes)
include_directories(../inc/lifecycle)

add_subdirectory(autogencode)

file(GLOB TMP_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp	
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/core/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/lifecycle/v2/org/genivi/NodeStateManager/*.hpp	
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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -fPIC")
ENDIF()

add_library(prescore
SHARED
${TMP_FILES}
)

IF (WIN32)
	target_link_libraries(prescore CommonAPI ws2_32)
ELSE()
	target_link_libraries(prescore CommonAPI dlt rt persistence_client_library dl systemd)
ENDIF()

add_dependencies(prescore nsmconsumer-dbus)
