//
// Created by cwout on 23/10/17.
//

#include <iostream>
#include <cstring>
#include "StorageFormatCodec.h"
#include "BitStream.h"

uint8_t *StorageFormatCodec::toStorageFormat(int16_t *data, int size, int &outSize, int width, int height, bool rle, ValueBlock4x4 &quant) {

    util::BitStreamWriter outBits(size*2+64);

    // width and height

    uint16_t w = static_cast<uint16_t>(width);
    uint16_t h = static_cast<uint16_t>(height);
    util::BitStreamReader wBits((uint8_t*)(&w), 2);
    util::BitStreamReader hBits((uint8_t*)(&h), 2);
    outBits.put(8, wBits.get(8));
    outBits.put(8, wBits.get(8));
    outBits.put(8, hBits.get(8));
    outBits.put(8, hBits.get(8));

    // use of rle bit

    if (rle) outBits.put_bit(1);
    else outBits.put_bit(0);

    // quantization matrix

    uint8_t quantMatrixBits = 1;
    uint8_t quantMatrixMaxValue = 1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            while (quant.getValue(i, j) > quantMatrixMaxValue) {
                quantMatrixBits++;
                quantMatrixMaxValue = static_cast<uint8_t>((quantMatrixMaxValue + 1) * 2 - 1);
            }
        }
    }
    outBits.put(4, quantMatrixBits);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            uint16_t val = static_cast<uint16_t>(quant.getValue(i, j));
            outBits.put(quantMatrixBits, val);
        }
    }

//    for (int iBlock = 0; iBlock < width/4; iBlock++) {
//        for (int jBlock = 0; jBlock < width/4; jBlock++) {
//
//        }
//    }

    // copy bytes to memory and return pointer

    outSize = (outBits.get_position()/8)+(outBits.get_position()%8==0?0:1);
    uint8_t * out = new uint8_t[outSize];
    memcpy(out, outBits.get_buffer(), sizeof(uint8_t)*outSize);
    return out;

}

int16_t *StorageFormatCodec::fromStorageFormat(uint8_t *data, int size, int &outSize, int &wOut, int &hOut, bool &rleOut, ValueBlock4x4 &quant) {

    util::BitStreamReader inBits(data, size);

    // width and height

    uint16_t w;
    uint16_t h;
    util::BitStreamWriter wBits((uint8_t *)&w, 2);
    util::BitStreamWriter hBits((uint8_t *)&h, 2);
    wBits.put(8, inBits.get(8));
    wBits.put(8, inBits.get(8));
    hBits.put(8, inBits.get(8));
    hBits.put(8, inBits.get(8));
    wOut = static_cast<int>(w);
    hOut = static_cast<int>(h);

    // use of rle bit

    rleOut = inBits.get_bit() == 1;

    // quantization matrix

    uint8_t quantMatrixBits = (uint8_t )inBits.get(4);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            quant.getData()[i*4+j] = static_cast<short>((uint16_t)inBits.get(quantMatrixBits));
        }
    }

    return nullptr;
}
