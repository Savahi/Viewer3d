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
CMAKE_COMMAND = /home/user/Desktop/install/cmake/bin/cmake

# The command to remove a file.
RM = /home/user/Desktop/install/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/user/Desktop/spider/Viewer3d

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/Desktop/spider/Viewer3d/build

# Include any dependencies generated for this target.
include CMakeFiles/Viewer3d.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Viewer3d.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Viewer3d.dir/flags.make

CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o: CMakeFiles/Viewer3d.dir/flags.make
CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o: ../src/Helpers.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o -c /home/user/Desktop/spider/Viewer3d/src/Helpers.cpp

CMakeFiles/Viewer3d.dir/src/Helpers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Viewer3d.dir/src/Helpers.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/spider/Viewer3d/src/Helpers.cpp > CMakeFiles/Viewer3d.dir/src/Helpers.cpp.i

CMakeFiles/Viewer3d.dir/src/Helpers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Viewer3d.dir/src/Helpers.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/spider/Viewer3d/src/Helpers.cpp -o CMakeFiles/Viewer3d.dir/src/Helpers.cpp.s

CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o.requires:

.PHONY : CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o.requires

CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o.provides: CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o.requires
	$(MAKE) -f CMakeFiles/Viewer3d.dir/build.make CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o.provides.build
.PHONY : CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o.provides

CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o.provides.build: CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o


CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o: CMakeFiles/Viewer3d.dir/flags.make
CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o: ../src/Model3d.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o -c /home/user/Desktop/spider/Viewer3d/src/Model3d.cpp

CMakeFiles/Viewer3d.dir/src/Model3d.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Viewer3d.dir/src/Model3d.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/spider/Viewer3d/src/Model3d.cpp > CMakeFiles/Viewer3d.dir/src/Model3d.cpp.i

CMakeFiles/Viewer3d.dir/src/Model3d.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Viewer3d.dir/src/Model3d.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/spider/Viewer3d/src/Model3d.cpp -o CMakeFiles/Viewer3d.dir/src/Model3d.cpp.s

CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o.requires:

.PHONY : CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o.requires

CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o.provides: CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o.requires
	$(MAKE) -f CMakeFiles/Viewer3d.dir/build.make CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o.provides.build
.PHONY : CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o.provides

CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o.provides.build: CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o


CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o: CMakeFiles/Viewer3d.dir/flags.make
CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o: ../src/Viewer3d.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o -c /home/user/Desktop/spider/Viewer3d/src/Viewer3d.cpp

CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/spider/Viewer3d/src/Viewer3d.cpp > CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.i

CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/spider/Viewer3d/src/Viewer3d.cpp -o CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.s

CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o.requires:

.PHONY : CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o.requires

CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o.provides: CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o.requires
	$(MAKE) -f CMakeFiles/Viewer3d.dir/build.make CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o.provides.build
.PHONY : CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o.provides

CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o.provides.build: CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o


# Object files for target Viewer3d
Viewer3d_OBJECTS = \
"CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o" \
"CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o" \
"CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o"

# External object files for target Viewer3d
Viewer3d_EXTERNAL_OBJECTS =

Viewer3d: CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o
Viewer3d: CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o
Viewer3d: CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o
Viewer3d: CMakeFiles/Viewer3d.dir/build.make
Viewer3d: /usr/lib/x86_64-linux-gnu/libglut.so
Viewer3d: /usr/lib/x86_64-linux-gnu/libGL.so
Viewer3d: /usr/lib/x86_64-linux-gnu/libGLU.so
Viewer3d: CMakeFiles/Viewer3d.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable Viewer3d"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Viewer3d.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Viewer3d.dir/build: Viewer3d

.PHONY : CMakeFiles/Viewer3d.dir/build

CMakeFiles/Viewer3d.dir/requires: CMakeFiles/Viewer3d.dir/src/Helpers.cpp.o.requires
CMakeFiles/Viewer3d.dir/requires: CMakeFiles/Viewer3d.dir/src/Model3d.cpp.o.requires
CMakeFiles/Viewer3d.dir/requires: CMakeFiles/Viewer3d.dir/src/Viewer3d.cpp.o.requires

.PHONY : CMakeFiles/Viewer3d.dir/requires

CMakeFiles/Viewer3d.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Viewer3d.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Viewer3d.dir/clean

CMakeFiles/Viewer3d.dir/depend:
	cd /home/user/Desktop/spider/Viewer3d/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/Desktop/spider/Viewer3d /home/user/Desktop/spider/Viewer3d /home/user/Desktop/spider/Viewer3d/build /home/user/Desktop/spider/Viewer3d/build /home/user/Desktop/spider/Viewer3d/build/CMakeFiles/Viewer3d.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Viewer3d.dir/depend
