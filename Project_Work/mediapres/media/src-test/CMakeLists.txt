include_directories(autogencode/src-gen/consumes)
add_subdirectory(autogencode .)

add_executable(
MediaPresCtrlTestCLI
main.cpp
)

target_link_libraries(
MediaPresCtrlTestCLI
CommonAPI
)
