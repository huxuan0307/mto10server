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
include src/CMakeFiles/tools.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/tools.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/tools.dir/flags.make

src/CMakeFiles/tools.dir/read_config.cpp.o: src/CMakeFiles/tools.dir/flags.make
src/CMakeFiles/tools.dir/read_config.cpp.o: src/read_config.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ComputerNetwork/project/mto10server/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/tools.dir/read_config.cpp.o"
	cd /root/ComputerNetwork/project/mto10server/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/tools.dir/read_config.cpp.o -c /root/ComputerNetwork/project/mto10server/src/read_config.cpp

src/CMakeFiles/tools.dir/read_config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tools.dir/read_config.cpp.i"
	cd /root/ComputerNetwork/project/mto10server/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/ComputerNetwork/project/mto10server/src/read_config.cpp > CMakeFiles/tools.dir/read_config.cpp.i

src/CMakeFiles/tools.dir/read_config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tools.dir/read_config.cpp.s"
	cd /root/ComputerNetwork/project/mto10server/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/ComputerNetwork/project/mto10server/src/read_config.cpp -o CMakeFiles/tools.dir/read_config.cpp.s

src/CMakeFiles/tools.dir/read_config.cpp.o.requires:
.PHONY : src/CMakeFiles/tools.dir/read_config.cpp.o.requires

src/CMakeFiles/tools.dir/read_config.cpp.o.provides: src/CMakeFiles/tools.dir/read_config.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/tools.dir/build.make src/CMakeFiles/tools.dir/read_config.cpp.o.provides.build
.PHONY : src/CMakeFiles/tools.dir/read_config.cpp.o.provides

src/CMakeFiles/tools.dir/read_config.cpp.o.provides.build: src/CMakeFiles/tools.dir/read_config.cpp.o

src/CMakeFiles/tools.dir/logging.cpp.o: src/CMakeFiles/tools.dir/flags.make
src/CMakeFiles/tools.dir/logging.cpp.o: src/logging.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ComputerNetwork/project/mto10server/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/tools.dir/logging.cpp.o"
	cd /root/ComputerNetwork/project/mto10server/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/tools.dir/logging.cpp.o -c /root/ComputerNetwork/project/mto10server/src/logging.cpp

src/CMakeFiles/tools.dir/logging.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tools.dir/logging.cpp.i"
	cd /root/ComputerNetwork/project/mto10server/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/ComputerNetwork/project/mto10server/src/logging.cpp > CMakeFiles/tools.dir/logging.cpp.i

src/CMakeFiles/tools.dir/logging.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tools.dir/logging.cpp.s"
	cd /root/ComputerNetwork/project/mto10server/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/ComputerNetwork/project/mto10server/src/logging.cpp -o CMakeFiles/tools.dir/logging.cpp.s

src/CMakeFiles/tools.dir/logging.cpp.o.requires:
.PHONY : src/CMakeFiles/tools.dir/logging.cpp.o.requires

src/CMakeFiles/tools.dir/logging.cpp.o.provides: src/CMakeFiles/tools.dir/logging.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/tools.dir/build.make src/CMakeFiles/tools.dir/logging.cpp.o.provides.build
.PHONY : src/CMakeFiles/tools.dir/logging.cpp.o.provides

src/CMakeFiles/tools.dir/logging.cpp.o.provides.build: src/CMakeFiles/tools.dir/logging.cpp.o

src/CMakeFiles/tools.dir/argument.cpp.o: src/CMakeFiles/tools.dir/flags.make
src/CMakeFiles/tools.dir/argument.cpp.o: src/argument.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /root/ComputerNetwork/project/mto10server/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src/CMakeFiles/tools.dir/argument.cpp.o"
	cd /root/ComputerNetwork/project/mto10server/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/tools.dir/argument.cpp.o -c /root/ComputerNetwork/project/mto10server/src/argument.cpp

src/CMakeFiles/tools.dir/argument.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tools.dir/argument.cpp.i"
	cd /root/ComputerNetwork/project/mto10server/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/ComputerNetwork/project/mto10server/src/argument.cpp > CMakeFiles/tools.dir/argument.cpp.i

src/CMakeFiles/tools.dir/argument.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tools.dir/argument.cpp.s"
	cd /root/ComputerNetwork/project/mto10server/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/ComputerNetwork/project/mto10server/src/argument.cpp -o CMakeFiles/tools.dir/argument.cpp.s

src/CMakeFiles/tools.dir/argument.cpp.o.requires:
.PHONY : src/CMakeFiles/tools.dir/argument.cpp.o.requires

src/CMakeFiles/tools.dir/argument.cpp.o.provides: src/CMakeFiles/tools.dir/argument.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/tools.dir/build.make src/CMakeFiles/tools.dir/argument.cpp.o.provides.build
.PHONY : src/CMakeFiles/tools.dir/argument.cpp.o.provides

src/CMakeFiles/tools.dir/argument.cpp.o.provides.build: src/CMakeFiles/tools.dir/argument.cpp.o

# Object files for target tools
tools_OBJECTS = \
"CMakeFiles/tools.dir/read_config.cpp.o" \
"CMakeFiles/tools.dir/logging.cpp.o" \
"CMakeFiles/tools.dir/argument.cpp.o"

# External object files for target tools
tools_EXTERNAL_OBJECTS =

src/libtools.a: src/CMakeFiles/tools.dir/read_config.cpp.o
src/libtools.a: src/CMakeFiles/tools.dir/logging.cpp.o
src/libtools.a: src/CMakeFiles/tools.dir/argument.cpp.o
src/libtools.a: src/CMakeFiles/tools.dir/build.make
src/libtools.a: src/CMakeFiles/tools.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libtools.a"
	cd /root/ComputerNetwork/project/mto10server/src && $(CMAKE_COMMAND) -P CMakeFiles/tools.dir/cmake_clean_target.cmake
	cd /root/ComputerNetwork/project/mto10server/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tools.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/tools.dir/build: src/libtools.a
.PHONY : src/CMakeFiles/tools.dir/build

src/CMakeFiles/tools.dir/requires: src/CMakeFiles/tools.dir/read_config.cpp.o.requires
src/CMakeFiles/tools.dir/requires: src/CMakeFiles/tools.dir/logging.cpp.o.requires
src/CMakeFiles/tools.dir/requires: src/CMakeFiles/tools.dir/argument.cpp.o.requires
.PHONY : src/CMakeFiles/tools.dir/requires

src/CMakeFiles/tools.dir/clean:
	cd /root/ComputerNetwork/project/mto10server/src && $(CMAKE_COMMAND) -P CMakeFiles/tools.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/tools.dir/clean

src/CMakeFiles/tools.dir/depend:
	cd /root/ComputerNetwork/project/mto10server && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ComputerNetwork/project/mto10server /root/ComputerNetwork/project/mto10server/src /root/ComputerNetwork/project/mto10server /root/ComputerNetwork/project/mto10server/src /root/ComputerNetwork/project/mto10server/src/CMakeFiles/tools.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/tools.dir/depend

