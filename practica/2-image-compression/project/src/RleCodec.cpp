//
// Created by cwout on 19/10/17.
//

#include <vector>
#include <cstring>
#include <iostream>
#include "RleCodec.h"

short *RleCodec::rleEncode(short *in, int size, int &outSize) {
    int nonZeroSize = size;
    while (nonZeroSize > 0 && in[nonZeroSize] != 0) {
        nonZeroSize--;
    }
    std::vector<short> tmpOut;
    tmpOut.emplace_back(nonZeroSize);
    for (int i = 0; i < nonZeroSize; i++) {
        tmpOut.emplace_back(in[i]);
    }
    outSize = static_cast<int>(tmpOut.size());
    auto * out = new short[outSize];
    memcpy(out, &tmpOut[0], sizeof(short)*outSize);
    return out;
}

short *RleCodec::rleDecode(short *in, int outSize, int &inSizeUsed) {
    inSizeUsed = 0;
    int size = in[0];
    inSizeUsed += 1 + size;
    short* out = new short[outSize];
    for (int i = 0; i < size; i++) {
        out[i] = in[i+1];
    }
    for (int i = size; i < outSize; i++) {
        out[i] = in[i];
    }
    return out;
}
