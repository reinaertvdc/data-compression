#include <iostream>
#include <cstring>
#include <vector>
#include "IFrameStorageCodec.h"
#include "BitStream.h"
#include "RleCodec.h"
#include "CompactingCodec.h"

uint8_t *IFrameStorageCodec::toStorageFormat(int16_t *data, int size, int &outSize, int width, int height, bool rle) {
    util::BitStreamWriter outBits(size * 2 * 18 / 16 + 256);
    int iData = 0;
    for (int iBlock = 0; iBlock < height / 4; iBlock++) {
        for (int jBlock = 0; jBlock < width / 4; jBlock++) {
            if (rle) {
                int valCount = static_cast<int>(data[iData++]);
                if (valCount == 0) {
                    uint8_t tmp = 0x00;
                    outBits.put(8, tmp);
                    continue;
                }
                int dcVal = static_cast<int>(data[iData++]);
                int dcValAbs = dcVal;
                if (dcValAbs < 0) {
                    dcValAbs = -dcVal;
                }
                int dcValBitSize = 1;
                int dcValSize = 2;
                while (dcValAbs >= dcValSize) {
                    dcValBitSize++;
                    dcValSize *= 2;
                }
                outBits.put(4, static_cast<uint8_t>(dcValBitSize));
                if (dcVal != dcValAbs) {
                    outBits.put_bit(static_cast<uint8_t>(1));
                } else {
                    outBits.put_bit(static_cast<uint8_t>(0));
                }
                outBits.put(dcValBitSize, static_cast<uint32_t>(dcValAbs));
                int acValCount = valCount - 1;
                outBits.put(4, static_cast<uint8_t>(acValCount));
                if (acValCount == 0) {
                    continue;
                }
                int minAcVal = static_cast<int>(data[iData]);
                int maxAcVal = static_cast<int>(data[iData]);
                for (int i = 0; i < acValCount; i++) {
                    if (static_cast<int>(data[iData + i]) < minAcVal) minAcVal = static_cast<int>(data[iData + i]);
                    if (static_cast<int>(data[iData + i]) > maxAcVal) maxAcVal = static_cast<int>(data[iData + i]);
                }
                int offset = minAcVal;
                int offsetAbs = offset;
                if (offsetAbs < 0) {
                    offsetAbs = -offsetAbs;
                }
                if (offset == 0) {
                    outBits.put(4, static_cast<uint8_t>(0));
                } else {
                    int offsetBitSize = 1;
                    int offsetSize = 2;
                    while (offsetAbs >= offsetSize) {
                        offsetBitSize++;
                        offsetSize *= 2;
                    }
                    outBits.put(4, static_cast<uint8_t>(offsetBitSize));
                    if (offset != offsetAbs) {
                        outBits.put_bit(static_cast<uint8_t>(1));
                    } else {
                        outBits.put_bit(static_cast<uint8_t>(0));
                    }
                    outBits.put(offsetBitSize, static_cast<uint32_t>(offsetAbs));
                }
                int acRange = maxAcVal - minAcVal;
                int acValBitSize = 1;
                int acValSize = 2;
                while (acRange >= acValSize) {
                    acValBitSize++;
                    acValSize *= 2;
                }
                outBits.put(4, static_cast<uint8_t>(acValBitSize - 1));
                for (int i = 0; i < acValCount; i++) {
                    outBits.put(acValBitSize, static_cast<uint32_t>(static_cast<int>(data[iData++]) - offset));
                }
            } else {
                int dcVal = static_cast<int>(data[iData++]);
                if (dcVal == 0) {
                    outBits.put(4, static_cast<uint8_t>(0));
                } else {
                    int dcValAbs = dcVal;
                    if (dcValAbs < 0) {
                        dcValAbs = -dcVal;
                    }
                    int dcValBitSize = 1;
                    int dcValSize = 2;
                    while (dcValAbs >= dcValSize) {
                        dcValBitSize++;
                        dcValSize *= 2;
                    }
                    outBits.put(4, static_cast<uint8_t>(dcValBitSize));
                    if (dcVal != dcValAbs) {
                        outBits.put_bit(static_cast<uint8_t>(1));
                    } else {
                        outBits.put_bit(static_cast<uint8_t>(0));
                    }
                    outBits.put(dcValBitSize, static_cast<uint32_t>(dcValAbs));
                }
                int minAcVal = static_cast<int>(data[iData]);
                int maxAcVal = static_cast<int>(data[iData]);
                for (int i = 0; i < 15; i++) {
                    if (static_cast<int>(data[iData + i]) < minAcVal) minAcVal = static_cast<int>(data[iData + i]);
                    if (static_cast<int>(data[iData + i]) > maxAcVal) maxAcVal = static_cast<int>(data[iData + i]);
                }
                int offset = minAcVal;
                int offsetAbs = offset;
                if (offsetAbs < 0) {
                    offsetAbs = -offsetAbs;
                }
                if (offset == 0) {
                    outBits.put(4, static_cast<uint8_t>(0));
                } else {
                    int offsetBitSize = 1;
                    int offsetSize = 2;
                    while (offsetAbs >= offsetSize) {
                        offsetBitSize++;
                        offsetSize *= 2;
                    }
                    outBits.put(4, static_cast<uint8_t>(offsetBitSize));
                    if (offset != offsetAbs) {
                        outBits.put_bit(static_cast<uint8_t>(1));
                    } else {
                        outBits.put_bit(static_cast<uint8_t>(0));
                    }
                    outBits.put(offsetBitSize, static_cast<uint32_t>(offsetAbs));
                }
                int acRange = maxAcVal - minAcVal;
                int acValBitSize = 1;
                int acValSize = 2;
                while (acRange >= acValSize) {
                    acValBitSize++;
                    acValSize *= 2;
                }
                outBits.put(4, static_cast<uint8_t>(acValBitSize - 1));
                for (int i = 0; i < 15; i++) {
                    outBits.put(acValBitSize, static_cast<uint32_t>(static_cast<int>(data[iData++]) - offset));
                }
            }
        }
    }

    // copy bytes to memory and return pointer

    outSize = (outBits.get_position() / 8) + (outBits.get_position() % 8 == 0 ? 0 : 1);
    uint8_t *out = new uint8_t[outSize];
    memcpy(out, outBits.get_buffer(), sizeof(uint8_t) * outSize);
    return out;

}

