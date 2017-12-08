#include <bitset>
#include <iostream>
#include "Frame.h"

Frame::Frame(const uint8_t buffer[], int width, int height) : width(width), height(height),
                                                              numBlocks((width * height) / 16),
                                                              image(new ValueBlock4x4[numBlocks]) {
    uint8_t raw[16];
    int16_t forBlocks[4][4];

    for (int blockIndex = 0, byteIndex = 0; blockIndex < numBlocks; blockIndex++) {
        for (int i = 0, pixelIndex = 0; i < 8; i++, pixelIndex += 2, byteIndex += 3) {
            raw[pixelIndex] = buffer[byteIndex];
            raw[pixelIndex + 1] = (buffer[byteIndex + 1] << 4) + (buffer[byteIndex + 2] >> 4);
        }

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                forBlocks[i][j] = 0;
                forBlocks[i][j] = raw[i * 4 + j];
            }
        }

        image[blockIndex] = ValueBlock4x4(forBlocks);
    }
}

Frame::~Frame() {
    delete[] image;
}

void Frame::raw(uint8_t *buffer) {
    for (int blockIndex = 0, byteIndex = 0; blockIndex < numBlocks; blockIndex++) {
        auto *raw = image[blockIndex].getData();

        for (int pixelIndex = 0; pixelIndex < 16; pixelIndex += 2, byteIndex += 3) {
            buffer[byteIndex] = raw[pixelIndex];
            buffer[byteIndex + 1] = raw[pixelIndex + 1] >> 4;
            buffer[byteIndex + 2] = raw[pixelIndex + 1] << 4;
        }
    }
}
