#ifndef INC_3_VIDEO_COMPRESSION_ENCODERCONFIG_H
#define INC_3_VIDEO_COMPRESSION_ENCODERCONFIG_H


#include <iostream>
#include "Config.h"

class EncoderConfig : public Config {
public:
    explicit EncoderConfig(const std::string &filePath);

    const std::string rawfile;
    const std::string encfile;
    const int width;
    const int height;
    const bool rle;
    const std::string quantfile;
    const std::string logfile;
    const int gop;
    const int merange;
};


#endif //INC_3_VIDEO_COMPRESSION_ENCODERCONFIG_H
