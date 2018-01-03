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
#include "CompactingCodec.h"
#include <time.h>

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

    uint32_t vectorStreamSize;
    in.read((char *)(&vectorStreamSize), 4);
    uint8_t vectorBuffer[vectorStreamSize];
    in.read((char *)(&vectorBuffer), vectorStreamSize);
    util::BitStreamReader vectorStream(vectorBuffer, vectorStreamSize);
    int16_t *vectorBufferX = CompactingCodec::decompact(numMacroBlocks, vectorStream, 4, 4);
    int16_t *vectorBufferY = CompactingCodec::decompact(numMacroBlocks, vectorStream, 4, 4);

    readI(in, rle, quantMatrix);

    bool ret = loadP(vectorBufferX, vectorBufferY, vectorStreamSize, quantMatrix, previousFrame, merange, motionCompensation);
    delete[] vectorBufferX;
    delete[] vectorBufferY;
    return ret;
}

bool Frame::loadP(int16_t *vectorBufferX, int16_t *vectorBufferY, int vectorStreamSize, const ValueBlock4x4 &quantMatrix,
                  const Frame &previousFrame, uint16_t merange, bool motionCompensation) {
    int16_t macroBlock[macroBlockSize];
    for (int macroRow = 0; macroRow < numMacroBlocksPerCol; macroRow++) {
        for (int macroCol = 0; macroCol < numMacroBlocksPerRow; macroCol++) {

            int macroIndex = macroRow * numMacroBlocksPerRow + macroCol;
            if (motionCompensation) {
                readMacroBlock(macroBlock, macroRow, macroCol);
            }
            else {
                for (int i = 0; i < macroBlockSize; i++) {
                    macroBlock[i] = 0;
                }
            }
            int matchRow = static_cast<int>(vectorBufferY[macroIndex]) + macroRow * macroBlockHeight;
            int matchCol = static_cast<int>(vectorBufferX[macroIndex]) + macroCol * macroBlockWidth;
            applyMotionCompensation(macroBlock, matchRow, matchCol);
            writeMacroBlock(macroBlock, macroRow, macroCol);
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

bool Frame::writeI(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix, double &compressionTime) {
    int size = 0;
    clock_t t1  = clock();
    uint8_t *data = IFrameStorageCodec::toStorageFormat(blocks, quantMatrix, size, width, height, rle, false);
    clock_t t2 = clock();
    uint32_t tmpSize = static_cast<uint32_t>(size);
    out.write((const char *)(&tmpSize), sizeof(uint32_t));
    out.write((const char *)(data), size);
    clock_t t3 = clock();
    loadI(data, size, rle, quantMatrix);
    delete[] data;
    clock_t t4 = clock();

    compressionTime = (double)((t2 - t1) + (t4 - t3)) / (double)CLOCKS_PER_SEC;

    return true;
}

bool Frame::writeP(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix, const Frame &previousFrame,
                   uint16_t merange, double &compressionTime) {
    clock_t t1 = clock();
    previousFrame.loadPixels();
    int16_t macroBlock[macroBlockSize];
    int16_t vectorBufferX[numMacroBlocks];
    int16_t vectorBufferY[numMacroBlocks];
    for (int macroRow = 0; macroRow < numMacroBlocksPerCol; macroRow++) {
        for (int macroCol = 0; macroCol < numMacroBlocksPerRow; macroCol++) {
            readMacroBlock(macroBlock, macroRow, macroCol);
            int matchRow, matchCol;
            findClosestMatch(macroBlock, macroRow * macroBlockWidth, macroCol * macroBlockHeight, merange, matchRow, matchCol);
            int macroIndex = macroRow * numMacroBlocksPerRow + macroCol;
            vectorBufferY[macroIndex] = static_cast<int16_t>(matchRow - macroRow * macroBlockHeight);
            vectorBufferX[macroIndex] = static_cast<int16_t>(matchCol - macroCol * macroBlockWidth);
            getMotionCompensation(macroBlock, matchRow, matchCol);
            writeMacroBlock(macroBlock, macroRow, macroCol);
        }
    }
    util::BitStreamWriter vectorStream(new uint8_t[numMacroBlocks*2], numMacroBlocks*2);
    CompactingCodec::compact(vectorBufferX, numMacroBlocks, vectorStream, 4, 4);
    CompactingCodec::compact(vectorBufferY, numMacroBlocks, vectorStream, 4, 4);
    vectorStream.flush();
    uint32_t vectorStreamSize = static_cast<uint32_t>(vectorStream.get_position() / 8);
    clock_t t2 = clock();
    out.write((const char*)(&vectorStreamSize), 4);
    out.write(reinterpret_cast<const char *>(vectorStream.get_buffer()), sizeof(uint8_t) * vectorStreamSize);
    double tTmp = 0;
    writeI(out, rle, quantMatrix, tTmp);
    clock_t t3 = clock();
    loadP(vectorBufferX, vectorBufferY, vectorStreamSize, quantMatrix, previousFrame, merange, true);
    delete[] vectorStream.get_buffer();
    clock_t t4 =  clock();
    compressionTime = tTmp + (double)((t2 - t1) + (t4 - t3)) / (double)CLOCKS_PER_SEC;
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
    int endRow = std::min(height - macroBlockHeight, row + merange + 1);
    int beginCol = std::max(0, col - merange);
    int endCol = std::min(width - macroBlockWidth, col + merange + 1);

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
            int16_t &pixel = macroBlock[i * macroBlockWidth + j];
            pixel = pixel - prevPixel;
        }
    }
}

void Frame::applyMotionCompensation(int16_t *macroBlock, int row, int col) {
//    for (int i = 0; i < macroBlockHeight; i++) {
//        for (int j = 0; j < macroBlockWidth; j++) {
//            std::cout << macroBlock[i * macroBlockWidth + j] << "\t";
//        }
//        std::cout << std::endl;
//    }

    for (int i = 0; i < macroBlockWidth; i++) {
        for (int j = 0; j < macroBlockHeight; j++) {
            int16_t prevPixel = pixels[width * (row + i) + col + j];
            int16_t &pixel = macroBlock[i * macroBlockWidth + j];
            pixel = pixel + prevPixel;
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
