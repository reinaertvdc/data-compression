#include <bitset>
#include <fstream>
#include <cstring>
#include <iostream>
#include <ctgmath>
#include <chrono>
#include "Frame.h"
#include "RleCodec.h"
#include "IFrameStorageCodec.h"
#include "BitStream.h"
#include "Logger.h"

int Frame::numInstances = 0;
int Frame::frameBufferSize = 0;
uint8_t *Frame::frameBuffer = nullptr;
int16_t *Frame::pixels = nullptr;

Frame::Frame(int width, int height) : width(width), height(height), rawSize((int) (width * height * rawFrameSizeToPixelsRatio)),
                                      numBlocks((width * height) / blockSize), numMacroBlocks((width * height) / macroBlockSize),
                                      numBlocksPerRow(width / blockWidth), numBlocksPerCol(height / blockHeight),
                                      numMacroBlocksPerRow(width / macroBlockWidth), numMacroBlocksPerCol(height / macroBlockHeight),
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

    for (int blockRow = 0, byteIndex = 0; blockRow < numBlocksPerCol; blockRow++, byteIndex += width * (blockHeight - 1)) {
        for (int blockCol = 0; blockCol < numBlocksPerRow; blockCol++, byteIndex += blockWidth) {
            blocks[blockRow * numBlocksPerRow + blockCol]->fromUint8Buffer(frameBuffer + byteIndex, width);
        }
    }

    return true;
}

bool Frame::readI(std::ifstream &in, bool rle, const ValueBlock4x4 &quantMatrix) {
    uint32_t tmpSize;
    int compressedSize;
    in.read((char *)(&tmpSize), sizeof(uint32_t));
    compressedSize = static_cast<int>(tmpSize);
    in.read((char *)(frameBuffer), compressedSize);

    return loadI(frameBuffer, compressedSize, rle, quantMatrix);
}

bool Frame::loadI(uint8_t *data, int compressedSize, bool rle, const ValueBlock4x4 &quantMatrix) {

    int size;
    IFrameStorageCodec::fromStorageFormat(data, compressedSize, quantMatrix, size, width, height, rle, false, blocks);

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

    int16_t macroBlock[macroBlockSize];

    if (motionCompensation) {
        util::BitStreamReader vectorStream(vectorBuffer, vectorStreamSize);

        for (int macroRow = 0; macroRow < numMacroBlocksPerCol; macroRow++) {
            for (int macroCol = 0; macroCol < numMacroBlocksPerRow; macroCol++) {
                readMacroBlock(macroBlock, macroRow, macroCol);

                int matchRow = vectorStream.get(bitsPerVectorDim) + macroRow * macroBlockWidth - merange;
                int matchCol = vectorStream.get(bitsPerVectorDim) + macroCol * macroBlockHeight - merange;

                applyMotionCompensation(macroBlock, matchRow, matchCol);

                writeMacroBlock(macroBlock, macroRow, macroCol);
            }
        }
    }

    return true;
}

