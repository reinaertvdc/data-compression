#include <bitset>
#include <fstream>
#include "Frame.h"

int Frame::numInstances = 0;
int Frame::frameBufferSize = 0;
uint8_t *Frame::frameBuffer = nullptr;

Frame::Frame(uint16_t width, uint16_t height) : width(width), height(height),
                                                rawSize((int) (width * height * 1.5)), numBlocks((width * height) / 16),
                                                blocks(new ValueBlock4x4 *[numBlocks]) {
    numInstances++;

    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++) {
        blocks[blockIndex] = new ValueBlock4x4();
    }

    if (rawSize > frameBufferSize) {
        delete[] frameBuffer;
        frameBuffer = new uint8_t[rawSize];
        frameBufferSize = rawSize;
    }
}

Frame::~Frame() {
    numInstances--;

    for (int i = 0; i < numBlocks; i++) {
        delete blocks[i];
    }

    delete[] blocks;

    if (numInstances == 0) {
        delete[] frameBuffer;
        frameBuffer = nullptr;
        frameBufferSize = 0;
    }
}

bool Frame::readRaw(std::ifstream &in) {
    in.read(reinterpret_cast<char *>(frameBuffer), rawSize);

    for (int blockIndex = 0, byteIndex = 0; blockIndex < numBlocks; blockIndex++, byteIndex += 16) {
        blocks[blockIndex]->fromUint8Buffer(frameBuffer + byteIndex);
    }

    return true;
}

bool Frame::readI(std::ifstream &in, bool rle, const ValueBlock4x4 &quantMatrix) {
    in.read(reinterpret_cast<char *>(frameBuffer), width * height);

    for (int blockIndex = 0, byteIndex = 0; blockIndex < numBlocks; blockIndex++, byteIndex += 16) {
        blocks[blockIndex]->fromUint8Buffer(frameBuffer + byteIndex);
    }

    return true;
}

bool Frame::readP(std::ifstream &in, const Frame &previousFrame, uint16_t gop, uint16_t merange,
                  bool motionCompensation) {
    return readRaw(in);
}

bool Frame::writeRaw(std::ofstream &out) {
    int byteIndex = 0;

    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++, byteIndex += 16) {
        blocks[blockIndex]->toUint8Buffer(frameBuffer + byteIndex);
    }

    for (; byteIndex < rawSize; byteIndex++) {
        frameBuffer[byteIndex] = 127;
    }

    out.write(reinterpret_cast<const char *>(frameBuffer), byteIndex);

    return true;
}

bool Frame::writeI(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix) {
    int byteIndex = 0;

    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++, byteIndex += 16) {
        blocks[blockIndex]->toUint8Buffer(frameBuffer + byteIndex);
    }

    out.write(reinterpret_cast<const char *>(frameBuffer), byteIndex);

    return true;
}

bool Frame::writeP(std::ofstream &out, const Frame &previousFrame, uint16_t gop, uint16_t merange) {
    return writeRaw(out);
}
