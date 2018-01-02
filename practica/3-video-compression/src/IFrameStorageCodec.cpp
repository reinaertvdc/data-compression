#include <iostream>
#include <cstring>
#include "IFrameStorageCodec.h"
#include "BitStream.h"
#include "RleCodec.h"
#include "CompactingCodec.h"

uint8_t *
IFrameStorageCodec::toStorageFormat(ValueBlock4x4 **pixelBlocks, const ValueBlock4x4 &quantMatrix, int &storageSize,
                                    int width, int height, bool rle, bool differentialEncodeDc) {
    if (width % 4 != 0 || height % 4 != 0) {
        return nullptr;
    }
    util::BitStreamWriter outBits(new uint8_t[width * height * 4], width * height * 4);
    int blockRows = height / 4;
    int blockCols = width / 4;
    int blockCount = blockRows * blockCols;
    int16_t dcComponents[blockCount];
    int16_t *acComponents[blockCount];
    for (int blockRow = 0; blockRow < blockRows; blockRow++) {
        for (int blockCol = 0; blockCol < blockCols; blockCol++) {
            int blockIndex = blockRow * blockCols + blockCol;
            ValueBlock4x4 &block = *pixelBlocks[blockIndex];
            block.applyDct();
            block.quantize(quantMatrix);
            block.zigzag();
            dcComponents[blockIndex] = block.getValue(0, 0);
            acComponents[blockIndex] = &(block.getData()[1]);
        }
    }
    if (differentialEncodeDc) {
        IFrameStorageCodec::differentialEncode(dcComponents, blockCount);
    }
    int dcComponentStorageCount = blockCount;
    int16_t *dcComponentStorageValues = dcComponents;
    if (rle) {
        int16_t *dcRleCounts;
        RleCodec::rleEncode(dcComponents, dcRleCounts, dcComponentStorageValues, blockCount, dcComponentStorageCount);
        CompactingCodec::compactSingleValue(static_cast<uint32_t>(dcComponentStorageCount), outBits, 8);
        CompactingCodec::compact(dcRleCounts, dcComponentStorageCount, outBits, 4, 4);
        delete[] dcRleCounts;
    } else {
        CompactingCodec::compactSingleValue(static_cast<uint32_t>(dcComponentStorageCount), outBits, 8);
    }
    CompactingCodec::compact(dcComponentStorageValues, dcComponentStorageCount, outBits, 4, 4);
    if (rle) {
        delete[] dcComponentStorageValues;
    }
    for (int i = 0; i < blockCount; i++) {
        int acComponentStorageCount = 15;
        int16_t *acComponentStorageValues = acComponents[i];
        if (rle) {
            int16_t *acZeroCounts;
            RleCodec::rleEncodeZeros(acComponents[i], acZeroCounts, acComponentStorageValues, 15,
                                     acComponentStorageCount, 15);
            CompactingCodec::compactSingleValue(static_cast<uint32_t>(acComponentStorageCount), outBits, 8);
            CompactingCodec::compact(acZeroCounts, acComponentStorageCount, outBits, 4, 4);
            delete[] acZeroCounts;
        } else {
            CompactingCodec::compactSingleValue(static_cast<uint32_t>(acComponentStorageCount), outBits, 8);
        }
        CompactingCodec::compact(acComponentStorageValues, acComponentStorageCount, outBits, 4, 4);
        if (rle) {
            delete[] acComponentStorageValues;
        }
    }
    outBits.flush();
    storageSize = outBits.get_position() / 8;
    return outBits.get_buffer();
}

bool IFrameStorageCodec::fromStorageFormat(uint8_t *data, int inSize, const ValueBlock4x4 &quantMatrix, int &outSize,
                                           int width, int height, bool rle, bool differentialEncodeAc,
                                           ValueBlock4x4 **pixelBlocks) {
    if (width % 4 != 0 || height % 4 != 0) {
        return false;
    }
    util::BitStreamReader inBits(data, inSize);
    int blockRows = height / 4;
    int blockCols = width / 4;
    int blockCount = blockRows * blockCols;
    int16_t *acComponents[blockCount];
    for (int blockRow = 0; blockRow < blockRows; blockRow++) {
        for (int blockCol = 0; blockCol < blockCols; blockCol++) {
            int blockIndex = blockRow * blockCols + blockCol;
            ValueBlock4x4 &block = *pixelBlocks[blockIndex];
            acComponents[blockIndex] = &(block.getData()[1]);
        }
    }
    int dcComponentStorageCount = CompactingCodec::decompactSingleValue(inBits, 8);
    int16_t *dcRleCounts = nullptr;
    if (rle) {
        dcRleCounts = CompactingCodec::decompact(dcComponentStorageCount, inBits, 4, 4);
    }
    int16_t *dcComponentStorageValues = CompactingCodec::decompact(dcComponentStorageCount, inBits, 4, 4);
    int16_t *dcComponents = dcComponentStorageValues;
    if (rle) {
        int tmp;
        RleCodec::rleDecode(dcComponents, dcRleCounts, dcComponentStorageValues, dcComponentStorageCount, tmp);
        delete[] dcRleCounts;
    }
    delete[] dcComponentStorageValues;
    //TODO: differential decode

    for (int i = 0; i < blockCount; i++) {
        int acComponentStorageCount = CompactingCodec::decompactSingleValue(inBits, 8);
        int16_t *acZeroCounts = nullptr;
        if (rle) {
            acZeroCounts = CompactingCodec::decompact(acComponentStorageCount, inBits, 4, 4);
        }
        int16_t *acComponentStorageValues = CompactingCodec::decompact(acComponentStorageCount, inBits, 4, 4);
        int16_t *acComponentsTmp = acComponentStorageValues;
        if (rle) {
            int tmp;
            RleCodec::rleDecodeZeros(acComponentsTmp, acZeroCounts, acComponentStorageValues, acComponentStorageCount,
                                     tmp);
            delete[] acZeroCounts;
        }
        memcpy(acComponents[i], acComponentsTmp, sizeof(int16_t) * 15);
        delete[] acComponentStorageValues;
        if(rle) {
            delete[] acComponentsTmp;
        }
    }

    for (int blockRow = 0; blockRow < blockRows; blockRow++) {
        for (int blockCol = 0; blockCol < blockCols; blockCol++) {
            int blockIndex = blockRow * blockCols + blockCol;
            ValueBlock4x4 &block = *pixelBlocks[blockIndex];
            block.getData()[0] = dcComponents[blockIndex];
            block.setFilled();
            block.deZigzag();
            block.deQuantize(quantMatrix);
            block.applyInverseDct();
        }
    }
    if (rle) {
        delete[] dcComponents;
    }
    return true;
}

bool IFrameStorageCodec::differentialEncode(int16_t *data, int size) {
    int prev = data[0];
    bool overflow = false;
    for (int i = 1; i < size; i++) {
        int cur = data[i];
        int tmpDiff = cur - prev;
        int16_t diff = static_cast<int16_t>(tmpDiff);
        if (diff != tmpDiff) overflow = true;
        data[i] = diff;
        prev = cur;
    }
    return !overflow;
}
