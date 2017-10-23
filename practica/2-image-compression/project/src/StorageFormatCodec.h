//
// Created by cwout on 23/10/17.
//

#ifndef PROJECT_STORAGEFORMATCODEC_H
#define PROJECT_STORAGEFORMATCODEC_H


#include <cstdint>
#include "ValueBlock4x4.h"

class StorageFormatCodec {
public:
    static uint8_t * toStorageFormat(int16_t * data, int size, int &outSize, int width, int height, bool rle, ValueBlock4x4 &quant);
    static int16_t * fromStorageFormat(uint8_t * data, int size, int &outSize, int &wOut, int &hOut, bool &rleOut, ValueBlock4x4 &quant);
};


#endif //PROJECT_STORAGEFORMATCODEC_H
