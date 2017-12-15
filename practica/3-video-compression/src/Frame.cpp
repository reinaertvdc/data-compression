#include <bitset>
#include <fstream>
#include <iostream>
#include "Frame.h"

int Frame::numInstances = 0;
int Frame::frameBufferSize = 0;
uint8_t *Frame::frameBuffer = nullptr;
int16_t Frame::blockBuffer[4][4];

Frame::~Frame() {
    prepareForRead(0, 0);

    numInstances--;

    if (numInstances == 0) {
        delete[] frameBuffer;
        frameBuffer = nullptr;
        frameBufferSize = 0;
    }
}

void Frame::prepareForRead(int width, int height) {
    if (numBlocks > 0) {
        for (int i = 0; i < numBlocks; i++) {
            delete blocks[i];
        }

        delete[] blocks;
    }

    this->rawSize = (int) (width * height * 1.5);
    this->numBlocks = (width * height) / 16;
    this->blocks = (numBlocks > 0) ? (new ValueBlock4x4*[numBlocks]) : nullptr;

    if (rawSize > frameBufferSize) {
        delete[] frameBuffer;
        frameBuffer = new uint8_t[rawSize];
        frameBufferSize = rawSize;
    }
}

bool Frame::readRaw(std::ifstream &in, int width, int height) {
    prepareForRead(width, height);

    in.read(reinterpret_cast<char *>(frameBuffer), rawSize);

    int byteIndex = 0;

    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++) {
        for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
            for (int colIndex = 0; colIndex < 4; colIndex++) {
                blockBuffer[rowIndex][colIndex] = frameBuffer[byteIndex];
                byteIndex++;
            }
        }

        blocks[blockIndex] = new ValueBlock4x4(blockBuffer);
    }

    return true;
}

bool Frame::readI(std::ifstream &in, int width, int height) {
    prepareForRead(width, height);

    in.read(reinterpret_cast<char *>(frameBuffer), width * height);

    int byteIndex = 0;

    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++) {
        for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
            for (int colIndex = 0; colIndex < 4; colIndex++) {
                blockBuffer[rowIndex][colIndex] = frameBuffer[byteIndex];
                byteIndex++;
            }
        }

        blocks[blockIndex] = new ValueBlock4x4(blockBuffer);
    }

    return true;
}

bool Frame::readP(std::ifstream &in) {
    return false;
}

bool Frame::writeRaw(std::ofstream &out) {
    int byteIndex = 0;

    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++) {
        auto raw = blocks[blockIndex]->getData();

        for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
            for (int colIndex = 0; colIndex < 4; colIndex++) {
                frameBuffer[byteIndex] = (uint8_t) *(raw + rowIndex * 4 + colIndex);
                byteIndex++;
            }
        }
    }

    for (; byteIndex < rawSize; byteIndex++) {
        frameBuffer[byteIndex] = 127;
    }

    out.write(reinterpret_cast<const char *>(frameBuffer), byteIndex);

    return true;
}

bool Frame::writeI(std::ofstream &out) {
    int byteIndex = 0;

    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++) {
        auto raw = blocks[blockIndex]->getData();

        for (int rowIndex = 0; rowIndex < 4; rowIndex++) {
            for (int colIndex = 0; colIndex < 4; colIndex++) {
                frameBuffer[byteIndex] = (uint8_t) *(raw + rowIndex * 4 + colIndex);
                byteIndex++;
            }
        }
    }

    out.write(reinterpret_cast<const char *>(frameBuffer), byteIndex);

    return true;
}

bool Frame::writeP(std::ofstream &out) {
    return false;
}
