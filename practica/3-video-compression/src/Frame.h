#ifndef INC_3_VIDEO_COMPRESSION_FRAME_H
#define INC_3_VIDEO_COMPRESSION_FRAME_H


#include <vector>
#include "ValueBlock4x4.h"

class Frame {
public:
    Frame(int width, int height);

    ~Frame();

    bool copy(const Frame &other);

    bool readRaw(std::ifstream &in);

    bool readI(std::ifstream &in, bool rle, const ValueBlock4x4 &quantMatrix);

    bool readP(std::ifstream &in, bool rle, const ValueBlock4x4 &quantMatrix, const Frame &previousFrame, uint16_t gop,
               uint16_t merange, bool motionCompensation);

    bool writeRaw(std::ofstream &out);

    bool writeI(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix);

    bool writeP(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix, const Frame &previousFrame,
                uint16_t gop, uint16_t merange);

private:
    static int numInstances, frameBufferSize;
    static uint8_t *frameBuffer;
    static int16_t *pixels;

    const int width, height;
    const int rawSize, numBlocks, numMacroBlocks;
    const int numBlocksPerRow, numBlocksPerCol, numMacroBlocksPerRow, numMacroBlocksPerCol;
    ValueBlock4x4 **const blocks;

    void loadPixels() const;

    void readMacroBlock(int16_t *buffer, int macroRow, int macroCol) const;

    void writeMacroBlock(int16_t *buffer, int macroRow, int macroCol);
};


#endif //INC_3_VIDEO_COMPRESSION_FRAME_H