bool Frame::writeRaw(std::ofstream &out) {
    int byteIndex = 0;

    for (int blockRow = 0; blockRow < numBlocksPerCol; blockRow++, byteIndex += width * (blockHeight - 1)) {
        for (int blockCol = 0; blockCol < numBlocksPerRow; blockCol++, byteIndex += blockWidth) {
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
    int size = 0;
    uint8_t *data = IFrameStorageCodec::toStorageFormat(blocks, quantMatrix, size, width, height, rle, false);
    uint32_t tmpSize = static_cast<uint32_t>(size);
    out.write((const char *)(&tmpSize), sizeof(uint32_t));
    out.write((const char *)(data), size);
    loadI(data, size, rle, quantMatrix);

    delete[] data;

//    int16_t rleOutput[width * height * (4 * 4 + 1) / (4 * 4)];
//    int iRleTmpOut = 0;
//
//    for (int blockIndex = 0; blockIndex < numBlocks; blockIndex++) {
//        ValueBlock4x4 &block = *blocks[blockIndex];
//
//        block.applyDct();
//        block.quantize(quantMatrix);
//
//        int16_t zzOutput[blockSize];
//        block.zigzag(zzOutput);
//
//        if (rle) {
//            int len;
//            int16_t *rleBuffer = RleCodec::rleEncode(zzOutput, blockSize, len);
//
//            memcpy(&rleOutput[iRleTmpOut], rleBuffer, len * sizeof(int16_t));
//            delete[] rleBuffer;
//            iRleTmpOut += len;
//        } else {
//            memcpy(&rleOutput[iRleTmpOut], zzOutput, blockSize * sizeof(int16_t));
//            iRleTmpOut += blockSize;
//        }
//    }
//
//    int size;
//    uint8_t *data = IFrameStorageCodec::toStorageFormat(rleOutput, iRleTmpOut, size, width, height, rle);
//
//    delete[] data;

    return true;
}

bool Frame::writeP(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix, const Frame &previousFrame,
                   uint16_t merange) {
    previousFrame.loadPixels();

    int16_t macroBlock[macroBlockSize];

    auto bitsPerVectorDim = (int) ceil(log2(merange * 2 + 1));
    auto vectorStreamSize = (int) ceil((numMacroBlocks * bitsPerVectorDim * 2) / 8.0);
    uint8_t vectorBuffer[vectorStreamSize];
    util::BitStreamWriter vectorStream(vectorBuffer, vectorStreamSize);

    for (int macroRow = 0; macroRow < numMacroBlocksPerCol; macroRow++) {
        for (int macroCol = 0; macroCol < numMacroBlocksPerRow; macroCol++) {
            readMacroBlock(macroBlock, macroRow, macroCol);

            int matchRow, matchCol;

            findClosestMatch(macroBlock, macroRow * macroBlockWidth, macroCol * macroBlockHeight, merange, matchRow, matchCol);

            vectorStream.put(bitsPerVectorDim, (uint32_t) (matchRow - macroRow * macroBlockWidth + merange));
            vectorStream.put(bitsPerVectorDim, (uint32_t) (matchCol - macroCol * macroBlockHeight + merange));

            getMotionCompensation(macroBlock, matchRow, matchCol);

            writeMacroBlock(macroBlock, macroRow, macroCol);
        }
    }

    out.write(reinterpret_cast<const char *>(&vectorBuffer), sizeof(uint8_t) * vectorStreamSize);

    writeI(out, rle, quantMatrix);

    loadP(vectorBuffer, vectorStreamSize, quantMatrix, previousFrame, merange, true);
}

void Frame::readMacroBlock(int16_t *buffer, int macroRow, int macroCol) const {
    for (int rowOffset = 0; rowOffset < blocksPerMacroRow; rowOffset++) {
        for (int colOffset = 0; colOffset < blocksPerMacroCol; colOffset++) {
            int16_t *block = blocks[(macroRow * blocksPerMacroRow + rowOffset) * numBlocksPerRow + macroCol * blocksPerMacroCol + colOffset]->getData();

            for (int i = 0, j = rowOffset * blockSize * blocksPerMacroCol + colOffset * blockWidth; i < blockSize; i += blockWidth, j += macroBlockWidth) {
                memcpy(&buffer[j], &block[i], sizeof(int16_t) * blockWidth);
            }
        }
    }
}

void Frame::writeMacroBlock(int16_t *buffer, int macroRow, int macroCol) {
    for (int rowOffset = 0; rowOffset < blocksPerMacroRow; rowOffset++) {
        for (int colOffset = 0; colOffset < blocksPerMacroCol; colOffset++) {
            int16_t *block = blocks[(macroRow * blocksPerMacroRow + rowOffset) * numBlocksPerRow + macroCol * blocksPerMacroCol + colOffset]->getData();

            for (int i = 0, j = rowOffset * blockSize * blocksPerMacroCol + colOffset * blockWidth; i < blockSize; i += blockWidth, j += macroBlockWidth) {
                memcpy(&block[i], &buffer[j], sizeof(int16_t) * blockWidth);
            }
        }
    }
}

void Frame::loadPixels() const {
    for (int blockRow = 0; blockRow < numBlocksPerCol; blockRow++) {
        int16_t *blockRowPixels = &pixels[numBlocksPerRow * blockRow * blockSize];

        for (int blockCol = 0; blockCol < numBlocksPerRow; blockCol++) {
            int16_t *blockPixels = &blockRowPixels[blockCol * blockWidth];
            int16_t *block = blocks[blockRow * numBlocksPerRow + blockCol]->getData();

            for (int row = 0; row < blockHeight; row++) {
                memcpy(blockPixels, block, sizeof(int16_t) * blockWidth);

                blockPixels = &blockPixels[width];
                block = &block[blockWidth];
            }
        }
    }
}

void Frame::findClosestMatch(int16_t *macroBlock, int row, int col, uint16_t merange,
                             int &matchRow, int &matchCol) const {
    int beginRow = std::max(0, row - merange);
    int endRow = std::min(height - macroBlockHeight, row + merange);
    int beginCol = std::max(0, col - merange);
    int endCol = std::min(width - macroBlockWidth, col + merange);

    int minDifference = 255 * 2 * macroBlockSize + 1;

    matchRow = row;
    matchCol = col;

    for (int i = beginRow; i < endRow; i++) {
        for (int j = beginCol; j < endCol; j++) {
            int difference = getDifferenceNotHigher(macroBlock, i, j, minDifference);

            if (difference < minDifference || (difference == minDifference && (std::abs(row - i) + std::abs(col - j)) < std::abs(row - matchRow) + std::abs(col - matchCol))) {
                minDifference = difference;

                matchRow = i;
                matchCol = j;
            }
        }
    }
}

void Frame::getMotionCompensation(int16_t *macroBlock, int row, int col) {
    for (int i = 0; i < macroBlockWidth; i++) {
        for (int j = 0; j < macroBlockHeight; j++) {
            int16_t prevPixel = pixels[width * (row + i) + col + j];
            int16_t *pixel = &macroBlock[i * macroBlockWidth + j];

            pixel[0] = (int16_t) ((pixel[0] - prevPixel + 128) % 256);

            if (pixel[0] < 0) {
                pixel[0] += 256;
            }
        }
    }
}

void Frame::applyMotionCompensation(int16_t *macroBlock, int row, int col) {
    for (int i = 0; i < macroBlockWidth; i++) {
        for (int j = 0; j < macroBlockHeight; j++) {
            int16_t prevPixel = pixels[width * (row + i) + col + j];
            int16_t *pixel = &macroBlock[i * macroBlockWidth + j];

            pixel[0] = (int16_t) ((pixel[0] + prevPixel - 128) % 256);

            if (pixel[0] < 0) {
                pixel[0] += 256;
            }
        }
    }
}

inline int Frame::getDifferenceNotHigher(const int16_t *macroBlock, int row, int col, int currentMinDifference) const {
    int difference = 0;

    for (int i = 0; i < macroBlockWidth; i++) {
        for (int j = 0; j < macroBlockHeight; j++) {
            int16_t prevPixel = pixels[width * (row + i) + col + j];
            int16_t pixel = macroBlock[i * macroBlockWidth + j];

            difference += std::abs(pixel - prevPixel);
        }

        if (difference > currentMinDifference) {
            return difference;
        }
    }

    return difference;
}
