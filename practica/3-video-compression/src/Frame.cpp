#include <bitset>
#include <fstream>
#include <cstring>
#include <iostream>
#include <ctgmath>
#include "Frame.h"
#include "RleCodec.h"
#include "IFrameCodec.h"
#include "BitStream.h"

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
    int compressedSize;

    in.read(reinterpret_cast<char *>(&compressedSize), sizeof(int));
    in.read(reinterpret_cast<char *>(frameBuffer), compressedSize);

    return loadI(frameBuffer, compressedSize, rle, quantMatrix);
}

bool Frame::loadI(uint8_t *data, int compressedSize, bool rle, const ValueBlock4x4 &quantMatrix) {
    int encodedSize;

    int16_t *encoded = IFrameCodec::fromStorageFormat(data, compressedSize, encodedSize, width, height, rle,
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
                  uint16_t merange, bool motionCompensation) {
    previousFrame.loadPixels();

    auto bitsPerVectorDim = (int) ceil(log2(merange * 2 + 1));
    auto vectorStreamSize = (int) ceil((numMacroBlocks * bitsPerVectorDim * 2) / 8.0);
    uint8_t vectorBuffer[vectorStreamSize];
    in.read(reinterpret_cast<char *>(&vectorBuffer), sizeof(uint8_t) * vectorStreamSize);

    readI(in, rle, quantMatrix);

    return loadP(vectorBuffer, vectorStreamSize, quantMatrix, previousFrame, merange, motionCompensation);
}

bool Frame::loadP(uint8_t *vectorBuffer, int vectorStreamSize, const ValueBlock4x4 &quantMatrix,
                  const Frame &previousFrame, uint16_t merange, bool motionCompensation) {
    auto bitsPerVectorDim = (int) ceil(log2(merange * 2 + 1));

    int16_t macroBlock[64];

    if (motionCompensation) {
        util::BitStreamReader vectorStream(vectorBuffer, vectorStreamSize);

        for (int macroRow = 0; macroRow < numMacroBlocksPerCol; macroRow++) {
            for (int macroCol = 0; macroCol < numMacroBlocksPerRow; macroCol++) {
                readMacroBlock(macroBlock, macroRow, macroCol);

                int matchRow = vectorStream.get(bitsPerVectorDim) + macroRow * 8 - merange;
                int matchCol = vectorStream.get(bitsPerVectorDim) + macroCol * 8 - merange;

                applyMotionCompensation(macroBlock, matchRow, matchCol);

                writeMacroBlock(macroBlock, macroRow, macroCol);
            }
        }
    }

    return true;
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

    loadI(data, size, rle, quantMatrix);

    delete[] data;

    return true;
}

bool Frame::writeP(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix, const Frame &previousFrame,
                   uint16_t merange) {
    previousFrame.loadPixels();

    int16_t macroBlock[64];

    auto bitsPerVectorDim = (int) ceil(log2(merange * 2 + 1));
    auto vectorStreamSize = (int) ceil((numMacroBlocks * bitsPerVectorDim * 2) / 8.0);
    uint8_t vectorBuffer[vectorStreamSize];
    util::BitStreamWriter vectorStream(vectorBuffer, vectorStreamSize);

    for (int macroRow = 0; macroRow < numMacroBlocksPerCol; macroRow++) {
        for (int macroCol = 0; macroCol < numMacroBlocksPerRow; macroCol++) {
            readMacroBlock(macroBlock, macroRow, macroCol);

            int matchRow, matchCol;

            findClosestMatch(macroBlock, macroRow * 8, macroCol * 8, merange, matchRow, matchCol);

            vectorStream.put(bitsPerVectorDim, (uint32_t) (matchRow - macroRow * 8 + merange));
            vectorStream.put(bitsPerVectorDim, (uint32_t) (matchCol - macroCol * 8 + merange));

            getMotionCompensation(macroBlock, matchRow, matchCol);

            writeMacroBlock(macroBlock, macroRow, macroCol);
        }
    }

    out.write(reinterpret_cast<const char *>(&vectorBuffer), sizeof(uint8_t) * vectorStreamSize);

    writeI(out, rle, quantMatrix);

    loadP(vectorBuffer, vectorStreamSize, quantMatrix, previousFrame, merange, true);
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

void Frame::findClosestMatch(int16_t *macroBlock, int row, int col, uint16_t merange,
                             int &matchRow, int &matchCol) const {
    int beginRow = std::max(0, row - merange);
    int endRow = std::min(height - 16, row + merange);
    int beginCol = std::max(0, col - merange);
    int endCol = std::min(width - 16, col + merange);

    int minDifference = 255 * 2 * 64 + 1;

    matchRow = row;
    matchCol = col;

    for (int i = beginRow; i < endRow; i++) {
        for (int j = beginCol; j < endCol; j++) {
            int difference = getDifference(macroBlock, i, j);

            if (difference < minDifference) {
                minDifference = difference;

                matchRow = i;
                matchCol = j;
            }
        }
    }
}

void Frame::getMotionCompensation(int16_t *macroBlock, int row, int col) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int16_t prevPixel = pixels[width * (row + i) + col + j];
            int16_t *pixel = &macroBlock[i * 8 + j];

            pixel[0] = (int16_t) ((pixel[0] - prevPixel + 128) % 256);

            if (pixel[0] < 0) {
                pixel[0] += 256;
            }
        }
    }
}

void Frame::applyMotionCompensation(int16_t *macroBlock, int row, int col) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int16_t prevPixel = pixels[width * (row + i) + col + j];
            int16_t *pixel = &macroBlock[i * 8 + j];

            pixel[0] = (int16_t) ((pixel[0] + prevPixel - 128) % 256);

            if (pixel[0] < 0) {
                pixel[0] += 256;
            }
        }
    }
}

int Frame::getDifference(const int16_t *macroBlock, int row, int col) const {
    int difference = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            int16_t prevPixel = pixels[width * (row + i) + col + j];
            int16_t pixel = macroBlock[i * 8 + j];

            difference += std::abs(std::abs(pixel) - std::abs(prevPixel));
        }
    }

    return difference;
}
