//
// Created by cwout on 23/10/17.
//

#include <iostream>
#include <cstring>
#include "StorageFormatCodec.h"
#include "BitStream.h"

uint8_t *StorageFormatCodec::toStorageFormat(int16_t *data, int size, int &outSize, int width, int height, bool rle, const ValueBlock4x4 &quant) {

    util::BitStreamWriter outBits(size*2*18/16+256);

    // width and height

    uint8_t wh[4] = {static_cast<uint8_t>(width/256), static_cast<uint8_t>(width), static_cast<uint8_t>(height/256), static_cast<uint8_t>(height)};
    util::BitStreamReader whBits(wh, 4);
    outBits.put(32, whBits.get(32));

    // use of rle bit

    if (rle) outBits.put_bit(1);
    else outBits.put_bit(0);

    // quantization matrix

    int quantMatrixBits = 1;
    int quantMatrixMaxValue = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            while (quant.getValue(i, j) > quantMatrixMaxValue) {
                quantMatrixBits++;
                quantMatrixMaxValue = (quantMatrixMaxValue + 1) * 2 - 1;
            }
        }
    }
    outBits.put(4, static_cast<uint8_t>(quantMatrixBits - 1));
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            uint16_t val = static_cast<uint16_t>(quant.getValue(i, j));
            outBits.put(quantMatrixBits, val);
        }
    }

    // data

    int iData = 0;
    if (rle) {
        for (int iBlock = 0; iBlock < height / 4; iBlock++) {
            for (int jBlock = 0; jBlock < width / 4; jBlock++) {
                int valCount = static_cast<int>(data[iData++]);
                if (valCount == 0) {
                    uint8_t tmp = 0x00;
                    outBits.put(8, tmp);
                    continue;
                }
                uint8_t tmp = static_cast<uint8_t>(valCount - 1);
                outBits.put(4, tmp);
                int minVal = static_cast<int>(data[iData]);
                int maxVal = static_cast<int>(data[iData]);
                for (int i = 0; i < valCount; i++) {
                    if (static_cast<int>(data[iData+i]) < minVal) minVal = static_cast<int>(data[iData+i]);
                    if (static_cast<int>(data[iData+i]) > maxVal) maxVal = static_cast<int>(data[iData+i]);
                }
                int range = maxVal - minVal;
                int valBitSize = 1;
                int valSize = 2;
                while (range <= valSize) {
                    valBitSize++;
                    valSize *= 2;
                }
                outBits.put(4, static_cast<uint8_t>(valBitSize));
                int offset = minVal;
                if (offset < 0) {
                    outBits.put_bit(static_cast<uint8_t>(1));
                    offset = -offset;
                }
                else {
                    outBits.put_bit(static_cast<uint8_t>(0));
                }
                int offsetBitSize = 1;
                int offsetSize = 2;
                while (offset <= offsetSize) {
                    offsetBitSize++;
                    offsetSize *= 2;
                }
                outBits.put(4, static_cast<uint8_t>(offsetBitSize-1));
                for (int i = 1; i < valCount; i++) {
                    outBits.put(valBitSize, static_cast<uint16_t>(static_cast<int>(data[iData + i])-minVal));
                }
                iData += valCount;
            }
        }
    }
    else {
        //TODO
    }

    // copy bytes to memory and return pointer

    outSize = (outBits.get_position()/8)+(outBits.get_position()%8==0?0:1);
    std::cout << outSize << std::endl;
    uint8_t * out = new uint8_t[outSize];
    memcpy(out, outBits.get_buffer(), sizeof(uint8_t)*outSize);
    return out;

}

int16_t *StorageFormatCodec::fromStorageFormat(uint8_t *data, int size, int &outSize, int &wOut, int &hOut, bool &rleOut, ValueBlock4x4 &quant) {

    util::BitStreamReader inBits(data, size);

    // width and height

    uint8_t wh[4];
    util::BitStreamWriter whBits(wh, 4);
    whBits.put(32, inBits.get(32));
    wOut = static_cast<int>(wh[0])*256+ static_cast<int>(wh[1]);
    hOut = static_cast<int>(wh[2])*256+ static_cast<int>(wh[3]);

    std::cout << "w: " << wOut << std::endl;
    std::cout << "h: " << hOut << std::endl;

    // use of rle bit

    rleOut = inBits.get_bit() == 1;

    std::cout << "rle: " << rleOut << std::endl;

    // quantization matrix

    int quantMatrixAmountBits = static_cast<int>((uint8_t)inBits.get(4)) + 1;
    int quantMatrixAmountBytes = quantMatrixAmountBits / 8 + (quantMatrixAmountBits % 8 == 0 ? 0 : 1);
    uint8_t quantMatrixData[quantMatrixAmountBytes];
    util::BitStreamWriter quantMatrixDataBits(quantMatrixData, quantMatrixAmountBytes);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            quant.getData()[i*4+j] = static_cast<int16_t >((uint16_t)inBits.get(quantMatrixAmountBits));
        }
    }
    quant.setFilled();
    std::cout << "quantization matrix:" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << quant.getValue(i, j) << "\t";
        }
        std::cout << std::endl;
    }

    // data

    int16_t tmpData[hOut*wOut*18/16];
    int iTmpData = 0;
    if (rleOut) {
        for (int iBlock = 0; iBlock < hOut / 4; iBlock++) {
            for (int jBlock = 0; jBlock < wOut / 4; jBlock++) {
                int valCount = static_cast<int>(inBits.get(4)) + 1;
                int valBitSize = static_cast<int>(inBits.get(4));
                if (valCount == 0 && valBitSize == 0) {
                    tmpData[iTmpData++] = static_cast<int16_t>(0);
                    continue;
                }

            }
        }
    }
    else {
        //TODO
    }

    return nullptr;
}
