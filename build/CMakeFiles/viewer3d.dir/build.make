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
include CMakeFiles/viewer3d.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/viewer3d.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/viewer3d.dir/flags.make

CMakeFiles/viewer3d.dir/src/display.cpp.o: CMakeFiles/viewer3d.dir/flags.make
CMakeFiles/viewer3d.dir/src/display.cpp.o: ../src/display.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/viewer3d.dir/src/display.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer3d.dir/src/display.cpp.o -c /home/user/Desktop/spider/Viewer3d/src/display.cpp

CMakeFiles/viewer3d.dir/src/display.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer3d.dir/src/display.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/spider/Viewer3d/src/display.cpp > CMakeFiles/viewer3d.dir/src/display.cpp.i

CMakeFiles/viewer3d.dir/src/display.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer3d.dir/src/display.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/spider/Viewer3d/src/display.cpp -o CMakeFiles/viewer3d.dir/src/display.cpp.s

CMakeFiles/viewer3d.dir/src/display.cpp.o.requires:

.PHONY : CMakeFiles/viewer3d.dir/src/display.cpp.o.requires

CMakeFiles/viewer3d.dir/src/display.cpp.o.provides: CMakeFiles/viewer3d.dir/src/display.cpp.o.requires
	$(MAKE) -f CMakeFiles/viewer3d.dir/build.make CMakeFiles/viewer3d.dir/src/display.cpp.o.provides.build
.PHONY : CMakeFiles/viewer3d.dir/src/display.cpp.o.provides

CMakeFiles/viewer3d.dir/src/display.cpp.o.provides.build: CMakeFiles/viewer3d.dir/src/display.cpp.o


CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o: CMakeFiles/viewer3d.dir/flags.make
CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o: ../src/displayfacet.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o -c /home/user/Desktop/spider/Viewer3d/src/displayfacet.cpp

CMakeFiles/viewer3d.dir/src/displayfacet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer3d.dir/src/displayfacet.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/spider/Viewer3d/src/displayfacet.cpp > CMakeFiles/viewer3d.dir/src/displayfacet.cpp.i

CMakeFiles/viewer3d.dir/src/displayfacet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer3d.dir/src/displayfacet.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/spider/Viewer3d/src/displayfacet.cpp -o CMakeFiles/viewer3d.dir/src/displayfacet.cpp.s

CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o.requires:

.PHONY : CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o.requires

CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o.provides: CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o.requires
	$(MAKE) -f CMakeFiles/viewer3d.dir/build.make CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o.provides.build
.PHONY : CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o.provides

CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o.provides.build: CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o


CMakeFiles/viewer3d.dir/src/helpers.cpp.o: CMakeFiles/viewer3d.dir/flags.make
CMakeFiles/viewer3d.dir/src/helpers.cpp.o: ../src/helpers.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/viewer3d.dir/src/helpers.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer3d.dir/src/helpers.cpp.o -c /home/user/Desktop/spider/Viewer3d/src/helpers.cpp

CMakeFiles/viewer3d.dir/src/helpers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer3d.dir/src/helpers.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/spider/Viewer3d/src/helpers.cpp > CMakeFiles/viewer3d.dir/src/helpers.cpp.i

CMakeFiles/viewer3d.dir/src/helpers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer3d.dir/src/helpers.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/spider/Viewer3d/src/helpers.cpp -o CMakeFiles/viewer3d.dir/src/helpers.cpp.s

CMakeFiles/viewer3d.dir/src/helpers.cpp.o.requires:

.PHONY : CMakeFiles/viewer3d.dir/src/helpers.cpp.o.requires

CMakeFiles/viewer3d.dir/src/helpers.cpp.o.provides: CMakeFiles/viewer3d.dir/src/helpers.cpp.o.requires
	$(MAKE) -f CMakeFiles/viewer3d.dir/build.make CMakeFiles/viewer3d.dir/src/helpers.cpp.o.provides.build
.PHONY : CMakeFiles/viewer3d.dir/src/helpers.cpp.o.provides

CMakeFiles/viewer3d.dir/src/helpers.cpp.o.provides.build: CMakeFiles/viewer3d.dir/src/helpers.cpp.o


CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o: CMakeFiles/viewer3d.dir/flags.make
CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o: ../src/loadmodels.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o -c /home/user/Desktop/spider/Viewer3d/src/loadmodels.cpp

CMakeFiles/viewer3d.dir/src/loadmodels.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer3d.dir/src/loadmodels.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/spider/Viewer3d/src/loadmodels.cpp > CMakeFiles/viewer3d.dir/src/loadmodels.cpp.i

