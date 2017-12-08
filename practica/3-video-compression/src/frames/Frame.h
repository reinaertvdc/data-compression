#ifndef INC_3_VIDEO_COMPRESSION_FRAME_H
#define INC_3_VIDEO_COMPRESSION_FRAME_H


#include <vector>
#include "../helpers/ValueBlock4x4.h"

class Frame {
public:
    Frame(const uint8_t buffer[], int width, int height);

    ~Frame();

    void raw(uint8_t buffer[]);

private:
    const int width;
    const int height;
    const int numBlocks;
    ValueBlock4x4 *image;
};


#endif //INC_3_VIDEO_COMPRESSION_FRAME_H
