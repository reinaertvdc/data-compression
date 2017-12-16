#ifndef INC_3_VIDEO_COMPRESSION_VIDEOPROCESSOR_H
#define INC_3_VIDEO_COMPRESSION_VIDEOPROCESSOR_H


#include <fstream>
#include "ValueBlock4x4.h"

class VideoCodec {
public:
    bool encode(std::ifstream &in, long inEnd, std::ofstream &out,
                uint16_t width, uint16_t height,
                bool rle, ValueBlock4x4 &quantMatrix, uint16_t gop, uint16_t merange);

    bool decode(std::ifstream &in, long inEnd, std::ofstream &out, bool motionCompensation);
};


#endif //INC_3_VIDEO_COMPRESSION_VIDEOPROCESSOR_H
