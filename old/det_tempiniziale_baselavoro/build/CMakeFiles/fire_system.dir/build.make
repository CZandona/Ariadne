# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/d/GithubRepo/AriadneGit/det_tempiniziale

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/GithubRepo/AriadneGit/det_tempiniziale/build

# Include any dependencies generated for this target.
include CMakeFiles/fire_system.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fire_system.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fire_system.dir/flags.make

CMakeFiles/fire_system.dir/fire.cpp.o: CMakeFiles/fire_system.dir/flags.make
CMakeFiles/fire_system.dir/fire.cpp.o: ../fire.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/GithubRepo/AriadneGit/det_tempiniziale/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fire_system.dir/fire.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fire_system.dir/fire.cpp.o -c /mnt/d/GithubRepo/AriadneGit/det_tempiniziale/fire.cpp

CMakeFiles/fire_system.dir/fire.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fire_system.dir/fire.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/GithubRepo/AriadneGit/det_tempiniziale/fire.cpp > CMakeFiles/fire_system.dir/fire.cpp.i

CMakeFiles/fire_system.dir/fire.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fire_system.dir/fire.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/GithubRepo/AriadneGit/det_tempiniziale/fire.cpp -o CMakeFiles/fire_system.dir/fire.cpp.s

CMakeFiles/fire_system.dir/fire.cpp.o.requires:

.PHONY : CMakeFiles/fire_system.dir/fire.cpp.o.requires

CMakeFiles/fire_system.dir/fire.cpp.o.provides: CMakeFiles/fire_system.dir/fire.cpp.o.requires
	$(MAKE) -f CMakeFiles/fire_system.dir/build.make CMakeFiles/fire_system.dir/fire.cpp.o.provides.build
.PHONY : CMakeFiles/fire_system.dir/fire.cpp.o.provides

CMakeFiles/fire_system.dir/fire.cpp.o.provides.build: CMakeFiles/fire_system.dir/fire.cpp.o


# Object files for target fire_system
fire_system_OBJECTS = \
"CMakeFiles/fire_system.dir/fire.cpp.o"

# External object files for target fire_system
fire_system_EXTERNAL_OBJECTS =

fire_system: CMakeFiles/fire_system.dir/fire.cpp.o
fire_system: CMakeFiles/fire_system.dir/build.make
fire_system: /usr/local/lib/libariadne.so
fire_system: CMakeFiles/fire_system.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/GithubRepo/AriadneGit/det_tempiniziale/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable fire_system"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fire_system.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fire_system.dir/build: fire_system

.PHONY : CMakeFiles/fire_system.dir/build

CMakeFiles/fire_system.dir/requires: CMakeFiles/fire_system.dir/fire.cpp.o.requires

.PHONY : CMakeFiles/fire_system.dir/requires

CMakeFiles/fire_system.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fire_system.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fire_system.dir/clean

CMakeFiles/fire_system.dir/depend:
	cd /mnt/d/GithubRepo/AriadneGit/det_tempiniziale/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/GithubRepo/AriadneGit/det_tempiniziale /mnt/d/GithubRepo/AriadneGit/det_tempiniziale /mnt/d/GithubRepo/AriadneGit/det_tempiniziale/build /mnt/d/GithubRepo/AriadneGit/det_tempiniziale/build /mnt/d/GithubRepo/AriadneGit/det_tempiniziale/build/CMakeFiles/fire_system.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fire_system.dir/depend
