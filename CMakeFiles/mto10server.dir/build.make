# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/ComputerNetwork/project/mto10server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/ComputerNetwork/project/mto10server

# Include any dependencies generated for this target.
include CMakeFiles/mto10server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mto10server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mto10server.dir/flags.make

CMakeFiles/mto10server.dir/main.cpp.o: CMakeFiles/mto10server.dir/flags.make
CMakeFiles/mto10server.dir/main.cpp.o: main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ComputerNetwork/project/mto10server/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/mto10server.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/mto10server.dir/main.cpp.o -c /root/ComputerNetwork/project/mto10server/main.cpp

CMakeFiles/mto10server.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mto10server.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/ComputerNetwork/project/mto10server/main.cpp > CMakeFiles/mto10server.dir/main.cpp.i

CMakeFiles/mto10server.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mto10server.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/ComputerNetwork/project/mto10server/main.cpp -o CMakeFiles/mto10server.dir/main.cpp.s

CMakeFiles/mto10server.dir/main.cpp.o.requires:
.PHONY : CMakeFiles/mto10server.dir/main.cpp.o.requires

CMakeFiles/mto10server.dir/main.cpp.o.provides: CMakeFiles/mto10server.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/mto10server.dir/build.make CMakeFiles/mto10server.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/mto10server.dir/main.cpp.o.provides

CMakeFiles/mto10server.dir/main.cpp.o.provides.build: CMakeFiles/mto10server.dir/main.cpp.o

# Object files for target mto10server
mto10server_OBJECTS = \
"CMakeFiles/mto10server.dir/main.cpp.o"

# External object files for target mto10server
mto10server_EXTERNAL_OBJECTS =

mto10server: CMakeFiles/mto10server.dir/main.cpp.o
mto10server: CMakeFiles/mto10server.dir/build.make
mto10server: src/libmd5.a
mto10server: src/libdatabase.a
mto10server: src/libtools.a
mto10server: src/libserver.a
mto10server: TCP/libTcpServer.a
mto10server: src/libdatabase.a
mto10server: src/libtools.a
mto10server: CMakeFiles/mto10server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable mto10server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mto10server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mto10server.dir/build: mto10server
.PHONY : CMakeFiles/mto10server.dir/build

CMakeFiles/mto10server.dir/requires: CMakeFiles/mto10server.dir/main.cpp.o.requires
.PHONY : CMakeFiles/mto10server.dir/requires

CMakeFiles/mto10server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mto10server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mto10server.dir/clean

CMakeFiles/mto10server.dir/depend:
	cd /root/ComputerNetwork/project/mto10server && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ComputerNetwork/project/mto10server /root/ComputerNetwork/project/mto10server /root/ComputerNetwork/project/mto10server /root/ComputerNetwork/project/mto10server /root/ComputerNetwork/project/mto10server/CMakeFiles/mto10server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mto10server.dir/depend
