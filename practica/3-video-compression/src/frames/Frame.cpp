#include <bitset>
#include <iostream>
#include "Frame.h"

Frame::Frame(const uint8_t buffer[], int width, int height) : width(width), height(height),
                                                              numBlocks((width * height) / 16),
                                                              image(new ValueBlock4x4[numBlocks]) {
    int16_t raw[4][4];
    int byteIndex = 0;

    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++) {
        for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
            for (int colIndex = 0; colIndex < 4; colIndex++) {
                raw[rowIndex][colIndex] = buffer[byteIndex];
                byteIndex++;
            }
        }

        image[blockIndex] = ValueBlock4x4(raw);
    }
}

Frame::~Frame() {
    delete[] image;
}

void Frame::raw(uint8_t *buffer) {
    int byteIndex = 0;

    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++) {
        auto raw = image[blockIndex].getData();

        for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
            for (int colIndex = 0; colIndex < 4; colIndex++) {
                buffer[byteIndex] = (uint8_t)*(raw + rowIndex * 4 + colIndex);
                byteIndex++;
            }
        }
    }

    auto bufferSize = (int)((byteIndex + 1) * 1.5);

    for (; byteIndex < bufferSize; byteIndex++) {
        buffer[byteIndex] = 127;
    }
}
