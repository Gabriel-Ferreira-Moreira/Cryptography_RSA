# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/ProjetoVS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/ProjetoVS/build

# Include any dependencies generated for this target.
include CMakeFiles/ExpoM.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ExpoM.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ExpoM.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ExpoM.dir/flags.make

CMakeFiles/ExpoM.dir/ExpoM.cpp.o: CMakeFiles/ExpoM.dir/flags.make
CMakeFiles/ExpoM.dir/ExpoM.cpp.o: ../ExpoM.cpp
CMakeFiles/ExpoM.dir/ExpoM.cpp.o: CMakeFiles/ExpoM.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/ProjetoVS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ExpoM.dir/ExpoM.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ExpoM.dir/ExpoM.cpp.o -MF CMakeFiles/ExpoM.dir/ExpoM.cpp.o.d -o CMakeFiles/ExpoM.dir/ExpoM.cpp.o -c /mnt/c/ProjetoVS/ExpoM.cpp

CMakeFiles/ExpoM.dir/ExpoM.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ExpoM.dir/ExpoM.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/ProjetoVS/ExpoM.cpp > CMakeFiles/ExpoM.dir/ExpoM.cpp.i

CMakeFiles/ExpoM.dir/ExpoM.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ExpoM.dir/ExpoM.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/ProjetoVS/ExpoM.cpp -o CMakeFiles/ExpoM.dir/ExpoM.cpp.s

# Object files for target ExpoM
ExpoM_OBJECTS = \
"CMakeFiles/ExpoM.dir/ExpoM.cpp.o"

# External object files for target ExpoM
ExpoM_EXTERNAL_OBJECTS =

ExpoM: CMakeFiles/ExpoM.dir/ExpoM.cpp.o
ExpoM: CMakeFiles/ExpoM.dir/build.make
ExpoM: CMakeFiles/ExpoM.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/ProjetoVS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ExpoM"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ExpoM.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ExpoM.dir/build: ExpoM
.PHONY : CMakeFiles/ExpoM.dir/build

CMakeFiles/ExpoM.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ExpoM.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ExpoM.dir/clean

CMakeFiles/ExpoM.dir/depend:
	cd /mnt/c/ProjetoVS/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/ProjetoVS /mnt/c/ProjetoVS /mnt/c/ProjetoVS/build /mnt/c/ProjetoVS/build /mnt/c/ProjetoVS/build/CMakeFiles/ExpoM.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ExpoM.dir/depend
