cmake_minimum_required(VERSION 3.8)
project(project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/Main.cpp src/Main.h src/configreader.cpp src/BitStream.cpp src/PixelBlock.cpp src/PixelBlock.h src/QuantMatrix.cpp src/QuantMatrix.h src/QuantFileParser.cpp src/QuantFileParser.h src/Config.cpp src/Config.h)

add_executable(encoder src/encoder.cpp ${SOURCE_FILES})
add_executable(decoder src/decoder.cpp ${SOURCE_FILES})
add_executable(codec src/codec.cpp ${SOURCE_FILES})
