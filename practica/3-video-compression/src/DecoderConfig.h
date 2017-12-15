#ifndef INC_3_VIDEO_COMPRESSION_DECODERCONFIG_H
#define INC_3_VIDEO_COMPRESSION_DECODERCONFIG_H


#include <iostream>
#include "Config.h"

class DecoderConfig : public Config {
public:
    explicit DecoderConfig(const std::string &filePath);

    const std::string decfile;
    const std::string encfile;
    const std::string logfile;
    const bool motioncompensation;
};


#endif //INC_3_VIDEO_COMPRESSION_DECODERCONFIG_H
