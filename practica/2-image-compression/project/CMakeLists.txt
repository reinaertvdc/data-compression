cmake_minimum_required(VERSION 3.8)
project(project)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/configreader.cpp src/BitStream.cpp src/ValueBlock4x4.cpp src/QuantFileParser.cpp src/Config.cpp src/Init.cpp src/RawFileParser.cpp src/RawFileParser.h)
set(SOURCE_FILES_ENCODER src/MainEncoder.cpp)
set(SOURCE_FILES_DECODER src/MainDecoder.cpp)
set(SOURCE_FILES_CODEC src/MainCodec.cpp)

add_executable(encoder ${SOURCE_FILES} ${SOURCE_FILES_ENCODER})
add_executable(decoder ${SOURCE_FILES} ${SOURCE_FILES_DECODER})
add_executable(codec ${SOURCE_FILES} ${SOURCE_FILES_CODEC})
