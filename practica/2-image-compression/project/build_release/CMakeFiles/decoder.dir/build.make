# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /home/cwout/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/172.4343.16/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/cwout/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/172.4343.16/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release

# Include any dependencies generated for this target.
include CMakeFiles/decoder.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/decoder.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/decoder.dir/flags.make

CMakeFiles/decoder.dir/src/configreader.cpp.o: CMakeFiles/decoder.dir/flags.make
CMakeFiles/decoder.dir/src/configreader.cpp.o: ../src/configreader.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/decoder.dir/src/configreader.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/decoder.dir/src/configreader.cpp.o -c /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/configreader.cpp

CMakeFiles/decoder.dir/src/configreader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/decoder.dir/src/configreader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/configreader.cpp > CMakeFiles/decoder.dir/src/configreader.cpp.i

CMakeFiles/decoder.dir/src/configreader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/decoder.dir/src/configreader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/configreader.cpp -o CMakeFiles/decoder.dir/src/configreader.cpp.s

CMakeFiles/decoder.dir/src/configreader.cpp.o.requires:

.PHONY : CMakeFiles/decoder.dir/src/configreader.cpp.o.requires

CMakeFiles/decoder.dir/src/configreader.cpp.o.provides: CMakeFiles/decoder.dir/src/configreader.cpp.o.requires
	$(MAKE) -f CMakeFiles/decoder.dir/build.make CMakeFiles/decoder.dir/src/configreader.cpp.o.provides.build
.PHONY : CMakeFiles/decoder.dir/src/configreader.cpp.o.provides

CMakeFiles/decoder.dir/src/configreader.cpp.o.provides.build: CMakeFiles/decoder.dir/src/configreader.cpp.o


CMakeFiles/decoder.dir/src/BitStream.cpp.o: CMakeFiles/decoder.dir/flags.make
CMakeFiles/decoder.dir/src/BitStream.cpp.o: ../src/BitStream.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/decoder.dir/src/BitStream.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/decoder.dir/src/BitStream.cpp.o -c /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/BitStream.cpp

CMakeFiles/decoder.dir/src/BitStream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/decoder.dir/src/BitStream.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/BitStream.cpp > CMakeFiles/decoder.dir/src/BitStream.cpp.i

CMakeFiles/decoder.dir/src/BitStream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/decoder.dir/src/BitStream.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/BitStream.cpp -o CMakeFiles/decoder.dir/src/BitStream.cpp.s

CMakeFiles/decoder.dir/src/BitStream.cpp.o.requires:

.PHONY : CMakeFiles/decoder.dir/src/BitStream.cpp.o.requires

CMakeFiles/decoder.dir/src/BitStream.cpp.o.provides: CMakeFiles/decoder.dir/src/BitStream.cpp.o.requires
	$(MAKE) -f CMakeFiles/decoder.dir/build.make CMakeFiles/decoder.dir/src/BitStream.cpp.o.provides.build
.PHONY : CMakeFiles/decoder.dir/src/BitStream.cpp.o.provides

CMakeFiles/decoder.dir/src/BitStream.cpp.o.provides.build: CMakeFiles/decoder.dir/src/BitStream.cpp.o


CMakeFiles/decoder.dir/src/PixelBlock.cpp.o: CMakeFiles/decoder.dir/flags.make
CMakeFiles/decoder.dir/src/PixelBlock.cpp.o: ../src/PixelBlock.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/decoder.dir/src/PixelBlock.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/decoder.dir/src/PixelBlock.cpp.o -c /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/PixelBlock.cpp

CMakeFiles/decoder.dir/src/PixelBlock.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/decoder.dir/src/PixelBlock.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/PixelBlock.cpp > CMakeFiles/decoder.dir/src/PixelBlock.cpp.i

CMakeFiles/decoder.dir/src/PixelBlock.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/decoder.dir/src/PixelBlock.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/PixelBlock.cpp -o CMakeFiles/decoder.dir/src/PixelBlock.cpp.s

CMakeFiles/decoder.dir/src/PixelBlock.cpp.o.requires:

.PHONY : CMakeFiles/decoder.dir/src/PixelBlock.cpp.o.requires

CMakeFiles/decoder.dir/src/PixelBlock.cpp.o.provides: CMakeFiles/decoder.dir/src/PixelBlock.cpp.o.requires
	$(MAKE) -f CMakeFiles/decoder.dir/build.make CMakeFiles/decoder.dir/src/PixelBlock.cpp.o.provides.build
