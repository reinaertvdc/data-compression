#include <bitset>
#include <fstream>
#include <cstring>
#include <iostream>
#include "Frame.h"
#include "RleCodec.h"
#include "IFrameCodec.h"

int Frame::numInstances = 0;
int Frame::frameBufferSize = 0;
uint8_t *Frame::frameBuffer = nullptr;
int16_t *Frame::pixels = nullptr;

Frame::Frame(int width, int height) : width(width), height(height), rawSize((int) (width * height * 1.5)),
                                      numBlocks((width * height) / 16), numMacroBlocks((width * height) / 64),
                                      numBlocksPerRow(width / 4), numBlocksPerCol(height / 4),
                                      numMacroBlocksPerRow(width / 8), numMacroBlocksPerCol(height / 8),
                                      blocks(new ValueBlock4x4 *[numBlocks]) {
    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++) {
        blocks[blockIndex] = new ValueBlock4x4();
    }

    if (rawSize > frameBufferSize) {
        delete[] frameBuffer;
        frameBuffer = new uint8_t[rawSize];
        frameBufferSize = rawSize;

        delete[] pixels;
        pixels = new int16_t[width * height];
    }

    numInstances++;
}

bool Frame::copy(const Frame &other) {
    if (width != other.width || height != other.height) {
        return false;
    }

    for (int i = 0; i < numBlocks; i++) {
        blocks[i]->copy(*other.blocks[i]);
    }

    return true;
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

        delete[] pixels;
    }
}

bool Frame::readRaw(std::ifstream &in) {
    in.read(reinterpret_cast<char *>(frameBuffer), rawSize);

    for (int blockRow = 0, byteIndex = 0; blockRow < numBlocksPerCol; blockRow++, byteIndex += width * 3) {
        for (int blockCol = 0; blockCol < numBlocksPerRow; blockCol++, byteIndex += 4) {
            blocks[blockRow * numBlocksPerRow + blockCol]->fromUint8Buffer(frameBuffer + byteIndex, width);
        }
    }

    return true;
}

bool Frame::readI(std::ifstream &in, bool rle, const ValueBlock4x4 &quantMatrix) {
    int compressedSize, encodedSize;

    in.read(reinterpret_cast<char *>(&compressedSize), sizeof(int));
    in.read(reinterpret_cast<char *>(frameBuffer), compressedSize);

    int16_t *encoded = IFrameCodec::fromStorageFormat(frameBuffer, compressedSize, encodedSize, width, height, rle,
                                                      quantMatrix);

    int indexEncodedImage = 0;
    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++) {
        ValueBlock4x4 &block = *blocks[blockIndex];
        int tmpInSizeUsed;
        int16_t *zzPattern;
        if (rle) {
            zzPattern = RleCodec::rleDecode(&encoded[indexEncodedImage], 16, tmpInSizeUsed);
            indexEncodedImage += tmpInSizeUsed;
            block = ValueBlock4x4(zzPattern);
            delete[] zzPattern;
        } else {
            zzPattern = &encoded[indexEncodedImage];
            indexEncodedImage += 16;
            block = ValueBlock4x4(zzPattern);
        }
        block.deQuantize(quantMatrix);
        block.applyInverseDct();
    }

    delete[] encoded;

    return true;
}

bool Frame::readP(std::ifstream &in, bool rle, const ValueBlock4x4 &quantMatrix, const Frame &previousFrame,
                  uint16_t gop, uint16_t merange, bool motionCompensation) {
    return readRaw(in);
}

bool Frame::writeRaw(std::ofstream &out) {
    int byteIndex = 0;

    for (int blockRow = 0; blockRow < numBlocksPerCol; blockRow++, byteIndex += width * 3) {
        for (int blockCol = 0; blockCol < numBlocksPerRow; blockCol++, byteIndex += 4) {
            blocks[blockRow * numBlocksPerRow + blockCol]->toUint8Buffer(frameBuffer + byteIndex, width);
        }
    }

    for (; byteIndex < rawSize; byteIndex++) {
        frameBuffer[byteIndex] = 128;
    }

    out.write(reinterpret_cast<const char *>(frameBuffer), byteIndex);

    return true;
}