uint8_t *IFrameStorageCodec::toStorageFormat(ValueBlock4x4 *pixelBlocks, const ValueBlock4x4 &quantMatrix, int &storageSize, int width, int height, bool rle, bool differentialEncodeDc) {
    if (width % 4 != 0 || height % 4 != 0) {
        return nullptr;
    }
    util::BitStreamWriter outBits(new uint8_t[width * height * 2], width * height * 2);
    int blockRows = height / 4;
    int blockCols = width / 4;
    int blockCount = blockRows * blockCols;
    int16_t dcComponents[blockCount];
    int16_t *acComponents[blockCount];
    for (int blockRow = 0; blockRow < blockRows; blockRow++) {
        for (int blockCol = 0; blockCol < blockCols; blockCol++) {
            int blockIndex = blockRow * blockCols + blockCol;
            ValueBlock4x4 &block = pixelBlocks[blockIndex];
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
        CompactingCodec::compactSingleValue(static_cast<uint32_t>(dcComponentStorageCount), outBits, 5);
        CompactingCodec::compact(dcRleCounts, dcComponentStorageCount, outBits, 4, 4);
        delete[] dcRleCounts;
    }
    else {
        CompactingCodec::compactSingleValue(static_cast<uint16_t>(dcComponentStorageCount), outBits, 5);
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
            RleCodec::rleEncodeZeros(acComponents[i], acZeroCounts, acComponentStorageValues, 15, acComponentStorageCount, 15);
            CompactingCodec::compactSingleValue(static_cast<uint16_t>(acComponentStorageCount), outBits, 3);
            CompactingCodec::compact(acZeroCounts, acComponentStorageCount, outBits, 4, 4);
            delete[] acZeroCounts;
        }
        else {
            CompactingCodec::compactSingleValue(static_cast<uint16_t>(acComponentStorageCount), outBits, 3);
        }
        CompactingCodec::compact(acComponentStorageValues, acComponentStorageCount, outBits, 4, 4);
        if (rle) {
            delete[] acComponentStorageValues;
        }
    }
    return outBits.get_buffer();
}