.PHONY : CMakeFiles/decoder.dir/src/PixelBlock.cpp.o.provides

CMakeFiles/decoder.dir/src/PixelBlock.cpp.o.provides.build: CMakeFiles/decoder.dir/src/PixelBlock.cpp.o


CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o: CMakeFiles/decoder.dir/flags.make
CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o: ../src/QuantMatrix.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o -c /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/QuantMatrix.cpp

CMakeFiles/decoder.dir/src/QuantMatrix.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/decoder.dir/src/QuantMatrix.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/QuantMatrix.cpp > CMakeFiles/decoder.dir/src/QuantMatrix.cpp.i

CMakeFiles/decoder.dir/src/QuantMatrix.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/decoder.dir/src/QuantMatrix.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/QuantMatrix.cpp -o CMakeFiles/decoder.dir/src/QuantMatrix.cpp.s

CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o.requires:

.PHONY : CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o.requires

CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o.provides: CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o.requires
	$(MAKE) -f CMakeFiles/decoder.dir/build.make CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o.provides.build
.PHONY : CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o.provides

CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o.provides.build: CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o


CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o: CMakeFiles/decoder.dir/flags.make
CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o: ../src/QuantFileParser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o -c /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/QuantFileParser.cpp

CMakeFiles/decoder.dir/src/QuantFileParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/decoder.dir/src/QuantFileParser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/QuantFileParser.cpp > CMakeFiles/decoder.dir/src/QuantFileParser.cpp.i

CMakeFiles/decoder.dir/src/QuantFileParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/decoder.dir/src/QuantFileParser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/QuantFileParser.cpp -o CMakeFiles/decoder.dir/src/QuantFileParser.cpp.s

CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o.requires:

.PHONY : CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o.requires

CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o.provides: CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o.requires
	$(MAKE) -f CMakeFiles/decoder.dir/build.make CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o.provides.build
.PHONY : CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o.provides

CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o.provides.build: CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o


CMakeFiles/decoder.dir/src/Config.cpp.o: CMakeFiles/decoder.dir/flags.make
CMakeFiles/decoder.dir/src/Config.cpp.o: ../src/Config.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/decoder.dir/src/Config.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/decoder.dir/src/Config.cpp.o -c /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/Config.cpp

CMakeFiles/decoder.dir/src/Config.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/decoder.dir/src/Config.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/Config.cpp > CMakeFiles/decoder.dir/src/Config.cpp.i

CMakeFiles/decoder.dir/src/Config.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/decoder.dir/src/Config.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/Config.cpp -o CMakeFiles/decoder.dir/src/Config.cpp.s

CMakeFiles/decoder.dir/src/Config.cpp.o.requires:

.PHONY : CMakeFiles/decoder.dir/src/Config.cpp.o.requires

CMakeFiles/decoder.dir/src/Config.cpp.o.provides: CMakeFiles/decoder.dir/src/Config.cpp.o.requires
	$(MAKE) -f CMakeFiles/decoder.dir/build.make CMakeFiles/decoder.dir/src/Config.cpp.o.provides.build
.PHONY : CMakeFiles/decoder.dir/src/Config.cpp.o.provides

CMakeFiles/decoder.dir/src/Config.cpp.o.provides.build: CMakeFiles/decoder.dir/src/Config.cpp.o


CMakeFiles/decoder.dir/src/Init.cpp.o: CMakeFiles/decoder.dir/flags.make
CMakeFiles/decoder.dir/src/Init.cpp.o: ../src/Init.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/decoder.dir/src/Init.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/decoder.dir/src/Init.cpp.o -c /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/Init.cpp

CMakeFiles/decoder.dir/src/Init.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/decoder.dir/src/Init.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/Init.cpp > CMakeFiles/decoder.dir/src/Init.cpp.i

CMakeFiles/decoder.dir/src/Init.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/decoder.dir/src/Init.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/Init.cpp -o CMakeFiles/decoder.dir/src/Init.cpp.s

CMakeFiles/decoder.dir/src/Init.cpp.o.requires:

.PHONY : CMakeFiles/decoder.dir/src/Init.cpp.o.requires

CMakeFiles/decoder.dir/src/Init.cpp.o.provides: CMakeFiles/decoder.dir/src/Init.cpp.o.requires
	$(MAKE) -f CMakeFiles/decoder.dir/build.make CMakeFiles/decoder.dir/src/Init.cpp.o.provides.build
.PHONY : CMakeFiles/decoder.dir/src/Init.cpp.o.provides