bool Frame::writeI(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix) {
    int16_t rleOutput[width * height * (4 * 4 + 1) / (4 * 4)];
    int iRleTmpOut = 0;

    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++) {
        ValueBlock4x4 &block = *blocks[blockIndex];

        block.applyDct();
        block.quantize(quantMatrix);

        int16_t zzOutput[16];
        block.zigzag(zzOutput);

        if (rle) {
            int len;
            int16_t *rleBuffer = RleCodec::rleEncode(zzOutput, 16, len);

            memcpy(&rleOutput[iRleTmpOut], rleBuffer, len * sizeof(int16_t));
            delete[] rleBuffer;
            iRleTmpOut += len;
        } else {
            memcpy(&rleOutput[iRleTmpOut], zzOutput, 16 * sizeof(int16_t));
            iRleTmpOut += 16;
        }
    }

    int size;
    uint8_t *data = IFrameCodec::toStorageFormat(rleOutput, iRleTmpOut, size, width, height, rle, quantMatrix);

    out.write(reinterpret_cast<const char *>(&size), sizeof(int));
    out.write(reinterpret_cast<const char *>(data), size);

    delete[] data;

    return true;
}

bool Frame::writeP(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix, const Frame &previousFrame,
                   uint16_t gop, uint16_t merange) {
    loadPixels();

    int16_t macroBlock[64];

    for (int macroRow = 0; macroRow < numMacroBlocksPerCol; macroRow++) {
        for (int macroCol = 0; macroCol < numMacroBlocksPerRow; macroCol++) {
            readMacroBlock(macroBlock, macroRow, macroCol);

            // TODO Encode macro block
            //   1. Find closest match
            //     1. Loop over candidates
            //       1. Compare with candidate
            //   2. Store macro block
            //     1. Store vector
            //     2. Store motion compensation
            //       1. Get motion compensation

            writeMacroBlock(macroBlock, macroRow, macroCol);
        }
    }

    return writeRaw(out);
}

void Frame::readMacroBlock(int16_t *buffer, int macroRow, int macroCol) const {
    for (int rowOffset = 0; rowOffset < 2; rowOffset++) {
        for (int colOffset = 0; colOffset < 2; colOffset++) {
            int16_t *block = blocks[(macroRow * 2 + rowOffset) * numBlocksPerRow + macroCol * 2 + colOffset]->getData();

            for (int i = 0, j = rowOffset * 32 + colOffset * 4; i < 16; i += 4, j += 8) {
                memcpy(&buffer[j], &block[i], sizeof(int16_t) * 4);
            }
        }
    }
}

void Frame::writeMacroBlock(int16_t *buffer, int macroRow, int macroCol) {
    for (int rowOffset = 0; rowOffset < 2; rowOffset++) {
        for (int colOffset = 0; colOffset < 2; colOffset++) {
            int16_t *block = blocks[(macroRow * 2 + rowOffset) * numBlocksPerRow + macroCol * 2 + colOffset]->getData();

            for (int i = 0, j = rowOffset * 32 + colOffset * 4; i < 16; i += 4, j += 8) {
                memcpy(&block[i], &buffer[j], sizeof(int16_t) * 4);
            }
        }
    }
}

void Frame::loadPixels() const {
    for (int blockRow = 0; blockRow < numBlocksPerCol; blockRow++) {
        int16_t *blockRowPixels = &pixels[numBlocksPerRow * blockRow * 16];

        for (int blockCol = 0; blockCol < numBlocksPerRow; blockCol++) {
            int16_t *blockPixels = &blockRowPixels[blockCol * 4];
            int16_t *block = blocks[blockRow * numBlocksPerRow + blockCol]->getData();

            for (int row = 0; row < 4; row++) {
                memcpy(blockPixels, block, sizeof(int16_t) * 4);

                blockPixels = &blockPixels[width];
                block = &block[4];
            }
        }
    }
}
