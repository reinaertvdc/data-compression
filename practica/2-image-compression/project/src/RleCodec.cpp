
#include <vector>
#include <cstring>
#include <iostream>
#include "RleCodec.h"

int16_t *RleCodec::rleEncode(int16_t *in, int size, int &outSize, bool debug) {
    int nonZeroSize = size;
    while (nonZeroSize > 0 && static_cast<int>(in[nonZeroSize-1]) == 0) {
        nonZeroSize--;
    }
    std::vector<int16_t> tmpOut;
    tmpOut.emplace_back(static_cast<int16_t>(nonZeroSize));
    for (int i = 0; i < nonZeroSize; i++) {
        tmpOut.emplace_back(in[i]);
    }
    outSize = static_cast<int>(tmpOut.size());
    int16_t *out = new int16_t[outSize];
    memcpy(out, &tmpOut[0], sizeof(int16_t) * outSize);
    return out;
}

short *RleCodec::rleDecode(int16_t *in, int outSize, int &inSizeUsed) {
    inSizeUsed = 0;
    int size = in[0];
    inSizeUsed += 1 + size;
    int16_t *out = new int16_t[outSize];
    for (int i = 0; i < size; i++) {
        out[i] = in[i + 1];
    }
    for (int i = size; i < outSize; i++) {
        out[i] = 0;
    }
    return out;
}
