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
CMAKE_SOURCE_DIR = /root/ComputerNetwork/project/mto10server/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/ComputerNetwork/project/mto10server/test

# Include any dependencies generated for this target.
include CMakeFiles/distro_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/distro_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/distro_test.dir/flags.make

CMakeFiles/distro_test.dir/distro_test.cpp.o: CMakeFiles/distro_test.dir/flags.make
CMakeFiles/distro_test.dir/distro_test.cpp.o: distro_test.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ComputerNetwork/project/mto10server/test/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/distro_test.dir/distro_test.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/distro_test.dir/distro_test.cpp.o -c /root/ComputerNetwork/project/mto10server/test/distro_test.cpp

CMakeFiles/distro_test.dir/distro_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/distro_test.dir/distro_test.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/ComputerNetwork/project/mto10server/test/distro_test.cpp > CMakeFiles/distro_test.dir/distro_test.cpp.i

CMakeFiles/distro_test.dir/distro_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/distro_test.dir/distro_test.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/ComputerNetwork/project/mto10server/test/distro_test.cpp -o CMakeFiles/distro_test.dir/distro_test.cpp.s

CMakeFiles/distro_test.dir/distro_test.cpp.o.requires:
.PHONY : CMakeFiles/distro_test.dir/distro_test.cpp.o.requires

CMakeFiles/distro_test.dir/distro_test.cpp.o.provides: CMakeFiles/distro_test.dir/distro_test.cpp.o.requires
	$(MAKE) -f CMakeFiles/distro_test.dir/build.make CMakeFiles/distro_test.dir/distro_test.cpp.o.provides.build
.PHONY : CMakeFiles/distro_test.dir/distro_test.cpp.o.provides

CMakeFiles/distro_test.dir/distro_test.cpp.o.provides.build: CMakeFiles/distro_test.dir/distro_test.cpp.o

# Object files for target distro_test
distro_test_OBJECTS = \
"CMakeFiles/distro_test.dir/distro_test.cpp.o"

# External object files for target distro_test
distro_test_EXTERNAL_OBJECTS =

distro_test: CMakeFiles/distro_test.dir/distro_test.cpp.o
distro_test: CMakeFiles/distro_test.dir/build.make
distro_test: CMakeFiles/distro_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable distro_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/distro_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/distro_test.dir/build: distro_test
.PHONY : CMakeFiles/distro_test.dir/build

CMakeFiles/distro_test.dir/requires: CMakeFiles/distro_test.dir/distro_test.cpp.o.requires
.PHONY : CMakeFiles/distro_test.dir/requires

CMakeFiles/distro_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/distro_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/distro_test.dir/clean

CMakeFiles/distro_test.dir/depend:
	cd /root/ComputerNetwork/project/mto10server/test && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ComputerNetwork/project/mto10server/test /root/ComputerNetwork/project/mto10server/test /root/ComputerNetwork/project/mto10server/test /root/ComputerNetwork/project/mto10server/test /root/ComputerNetwork/project/mto10server/test/CMakeFiles/distro_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/distro_test.dir/depend

