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
CMAKE_SOURCE_DIR = /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/build

# Include any dependencies generated for this target.
include CMakeFiles/main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.dir/flags.make

your_message.pb.h: ../your_message.proto
your_message.pb.h: /usr/bin/protoc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running cpp protocol buffer compiler on your_message.proto"
	/usr/bin/protoc --cpp_out /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/build -I /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/your_message.proto

your_message.pb.cc: your_message.pb.h
	@$(CMAKE_COMMAND) -E touch_nocreate your_message.pb.cc

CMakeFiles/main.dir/main.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/main.cpp.o: ../main.cpp
CMakeFiles/main.dir/main.cpp.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/main.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.dir/main.cpp.o -MF CMakeFiles/main.dir/main.cpp.o.d -o CMakeFiles/main.dir/main.cpp.o -c /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/main.cpp

CMakeFiles/main.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/main.cpp > CMakeFiles/main.dir/main.cpp.i

CMakeFiles/main.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/main.cpp -o CMakeFiles/main.dir/main.cpp.s

CMakeFiles/main.dir/your_message.pb.cc.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/your_message.pb.cc.o: your_message.pb.cc
CMakeFiles/main.dir/your_message.pb.cc.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/main.dir/your_message.pb.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.dir/your_message.pb.cc.o -MF CMakeFiles/main.dir/your_message.pb.cc.o.d -o CMakeFiles/main.dir/your_message.pb.cc.o -c /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/build/your_message.pb.cc

CMakeFiles/main.dir/your_message.pb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/your_message.pb.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/build/your_message.pb.cc > CMakeFiles/main.dir/your_message.pb.cc.i

CMakeFiles/main.dir/your_message.pb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/your_message.pb.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/build/your_message.pb.cc -o CMakeFiles/main.dir/your_message.pb.cc.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/main.cpp.o" \
"CMakeFiles/main.dir/your_message.pb.cc.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

main: CMakeFiles/main.dir/main.cpp.o
main: CMakeFiles/main.dir/your_message.pb.cc.o
main: CMakeFiles/main.dir/build.make
main: CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main.dir/build: main
.PHONY : CMakeFiles/main.dir/build

CMakeFiles/main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main.dir/clean

CMakeFiles/main.dir/depend: your_message.pb.cc
CMakeFiles/main.dir/depend: your_message.pb.h
	cd /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/build /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/build /home/plusai/Documents/PlusNotes/PlusNotes/proto_bytes/build/CMakeFiles/main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main.dir/depend

