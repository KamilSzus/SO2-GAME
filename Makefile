# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_SOURCE_DIR = "/mnt/c/Users/kamil/Desktop/nauka/semestr 7/SO2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/kamil/Desktop/nauka/semestr 7/SO2"

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start "/mnt/c/Users/kamil/Desktop/nauka/semestr 7/SO2/CMakeFiles" "/mnt/c/Users/kamil/Desktop/nauka/semestr 7/SO2/CMakeFiles/progress.marks"
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start "/mnt/c/Users/kamil/Desktop/nauka/semestr 7/SO2/CMakeFiles" 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named SO2Client

# Build rule for target.
SO2Client: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 SO2Client
.PHONY : SO2Client

# fast build rule for target.
SO2Client/fast:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/build
.PHONY : SO2Client/fast

#=============================================================================
# Target rules for targets named SO2

# Build rule for target.
SO2: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 SO2
.PHONY : SO2

# fast build rule for target.
SO2/fast:
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/build
.PHONY : SO2/fast

clientMain.o: clientMain.c.o

.PHONY : clientMain.o

# target to build an object file
clientMain.c.o:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/clientMain.c.o
.PHONY : clientMain.c.o

clientMain.i: clientMain.c.i

.PHONY : clientMain.i

# target to preprocess a source file
clientMain.c.i:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/clientMain.c.i
.PHONY : clientMain.c.i

clientMain.s: clientMain.c.s

.PHONY : clientMain.s

# target to generate assembly for a file
clientMain.c.s:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/clientMain.c.s
.PHONY : clientMain.c.s

main.o: main.c.o

.PHONY : main.o

# target to build an object file
main.c.o:
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/main.c.o
.PHONY : main.c.o

main.i: main.c.i

.PHONY : main.i

# target to preprocess a source file
main.c.i:
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/main.c.i
.PHONY : main.c.i

main.s: main.c.s

.PHONY : main.s

# target to generate assembly for a file
main.c.s:
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/main.c.s
.PHONY : main.c.s

src/beast.o: src/beast.c.o

.PHONY : src/beast.o

# target to build an object file
src/beast.c.o:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/beast.c.o
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/beast.c.o
.PHONY : src/beast.c.o

src/beast.i: src/beast.c.i

.PHONY : src/beast.i

# target to preprocess a source file
src/beast.c.i:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/beast.c.i
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/beast.c.i
.PHONY : src/beast.c.i

src/beast.s: src/beast.c.s

.PHONY : src/beast.s

# target to generate assembly for a file
src/beast.c.s:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/beast.c.s
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/beast.c.s
.PHONY : src/beast.c.s

src/board.o: src/board.c.o

.PHONY : src/board.o

# target to build an object file
src/board.c.o:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/board.c.o
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/board.c.o
.PHONY : src/board.c.o

src/board.i: src/board.c.i

.PHONY : src/board.i

# target to preprocess a source file
src/board.c.i:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/board.c.i
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/board.c.i
.PHONY : src/board.c.i

src/board.s: src/board.c.s

.PHONY : src/board.s

# target to generate assembly for a file
src/board.c.s:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/board.c.s
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/board.c.s
.PHONY : src/board.c.s

src/client.o: src/client.c.o

.PHONY : src/client.o

# target to build an object file
src/client.c.o:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/client.c.o
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/client.c.o
.PHONY : src/client.c.o

src/client.i: src/client.c.i

.PHONY : src/client.i

# target to preprocess a source file
src/client.c.i:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/client.c.i
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/client.c.i
.PHONY : src/client.c.i

src/client.s: src/client.c.s

.PHONY : src/client.s

# target to generate assembly for a file
src/client.c.s:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/client.c.s
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/client.c.s
.PHONY : src/client.c.s

src/player.o: src/player.c.o

.PHONY : src/player.o

# target to build an object file
src/player.c.o:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/player.c.o
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/player.c.o
.PHONY : src/player.c.o

src/player.i: src/player.c.i

.PHONY : src/player.i

# target to preprocess a source file
src/player.c.i:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/player.c.i
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/player.c.i
.PHONY : src/player.c.i

src/player.s: src/player.c.s

.PHONY : src/player.s

# target to generate assembly for a file
src/player.c.s:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/player.c.s
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/player.c.s
.PHONY : src/player.c.s

src/point.o: src/point.c.o

.PHONY : src/point.o

# target to build an object file
src/point.c.o:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/point.c.o
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/point.c.o
.PHONY : src/point.c.o

src/point.i: src/point.c.i

.PHONY : src/point.i

# target to preprocess a source file
src/point.c.i:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/point.c.i
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/point.c.i
.PHONY : src/point.c.i

src/point.s: src/point.c.s

.PHONY : src/point.s

# target to generate assembly for a file
src/point.c.s:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/point.c.s
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/point.c.s
.PHONY : src/point.c.s

src/server.o: src/server.c.o

.PHONY : src/server.o

# target to build an object file
src/server.c.o:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/server.c.o
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/server.c.o
.PHONY : src/server.c.o

src/server.i: src/server.c.i

.PHONY : src/server.i

# target to preprocess a source file
src/server.c.i:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/server.c.i
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/server.c.i
.PHONY : src/server.c.i

src/server.s: src/server.c.s

.PHONY : src/server.s

# target to generate assembly for a file
src/server.c.s:
	$(MAKE) -f CMakeFiles/SO2Client.dir/build.make CMakeFiles/SO2Client.dir/src/server.c.s
	$(MAKE) -f CMakeFiles/SO2.dir/build.make CMakeFiles/SO2.dir/src/server.c.s
.PHONY : src/server.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... SO2Client"
	@echo "... SO2"
	@echo "... clientMain.o"
	@echo "... clientMain.i"
	@echo "... clientMain.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... src/beast.o"
	@echo "... src/beast.i"
	@echo "... src/beast.s"
	@echo "... src/board.o"
	@echo "... src/board.i"
	@echo "... src/board.s"
	@echo "... src/client.o"
	@echo "... src/client.i"
	@echo "... src/client.s"
	@echo "... src/player.o"
	@echo "... src/player.i"
	@echo "... src/player.s"
	@echo "... src/point.o"
	@echo "... src/point.i"
	@echo "... src/point.s"
	@echo "... src/server.o"
	@echo "... src/server.i"
	@echo "... src/server.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

