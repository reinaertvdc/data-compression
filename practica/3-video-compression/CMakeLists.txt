cmake_minimum_required(VERSION 3.8)
project(3_video_compression)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/Frame.cpp src/Frame.h src/Config.cpp src/Config.h src/Codec.cpp src/Codec.h src/Logger.cpp src/Logger.h src/BitStream.cpp src/BitStream.h src/EncoderConfig.cpp src/EncoderConfig.h src/DecoderConfig.cpp src/DecoderConfig.h src/ValueBlock4x4.cpp src/ValueBlock4x4.h src/QuantFileParser.cpp src/QuantFileParser.h src/VideoCodec.cpp src/VideoCodec.h src/IFrameStorageCodec.cpp src/IFrameStorageCodec.h src/RleCodec.cpp src/RleCodec.h  src/CompactingCodec.cpp src/CompactingCodec.h)
add_executable(encoder src/encoder.cpp ${SOURCE_FILES})
add_executable(decoder src/decoder.cpp ${SOURCE_FILES})