uint8_t *
IFrameStorageCodec::toStorageFormat(ValueBlock4x4 **pixelBlocks, const ValueBlock4x4 &quantMatrix, int &storageSize,
                                    int width, int height, bool rle, bool differentialEncodeDc) {
    if (width % 4 != 0 || height % 4 != 0) {
        return nullptr;
    }
    util::BitStreamWriter outBits(new uint8_t[width * height * 2], width * height * 2);
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
        CompactingCodec::compactSingleValue(static_cast<uint32_t>(dcComponentStorageCount), outBits, 5);
        CompactingCodec::compact(dcRleCounts, dcComponentStorageCount, outBits, 4, 4);
        delete[] dcRleCounts;
    }
    else {
        CompactingCodec::compactSingleValue(static_cast<uint16_t>(dcComponentStorageCount), outBits, 5);
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
            RleCodec::rleEncodeZeros(acComponents[i], acZeroCounts, acComponentStorageValues, 15, acComponentStorageCount, 15);
            CompactingCodec::compactSingleValue(static_cast<uint16_t>(acComponentStorageCount), outBits, 3);
            CompactingCodec::compact(acZeroCounts, acComponentStorageCount, outBits, 4, 4);
            delete[] acZeroCounts;
        }
        else {
            CompactingCodec::compactSingleValue(static_cast<uint16_t>(acComponentStorageCount), outBits, 3);
        }
        CompactingCodec::compact(acComponentStorageValues, acComponentStorageCount, outBits, 4, 4);
        if (rle) {
            delete[] acComponentStorageValues;
        }
    }
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
            acComponents[blockIndex] = &(block.getData()[0]);
            acComponents[blockIndex] = &(block.getData()[1]);
        }
    }

    int dcComponentStorageCount = CompactingCodec::decompactSingleValue(inBits, 5);
    int16_t *dcRleCounts = nullptr;
    if (rle) {
        dcRleCounts = CompactingCodec::decompact(dcComponentStorageCount, inBits, 4, 4);
    }
    int16_t *dcComponentStorageValues = CompactingCodec::decompact(blockCount, inBits, 4, 4);
    int16_t *dcComponents = dcComponentStorageValues;
    if (rle) {
        int tmp;
        RleCodec::rleDecode(dcComponents, dcRleCounts, dcComponentStorageValues, dcComponentStorageCount, tmp);
    }

    for (int i = 0; i < blockCount; i++) {
        int acComponentStorageCount = CompactingCodec::decompactSingleValue(inBits, 3);
        int16_t *acZeroCounts = nullptr;
        if (rle) {
            acZeroCounts = CompactingCodec::decompact(acComponentStorageCount, inBits, 4, 4);
        }
        int16_t *acComponentStorageValues = CompactingCodec::decompact(acComponentStorageCount, inBits, 4, 4);
        int16_t *acComponentsTmp = acComponentStorageValues;
        if (rle) {
            int tmp;
            RleCodec::rleDecodeZeros(acComponentsTmp, acZeroCounts, acComponentStorageValues, acComponentStorageCount, tmp);
        }
        memcpy(acComponents[1], acComponentsTmp, sizeof(int16_t) * 15);
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
    return true;
}

bool IFrameStorageCodec::fromStorageFormat(uint8_t *data, int inSize, const ValueBlock4x4 &quantMatrix, int &outSize, int width, int height, bool rle, bool differentialEncodeAc, ValueBlock4x4 * pixelBlocks) {
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
            ValueBlock4x4 &block = pixelBlocks[blockIndex];
            acComponents[blockIndex] = &(block.getData()[0]);
            acComponents[blockIndex] = &(block.getData()[1]);
        }
    }

    int dcComponentStorageCount = CompactingCodec::decompactSingleValue(inBits, 5);
    int16_t *dcRleCounts = nullptr;
    if (rle) {
        dcRleCounts = CompactingCodec::decompact(dcComponentStorageCount, inBits, 4, 4);
    }
    int16_t *dcComponentStorageValues = CompactingCodec::decompact(blockCount, inBits, 4, 4);
    int16_t *dcComponents = dcComponentStorageValues;
    if (rle) {
        int tmp;
        RleCodec::rleDecode(dcComponents, dcRleCounts, dcComponentStorageValues, dcComponentStorageCount, tmp);
    }

    for (int i = 0; i < blockCount; i++) {
        int acComponentStorageCount = CompactingCodec::decompactSingleValue(inBits, 3);
        int16_t *acZeroCounts = nullptr;
        if (rle) {
            acZeroCounts = CompactingCodec::decompact(acComponentStorageCount, inBits, 4, 4);
        }
        int16_t *acComponentStorageValues = CompactingCodec::decompact(acComponentStorageCount, inBits, 4, 4);
        int16_t *acComponentsTmp = acComponentStorageValues;
        if (rle) {
            int tmp;
            RleCodec::rleDecodeZeros(acComponentsTmp, acZeroCounts, acComponentStorageValues, acComponentStorageCount, tmp);
        }
        memcpy(acComponents[1], acComponentsTmp, sizeof(int16_t) * 15);
    }

    for (int blockRow = 0; blockRow < blockRows; blockRow++) {
        for (int blockCol = 0; blockCol < blockCols; blockCol++) {
            int blockIndex = blockRow * blockCols + blockCol;
            ValueBlock4x4 &block = pixelBlocks[blockIndex];
            block.getData()[0] = dcComponents[blockIndex];
            block.setFilled();
            block.deZigzag();
            block.deQuantize(quantMatrix);
            block.applyInverseDct();
        }
    }
    return true;
}