CMakeFiles/viewer3d.dir/src/loadmodels.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer3d.dir/src/loadmodels.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/spider/Viewer3d/src/loadmodels.cpp -o CMakeFiles/viewer3d.dir/src/loadmodels.cpp.s

CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o.requires:

.PHONY : CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o.requires

CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o.provides: CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o.requires
	$(MAKE) -f CMakeFiles/viewer3d.dir/build.make CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o.provides.build
.PHONY : CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o.provides

CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o.provides.build: CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o


CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o: CMakeFiles/viewer3d.dir/flags.make
CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o: ../src/loadoperations.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o -c /home/user/Desktop/spider/Viewer3d/src/loadoperations.cpp

CMakeFiles/viewer3d.dir/src/loadoperations.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer3d.dir/src/loadoperations.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/spider/Viewer3d/src/loadoperations.cpp > CMakeFiles/viewer3d.dir/src/loadoperations.cpp.i

CMakeFiles/viewer3d.dir/src/loadoperations.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer3d.dir/src/loadoperations.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/spider/Viewer3d/src/loadoperations.cpp -o CMakeFiles/viewer3d.dir/src/loadoperations.cpp.s

CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o.requires:

.PHONY : CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o.requires

CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o.provides: CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o.requires
	$(MAKE) -f CMakeFiles/viewer3d.dir/build.make CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o.provides.build
.PHONY : CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o.provides

CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o.provides.build: CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o


CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o: CMakeFiles/viewer3d.dir/flags.make
CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o: ../src/loadoptypes.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o -c /home/user/Desktop/spider/Viewer3d/src/loadoptypes.cpp

CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/spider/Viewer3d/src/loadoptypes.cpp > CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.i

CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/spider/Viewer3d/src/loadoptypes.cpp -o CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.s

CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o.requires:

.PHONY : CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o.requires

CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o.provides: CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o.requires
	$(MAKE) -f CMakeFiles/viewer3d.dir/build.make CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o.provides.build
.PHONY : CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o.provides

CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o.provides.build: CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o


CMakeFiles/viewer3d.dir/src/models.cpp.o: CMakeFiles/viewer3d.dir/flags.make
CMakeFiles/viewer3d.dir/src/models.cpp.o: ../src/models.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/viewer3d.dir/src/models.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer3d.dir/src/models.cpp.o -c /home/user/Desktop/spider/Viewer3d/src/models.cpp

CMakeFiles/viewer3d.dir/src/models.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer3d.dir/src/models.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/spider/Viewer3d/src/models.cpp > CMakeFiles/viewer3d.dir/src/models.cpp.i

CMakeFiles/viewer3d.dir/src/models.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer3d.dir/src/models.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/spider/Viewer3d/src/models.cpp -o CMakeFiles/viewer3d.dir/src/models.cpp.s

CMakeFiles/viewer3d.dir/src/models.cpp.o.requires:

.PHONY : CMakeFiles/viewer3d.dir/src/models.cpp.o.requires

CMakeFiles/viewer3d.dir/src/models.cpp.o.provides: CMakeFiles/viewer3d.dir/src/models.cpp.o.requires
	$(MAKE) -f CMakeFiles/viewer3d.dir/build.make CMakeFiles/viewer3d.dir/src/models.cpp.o.provides.build
.PHONY : CMakeFiles/viewer3d.dir/src/models.cpp.o.provides

CMakeFiles/viewer3d.dir/src/models.cpp.o.provides.build: CMakeFiles/viewer3d.dir/src/models.cpp.o


CMakeFiles/viewer3d.dir/src/operations.cpp.o: CMakeFiles/viewer3d.dir/flags.make
CMakeFiles/viewer3d.dir/src/operations.cpp.o: ../src/operations.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/viewer3d.dir/src/operations.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer3d.dir/src/operations.cpp.o -c /home/user/Desktop/spider/Viewer3d/src/operations.cpp

CMakeFiles/viewer3d.dir/src/operations.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer3d.dir/src/operations.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/spider/Viewer3d/src/operations.cpp > CMakeFiles/viewer3d.dir/src/operations.cpp.i

CMakeFiles/viewer3d.dir/src/operations.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer3d.dir/src/operations.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/spider/Viewer3d/src/operations.cpp -o CMakeFiles/viewer3d.dir/src/operations.cpp.s

CMakeFiles/viewer3d.dir/src/operations.cpp.o.requires:

.PHONY : CMakeFiles/viewer3d.dir/src/operations.cpp.o.requires

CMakeFiles/viewer3d.dir/src/operations.cpp.o.provides: CMakeFiles/viewer3d.dir/src/operations.cpp.o.requires
	$(MAKE) -f CMakeFiles/viewer3d.dir/build.make CMakeFiles/viewer3d.dir/src/operations.cpp.o.provides.build
