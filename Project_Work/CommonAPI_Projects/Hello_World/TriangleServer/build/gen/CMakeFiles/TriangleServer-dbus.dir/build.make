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
CMAKE_COMMAND = /opt/elina/2.0.2015143A/sysroots/x86_64-elinasdk-linux/usr/bin/cmake

# The command to remove a file.
RM = /opt/elina/2.0.2015143A/sysroots/x86_64-elinasdk-linux/usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build

# Include any dependencies generated for this target.
include gen/CMakeFiles/TriangleServer-dbus.dir/depend.make

# Include the progress variables for this target.
include gen/CMakeFiles/TriangleServer-dbus.dir/progress.make

# Include the compile flags for this target's objects.
include gen/CMakeFiles/TriangleServer-dbus.dir/flags.make

gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o: gen/CMakeFiles/TriangleServer-dbus.dir/flags.make
gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o: ../gen/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o"
	cd /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build/gen && /opt/elina/2.0.2015143A/sysroots/x86_64-elinasdk-linux/usr/bin/arm-elina-linux-gnueabi/arm-elina-linux-gnueabi-g++   -march=armv7-a -marm  -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=/opt/elina/2.0.2015143A/sysroots/cortexa9hf-vfp-neon-elina-linux-gnueabi --sysroot=/opt/elina/2.0.2015143A/sysroots/cortexa9hf-vfp-neon-elina-linux-gnueabi/  $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o -c /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/gen/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp

gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.i"
	cd /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build/gen && /opt/elina/2.0.2015143A/sysroots/x86_64-elinasdk-linux/usr/bin/arm-elina-linux-gnueabi/arm-elina-linux-gnueabi-g++   -march=armv7-a -marm  -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=/opt/elina/2.0.2015143A/sysroots/cortexa9hf-vfp-neon-elina-linux-gnueabi --sysroot=/opt/elina/2.0.2015143A/sysroots/cortexa9hf-vfp-neon-elina-linux-gnueabi/ $(CXX_DEFINES) $(CXX_FLAGS) -E /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/gen/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp > CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.i

gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.s"
	cd /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build/gen && /opt/elina/2.0.2015143A/sysroots/x86_64-elinasdk-linux/usr/bin/arm-elina-linux-gnueabi/arm-elina-linux-gnueabi-g++   -march=armv7-a -marm  -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=/opt/elina/2.0.2015143A/sysroots/cortexa9hf-vfp-neon-elina-linux-gnueabi --sysroot=/opt/elina/2.0.2015143A/sysroots/cortexa9hf-vfp-neon-elina-linux-gnueabi/ $(CXX_DEFINES) $(CXX_FLAGS) -S /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/gen/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp -o CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.s

gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o.requires:

.PHONY : gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o.requires

gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o.provides: gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o.requires
	$(MAKE) -f gen/CMakeFiles/TriangleServer-dbus.dir/build.make gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o.provides.build
.PHONY : gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o.provides

gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o.provides.build: gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o


gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o: gen/CMakeFiles/TriangleServer-dbus.dir/flags.make
gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o: ../gen/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o"
	cd /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build/gen && /opt/elina/2.0.2015143A/sysroots/x86_64-elinasdk-linux/usr/bin/arm-elina-linux-gnueabi/arm-elina-linux-gnueabi-g++   -march=armv7-a -marm  -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=/opt/elina/2.0.2015143A/sysroots/cortexa9hf-vfp-neon-elina-linux-gnueabi --sysroot=/opt/elina/2.0.2015143A/sysroots/cortexa9hf-vfp-neon-elina-linux-gnueabi/  $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o -c /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/gen/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp

gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.i"
	cd /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build/gen && /opt/elina/2.0.2015143A/sysroots/x86_64-elinasdk-linux/usr/bin/arm-elina-linux-gnueabi/arm-elina-linux-gnueabi-g++   -march=armv7-a -marm  -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=/opt/elina/2.0.2015143A/sysroots/cortexa9hf-vfp-neon-elina-linux-gnueabi --sysroot=/opt/elina/2.0.2015143A/sysroots/cortexa9hf-vfp-neon-elina-linux-gnueabi/ $(CXX_DEFINES) $(CXX_FLAGS) -E /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/gen/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp > CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.i

gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.s"
	cd /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build/gen && /opt/elina/2.0.2015143A/sysroots/x86_64-elinasdk-linux/usr/bin/arm-elina-linux-gnueabi/arm-elina-linux-gnueabi-g++   -march=armv7-a -marm  -mthumb-interwork -mfloat-abi=hard -mfpu=neon -mtune=cortex-a9 --sysroot=/opt/elina/2.0.2015143A/sysroots/cortexa9hf-vfp-neon-elina-linux-gnueabi --sysroot=/opt/elina/2.0.2015143A/sysroots/cortexa9hf-vfp-neon-elina-linux-gnueabi/ $(CXX_DEFINES) $(CXX_FLAGS) -S /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/gen/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp -o CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.s

gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o.requires:

.PHONY : gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o.requires

gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o.provides: gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o.requires
	$(MAKE) -f gen/CMakeFiles/TriangleServer-dbus.dir/build.make gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o.provides.build
.PHONY : gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o.provides

gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o.provides.build: gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o


# Object files for target TriangleServer-dbus
TriangleServer__dbus_OBJECTS = \
"CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o" \
"CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o"

# External object files for target TriangleServer-dbus
TriangleServer__dbus_EXTERNAL_OBJECTS =

gen/libTriangleServer-dbus.so: gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o
gen/libTriangleServer-dbus.so: gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o
gen/libTriangleServer-dbus.so: gen/CMakeFiles/TriangleServer-dbus.dir/build.make
gen/libTriangleServer-dbus.so: gen/CMakeFiles/TriangleServer-dbus.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX shared library libTriangleServer-dbus.so"
	cd /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build/gen && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TriangleServer-dbus.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
gen/CMakeFiles/TriangleServer-dbus.dir/build: gen/libTriangleServer-dbus.so

.PHONY : gen/CMakeFiles/TriangleServer-dbus.dir/build

gen/CMakeFiles/TriangleServer-dbus.dir/requires: gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusDeployment.cpp.o.requires
gen/CMakeFiles/TriangleServer-dbus.dir/requires: gen/CMakeFiles/TriangleServer-dbus.dir/dbus/provides/v1/com/monojit/TriangleDBusStubAdapter.cpp.o.requires

.PHONY : gen/CMakeFiles/TriangleServer-dbus.dir/requires

gen/CMakeFiles/TriangleServer-dbus.dir/clean:
	cd /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build/gen && $(CMAKE_COMMAND) -P CMakeFiles/TriangleServer-dbus.dir/cmake_clean.cmake
.PHONY : gen/CMakeFiles/TriangleServer-dbus.dir/clean

gen/CMakeFiles/TriangleServer-dbus.dir/depend:
	cd /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/gen /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build/gen /home/monojit/FORD/Linux_Programming/CommonAPI/Hello_World/TriangleServer/build/gen/CMakeFiles/TriangleServer-dbus.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : gen/CMakeFiles/TriangleServer-dbus.dir/depend