CMakeFiles/decoder.dir/src/Init.cpp.o.provides.build: CMakeFiles/decoder.dir/src/Init.cpp.o


CMakeFiles/decoder.dir/src/MainDecoder.cpp.o: CMakeFiles/decoder.dir/flags.make
CMakeFiles/decoder.dir/src/MainDecoder.cpp.o: ../src/MainDecoder.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/decoder.dir/src/MainDecoder.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/decoder.dir/src/MainDecoder.cpp.o -c /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/MainDecoder.cpp

CMakeFiles/decoder.dir/src/MainDecoder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/decoder.dir/src/MainDecoder.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/MainDecoder.cpp > CMakeFiles/decoder.dir/src/MainDecoder.cpp.i

CMakeFiles/decoder.dir/src/MainDecoder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/decoder.dir/src/MainDecoder.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/src/MainDecoder.cpp -o CMakeFiles/decoder.dir/src/MainDecoder.cpp.s

CMakeFiles/decoder.dir/src/MainDecoder.cpp.o.requires:

.PHONY : CMakeFiles/decoder.dir/src/MainDecoder.cpp.o.requires

CMakeFiles/decoder.dir/src/MainDecoder.cpp.o.provides: CMakeFiles/decoder.dir/src/MainDecoder.cpp.o.requires
	$(MAKE) -f CMakeFiles/decoder.dir/build.make CMakeFiles/decoder.dir/src/MainDecoder.cpp.o.provides.build
.PHONY : CMakeFiles/decoder.dir/src/MainDecoder.cpp.o.provides

CMakeFiles/decoder.dir/src/MainDecoder.cpp.o.provides.build: CMakeFiles/decoder.dir/src/MainDecoder.cpp.o


# Object files for target decoder
decoder_OBJECTS = \
"CMakeFiles/decoder.dir/src/configreader.cpp.o" \
"CMakeFiles/decoder.dir/src/BitStream.cpp.o" \
"CMakeFiles/decoder.dir/src/PixelBlock.cpp.o" \
"CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o" \
"CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o" \
"CMakeFiles/decoder.dir/src/Config.cpp.o" \
"CMakeFiles/decoder.dir/src/Init.cpp.o" \
"CMakeFiles/decoder.dir/src/MainDecoder.cpp.o"

# External object files for target decoder
decoder_EXTERNAL_OBJECTS =

decoder: CMakeFiles/decoder.dir/src/configreader.cpp.o
decoder: CMakeFiles/decoder.dir/src/BitStream.cpp.o
decoder: CMakeFiles/decoder.dir/src/PixelBlock.cpp.o
decoder: CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o
decoder: CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o
decoder: CMakeFiles/decoder.dir/src/Config.cpp.o
decoder: CMakeFiles/decoder.dir/src/Init.cpp.o
decoder: CMakeFiles/decoder.dir/src/MainDecoder.cpp.o
decoder: CMakeFiles/decoder.dir/build.make
decoder: CMakeFiles/decoder.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable decoder"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/decoder.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/decoder.dir/build: decoder

.PHONY : CMakeFiles/decoder.dir/build

CMakeFiles/decoder.dir/requires: CMakeFiles/decoder.dir/src/configreader.cpp.o.requires
CMakeFiles/decoder.dir/requires: CMakeFiles/decoder.dir/src/BitStream.cpp.o.requires
CMakeFiles/decoder.dir/requires: CMakeFiles/decoder.dir/src/PixelBlock.cpp.o.requires
CMakeFiles/decoder.dir/requires: CMakeFiles/decoder.dir/src/QuantMatrix.cpp.o.requires
CMakeFiles/decoder.dir/requires: CMakeFiles/decoder.dir/src/QuantFileParser.cpp.o.requires
CMakeFiles/decoder.dir/requires: CMakeFiles/decoder.dir/src/Config.cpp.o.requires
CMakeFiles/decoder.dir/requires: CMakeFiles/decoder.dir/src/Init.cpp.o.requires
CMakeFiles/decoder.dir/requires: CMakeFiles/decoder.dir/src/MainDecoder.cpp.o.requires

.PHONY : CMakeFiles/decoder.dir/requires

CMakeFiles/decoder.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/decoder.dir/cmake_clean.cmake
.PHONY : CMakeFiles/decoder.dir/clean

CMakeFiles/decoder.dir/depend:
	cd /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release /home/cwout/Documents/Unief/Dc/practica/2-image-compression/project/build_release/CMakeFiles/decoder.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/decoder.dir/depend