int16_t *
IFrameStorageCodec::fromStorageFormat(uint8_t *data, int size, int &outSize, int width, int height, bool rle) {
    util::BitStreamReader inBits(data, size);

    std::vector<int16_t> tmpData;
    for (int iBlock = 0; iBlock < height / 4; iBlock++) {
        for (int jBlock = 0; jBlock < width / 4; jBlock++) {
            if (rle) {
                int dcValBitSize = static_cast<int>(inBits.get(4));
                int dcVal = 0;
                if (dcValBitSize > 0) {
                    bool dcValNeg = static_cast<int>(inBits.get_bit()) == 1;
                    int dcValAbs = static_cast<int>(inBits.get(dcValBitSize));
                    if (dcValNeg) dcVal = -dcValAbs;
                    else dcVal = dcValAbs;
                }
                int acValCount = static_cast<int>(inBits.get(4));
                int valCount = acValCount + 1;
                if (dcValBitSize == 0 && acValCount == 0) {
                    tmpData.emplace_back(static_cast<int16_t>(0));
                    continue;
                }
                tmpData.emplace_back(valCount);
                tmpData.emplace_back(dcVal);
                if (acValCount > 0) {
                    int offsetBitSize = static_cast<int>(inBits.get(4));
                    int offset = 0;
                    if (offsetBitSize > 0) {
                        bool offsetNeg = static_cast<int>(inBits.get_bit()) == 1;
                        int offsetAbs = static_cast<int>(inBits.get(offsetBitSize));
                        if (offsetNeg) offset = -offsetAbs;
                        else offset = offsetAbs;
                    }
                    int acValBitSize = static_cast<int>(inBits.get(4)) + 1;
                    for (int i = 0; i < acValCount; i++) {
                        tmpData.emplace_back(static_cast<int16_t>(static_cast<int>(inBits.get(acValBitSize)) + offset));
                    }
                }
            } else {
                int dcValBitSize = static_cast<int>(inBits.get(4));
                int dcVal = 0;
                if (dcValBitSize > 0) {
                    bool dcValNeg = static_cast<int>(inBits.get_bit()) == 1;
                    int dcValAbs = static_cast<int>(inBits.get(dcValBitSize));
                    if (dcValNeg) dcVal = -dcValAbs;
                    else dcVal = dcValAbs;
                }
                tmpData.emplace_back(dcVal);
                int offsetBitSize = static_cast<int>(inBits.get(4));
                int offset = 0;
                if (offsetBitSize > 0) {
                    bool offsetNeg = static_cast<int>(inBits.get_bit()) == 1;
                    int offsetAbs = static_cast<int>(inBits.get(offsetBitSize));
                    if (offsetNeg) offset = -offsetAbs;
                    else offset = offsetAbs;
                }
                int acValBitSize = static_cast<int>(inBits.get(4)) + 1;
                for (int i = 0; i < 15; i++) {
                    tmpData.emplace_back(static_cast<int16_t>(static_cast<int>(inBits.get(acValBitSize)) + offset));
                }
            }
        }
    }

    outSize = static_cast<int>(tmpData.size());
    int16_t *out = new int16_t[outSize];
    memcpy(out, &tmpData[0], sizeof(int16_t) * outSize);
    return out;
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
