cmake_minimum_required(VERSION 3.8)
project(3_video_compression)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/frames/Frame.cpp src/frames/Frame.h src/frames/IFrame.cpp src/frames/IFrame.h src/frames/PFrame.cpp src/frames/PFrame.h src/configs/Config.cpp src/configs/Config.h src/main/Codec.cpp src/main/Codec.h src/helpers/Logger.cpp src/helpers/Logger.h src/helpers/BitStream.cpp src/helpers/BitStream.h src/configs/EncoderConfig.cpp src/configs/EncoderConfig.h src/configs/DecoderConfig.cpp src/configs/DecoderConfig.h src/helpers/ValueBlock4x4.cpp src/helpers/ValueBlock4x4.h src/helpers/QuantFileParser.cpp src/helpers/QuantFileParser.h)
add_executable(encoder src/main/encoder.cpp ${SOURCE_FILES})
add_executable(decoder src/main/decoder.cpp ${SOURCE_FILES})