.PHONY : CMakeFiles/viewer3d.dir/src/operations.cpp.o.provides

CMakeFiles/viewer3d.dir/src/operations.cpp.o.provides.build: CMakeFiles/viewer3d.dir/src/operations.cpp.o


CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o: CMakeFiles/viewer3d.dir/flags.make
CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o: ../src/viewer3d.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o -c /home/user/Desktop/spider/Viewer3d/src/viewer3d.cpp

CMakeFiles/viewer3d.dir/src/viewer3d.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/viewer3d.dir/src/viewer3d.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/spider/Viewer3d/src/viewer3d.cpp > CMakeFiles/viewer3d.dir/src/viewer3d.cpp.i

CMakeFiles/viewer3d.dir/src/viewer3d.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/viewer3d.dir/src/viewer3d.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/spider/Viewer3d/src/viewer3d.cpp -o CMakeFiles/viewer3d.dir/src/viewer3d.cpp.s

CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o.requires:

.PHONY : CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o.requires

CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o.provides: CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o.requires
	$(MAKE) -f CMakeFiles/viewer3d.dir/build.make CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o.provides.build
.PHONY : CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o.provides

CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o.provides.build: CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o


# Object files for target viewer3d
viewer3d_OBJECTS = \
"CMakeFiles/viewer3d.dir/src/display.cpp.o" \
"CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o" \
"CMakeFiles/viewer3d.dir/src/helpers.cpp.o" \
"CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o" \
"CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o" \
"CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o" \
"CMakeFiles/viewer3d.dir/src/models.cpp.o" \
"CMakeFiles/viewer3d.dir/src/operations.cpp.o" \
"CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o"

# External object files for target viewer3d
viewer3d_EXTERNAL_OBJECTS =

viewer3d: CMakeFiles/viewer3d.dir/src/display.cpp.o
viewer3d: CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o
viewer3d: CMakeFiles/viewer3d.dir/src/helpers.cpp.o
viewer3d: CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o
viewer3d: CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o
viewer3d: CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o
viewer3d: CMakeFiles/viewer3d.dir/src/models.cpp.o
viewer3d: CMakeFiles/viewer3d.dir/src/operations.cpp.o
viewer3d: CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o
viewer3d: CMakeFiles/viewer3d.dir/build.make
viewer3d: /usr/lib/x86_64-linux-gnu/libglut.so
viewer3d: /usr/lib/x86_64-linux-gnu/libGL.so
viewer3d: /usr/lib/x86_64-linux-gnu/libGLU.so
viewer3d: CMakeFiles/viewer3d.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/Desktop/spider/Viewer3d/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable viewer3d"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/viewer3d.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/viewer3d.dir/build: viewer3d

.PHONY : CMakeFiles/viewer3d.dir/build

CMakeFiles/viewer3d.dir/requires: CMakeFiles/viewer3d.dir/src/display.cpp.o.requires
CMakeFiles/viewer3d.dir/requires: CMakeFiles/viewer3d.dir/src/displayfacet.cpp.o.requires
CMakeFiles/viewer3d.dir/requires: CMakeFiles/viewer3d.dir/src/helpers.cpp.o.requires
CMakeFiles/viewer3d.dir/requires: CMakeFiles/viewer3d.dir/src/loadmodels.cpp.o.requires
CMakeFiles/viewer3d.dir/requires: CMakeFiles/viewer3d.dir/src/loadoperations.cpp.o.requires
CMakeFiles/viewer3d.dir/requires: CMakeFiles/viewer3d.dir/src/loadoptypes.cpp.o.requires
CMakeFiles/viewer3d.dir/requires: CMakeFiles/viewer3d.dir/src/models.cpp.o.requires
CMakeFiles/viewer3d.dir/requires: CMakeFiles/viewer3d.dir/src/operations.cpp.o.requires
CMakeFiles/viewer3d.dir/requires: CMakeFiles/viewer3d.dir/src/viewer3d.cpp.o.requires

.PHONY : CMakeFiles/viewer3d.dir/requires

CMakeFiles/viewer3d.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/viewer3d.dir/cmake_clean.cmake
.PHONY : CMakeFiles/viewer3d.dir/clean

CMakeFiles/viewer3d.dir/depend:
	cd /home/user/Desktop/spider/Viewer3d/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/Desktop/spider/Viewer3d /home/user/Desktop/spider/Viewer3d /home/user/Desktop/spider/Viewer3d/build /home/user/Desktop/spider/Viewer3d/build /home/user/Desktop/spider/Viewer3d/build/CMakeFiles/viewer3d.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/viewer3d.dir/depend

