#ifndef INC_3_VIDEO_COMPRESSION_FRAME_H
#define INC_3_VIDEO_COMPRESSION_FRAME_H


#include <vector>
#include "ValueBlock4x4.h"

class Frame {
public:
    Frame() { numInstances++; }

    ~Frame();

    bool readRaw(std::ifstream &in, int width, int height);

    bool readI(std::ifstream &in, int width, int height);

    bool readP(std::ifstream &in);

    bool writeRaw(std::ofstream &out);

    bool writeI(std::ofstream &out);

    bool writeP(std::ofstream &out);

private:
    static int numInstances;
    static int frameBufferSize;
    static uint8_t *frameBuffer;
    static int16_t blockBuffer[4][4];

    int rawSize = 0;
    int numBlocks = 0;
    ValueBlock4x4 **blocks = nullptr;

    void prepareForRead(int width, int height);
};


#endif //INC_3_VIDEO_COMPRESSION_FRAME_H
