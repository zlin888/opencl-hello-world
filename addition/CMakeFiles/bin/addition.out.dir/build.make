# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.18.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.18.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/zlin/Local/opencl-hello-world/addition

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/zlin/Local/opencl-hello-world/addition

# Include any dependencies generated for this target.
include CMakeFiles/bin/addition.out.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bin/addition.out.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bin/addition.out.dir/flags.make

CMakeFiles/bin/addition.out.dir/main.cpp.o: CMakeFiles/bin/addition.out.dir/flags.make
CMakeFiles/bin/addition.out.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zlin/Local/opencl-hello-world/addition/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bin/addition.out.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bin/addition.out.dir/main.cpp.o -c /Users/zlin/Local/opencl-hello-world/addition/main.cpp

CMakeFiles/bin/addition.out.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bin/addition.out.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zlin/Local/opencl-hello-world/addition/main.cpp > CMakeFiles/bin/addition.out.dir/main.cpp.i

CMakeFiles/bin/addition.out.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bin/addition.out.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zlin/Local/opencl-hello-world/addition/main.cpp -o CMakeFiles/bin/addition.out.dir/main.cpp.s

# Object files for target bin/addition.out
bin/addition_out_OBJECTS = \
"CMakeFiles/bin/addition.out.dir/main.cpp.o"

# External object files for target bin/addition.out
bin/addition_out_EXTERNAL_OBJECTS =

bin/addition.out: CMakeFiles/bin/addition.out.dir/main.cpp.o
bin/addition.out: CMakeFiles/bin/addition.out.dir/build.make
bin/addition.out: CMakeFiles/bin/addition.out.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/zlin/Local/opencl-hello-world/addition/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/addition.out"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bin/addition.out.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bin/addition.out.dir/build: bin/addition.out

.PHONY : CMakeFiles/bin/addition.out.dir/build

CMakeFiles/bin/addition.out.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bin/addition.out.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bin/addition.out.dir/clean

CMakeFiles/bin/addition.out.dir/depend:
	cd /Users/zlin/Local/opencl-hello-world/addition && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/zlin/Local/opencl-hello-world/addition /Users/zlin/Local/opencl-hello-world/addition /Users/zlin/Local/opencl-hello-world/addition /Users/zlin/Local/opencl-hello-world/addition /Users/zlin/Local/opencl-hello-world/addition/CMakeFiles/bin/addition.out.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bin/addition.out.dir/depend
