cmake_minimum_required(VERSION 3.8)
project(project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/configreader.cpp src/BitStream.cpp src/PixelBlock.cpp src/PixelBlock.h src/QuantMatrix.cpp src/QuantMatrix.h src/QuantFileParser.cpp src/QuantFileParser.h)
add_executable(project ${SOURCE_FILES})
