#ifndef INC_3_VIDEO_COMPRESSION_VIDEOPROCESSOR_H
#define INC_3_VIDEO_COMPRESSION_VIDEOPROCESSOR_H


#include <fstream>
#include "ValueBlock4x4.h"

class VideoCodec {
public:
    bool encode(std::ifstream &in, long inEnd, std::ofstream &out,
                int width, int height,
                bool rle, ValueBlock4x4 &quantMatrix, int gop, int merange);

    bool decode(std::ifstream &in, long inEnd, std::ofstream &out, bool motionCompensation);
};


#endif //INC_3_VIDEO_COMPRESSION_VIDEOPROCESSOR_H
