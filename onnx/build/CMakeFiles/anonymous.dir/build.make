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
CMAKE_SOURCE_DIR = /home/plusai/workspace/cpptest/onnx

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/plusai/workspace/cpptest/onnx/build

# Include any dependencies generated for this target.
include CMakeFiles/anonymous.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/anonymous.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/anonymous.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/anonymous.dir/flags.make

CMakeFiles/anonymous.dir/anonymous.cpp.o: CMakeFiles/anonymous.dir/flags.make
CMakeFiles/anonymous.dir/anonymous.cpp.o: ../anonymous.cpp
CMakeFiles/anonymous.dir/anonymous.cpp.o: CMakeFiles/anonymous.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/plusai/workspace/cpptest/onnx/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/anonymous.dir/anonymous.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/anonymous.dir/anonymous.cpp.o -MF CMakeFiles/anonymous.dir/anonymous.cpp.o.d -o CMakeFiles/anonymous.dir/anonymous.cpp.o -c /home/plusai/workspace/cpptest/onnx/anonymous.cpp

CMakeFiles/anonymous.dir/anonymous.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/anonymous.dir/anonymous.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/plusai/workspace/cpptest/onnx/anonymous.cpp > CMakeFiles/anonymous.dir/anonymous.cpp.i

CMakeFiles/anonymous.dir/anonymous.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/anonymous.dir/anonymous.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/plusai/workspace/cpptest/onnx/anonymous.cpp -o CMakeFiles/anonymous.dir/anonymous.cpp.s

# Object files for target anonymous
anonymous_OBJECTS = \
"CMakeFiles/anonymous.dir/anonymous.cpp.o"

# External object files for target anonymous
anonymous_EXTERNAL_OBJECTS =

anonymous: CMakeFiles/anonymous.dir/anonymous.cpp.o
anonymous: CMakeFiles/anonymous.dir/build.make
anonymous: CMakeFiles/anonymous.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/plusai/workspace/cpptest/onnx/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable anonymous"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/anonymous.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/anonymous.dir/build: anonymous
.PHONY : CMakeFiles/anonymous.dir/build

CMakeFiles/anonymous.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/anonymous.dir/cmake_clean.cmake
.PHONY : CMakeFiles/anonymous.dir/clean

CMakeFiles/anonymous.dir/depend:
	cd /home/plusai/workspace/cpptest/onnx/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/plusai/workspace/cpptest/onnx /home/plusai/workspace/cpptest/onnx /home/plusai/workspace/cpptest/onnx/build /home/plusai/workspace/cpptest/onnx/build /home/plusai/workspace/cpptest/onnx/build/CMakeFiles/anonymous.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/anonymous.dir/depend

