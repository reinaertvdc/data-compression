#ifndef INC_3_VIDEO_COMPRESSION_FRAME_H
#define INC_3_VIDEO_COMPRESSION_FRAME_H


#include <vector>
#include "ValueBlock4x4.h"

class Frame {
public:
    Frame(uint16_t width, uint16_t height);

    ~Frame();

    bool readRaw(std::ifstream &in);

    bool readI(std::ifstream &in, bool rle, const ValueBlock4x4 &quantMatrix);

    bool readP(std::ifstream &in, const Frame &previousFrame, uint16_t gop, uint16_t merange, bool motionCompensation);

    bool writeRaw(std::ofstream &out);

    bool writeI(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix);

    bool writeP(std::ofstream &out, const Frame &previousFrame, uint16_t gop, uint16_t merange);

private:
    static int numInstances, frameBufferSize;
    static uint8_t *frameBuffer;

    const uint16_t width, height;
    const int rawSize, numBlocks;
    ValueBlock4x4 **const blocks;
};


#endif //INC_3_VIDEO_COMPRESSION_FRAME_H
