# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.3

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/yoctoadm/work/FORDSDK/Ford-SDK-17155/sysroots/x86_64-elinasdk-linux/usr/bin/cmake

# The command to remove a file.
RM = /home/yoctoadm/work/FORDSDK/Ford-SDK-17155/sysroots/x86_64-elinasdk-linux/usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yoctoadm/work/customerOptIn_srcV2/customeroptin

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yoctoadm/work/customerOptIn_srcV2/customeroptin/build

# Utility rule file for cleanall.

# Include the progress variables for this target.
include CMakeFiles/cleanall.dir/progress.make

CMakeFiles/cleanall:
	cmake -E remove_directory /home/yoctoadm/work/customerOptIn_srcV2/customeroptin/gen
	cmake -E remove_directory /home/yoctoadm/work/customerOptIn_srcV2/customeroptin/src-gen

cleanall: CMakeFiles/cleanall
cleanall: CMakeFiles/cleanall.dir/build.make

.PHONY : cleanall

# Rule to build all files generated by this target.
CMakeFiles/cleanall.dir/build: cleanall

.PHONY : CMakeFiles/cleanall.dir/build

CMakeFiles/cleanall.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cleanall.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cleanall.dir/clean

CMakeFiles/cleanall.dir/depend:
	cd /home/yoctoadm/work/customerOptIn_srcV2/customeroptin/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yoctoadm/work/customerOptIn_srcV2/customeroptin /home/yoctoadm/work/customerOptIn_srcV2/customeroptin /home/yoctoadm/work/customerOptIn_srcV2/customeroptin/build /home/yoctoadm/work/customerOptIn_srcV2/customeroptin/build /home/yoctoadm/work/customerOptIn_srcV2/customeroptin/build/CMakeFiles/cleanall.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cleanall.dir/depend
