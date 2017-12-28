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

    bool readP(std::ifstream &in, bool rle, const ValueBlock4x4 &quantMatrix, const Frame &previousFrame,
               uint16_t merange, bool motionCompensation);

    bool writeRaw(std::ofstream &out);

    bool writeI(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix);

    bool writeP(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix, const Frame &previousFrame,
                uint16_t merange);

private:
    static int numInstances, frameBufferSize;
    static uint8_t *frameBuffer;
    static int16_t *pixels;

    const int width, height;
    const int rawSize, numBlocks, numMacroBlocks;
    const int numBlocksPerRow, numBlocksPerCol, numMacroBlocksPerRow, numMacroBlocksPerCol;
    ValueBlock4x4 **const blocks;

    bool loadI(uint8_t *data, int compressedSize, bool rle, const ValueBlock4x4 &quantMatrix);

    bool loadP(uint8_t *vectorBuffer, int vectorStreamSize, const ValueBlock4x4 &quantMatrix,
               const Frame &previousFrame, uint16_t merange, bool motionCompensation);

    void loadPixels() const;

    void readMacroBlock(int16_t *buffer, int macroRow, int macroCol) const;

    void writeMacroBlock(int16_t *buffer, int macroRow, int macroCol);

    void findClosestMatch(int16_t *macroBlock, int row, int col, uint16_t merange, int &matchRow, int &matchCol) const;

    void getMotionCompensation(int16_t *macroBlock, int row, int col);

    void applyMotionCompensation(int16_t *macroBlock, int row, int col);

    int getDifferenceIfLower(const int16_t *macroBlock, int row, int col, int currentMinDifference) const;
};


#endif //INC_3_VIDEO_COMPRESSION_FRAME_H
