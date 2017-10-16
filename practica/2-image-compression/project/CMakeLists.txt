cmake_minimum_required(VERSION 3.8)
project(project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/Main.cpp src/configreader.cpp src/BitStream.cpp src/PixelBlock.cpp src/QuantMatrix.cpp src/QuantFileParser.cpp src/Config.cpp src/MainEncoder.cpp)
set(SOURCE_FILES_ENCODER src/MainEncoder.cpp)
set(SOURCE_FILES_DECODER )
set(SOURCE_FILES_CODEC )

add_executable(encoder src/encoder.cpp ${SOURCE_FILES})
add_executable(decoder src/decoder.cpp ${SOURCE_FILES})
add_executable(codec src/codec.cpp ${SOURCE_FILES})
