//
// Created by cwout on 19/10/17.
//

#include <vector>
#include <cstring>
#include <iostream>
#include "RleCodec.h"

short *RleCodec::rleEncode(short *in, int size, int &outSize) {
    std::vector<short> tmpOut;
    int zeroCount = 0;
    for (int i = 0; i < size; i++) {
        if (in[i] == 0) {
            zeroCount++;
        }
        else {
            tmpOut.emplace_back(zeroCount);
            tmpOut.emplace_back(in[i]);
            zeroCount = 0;
        }
    }
    tmpOut.emplace_back(0);
    tmpOut.emplace_back(0);
    outSize = static_cast<int>(tmpOut.size());
    auto * out = new short[tmpOut.size()];
    memcpy(out, &tmpOut[0], sizeof(short)*outSize);
    return out;
}

short *RleCodec::rleDecode(short *in, int minOutSize, int maxOutSize, int &outSize, int &inSizeUsed) {
    outSize = 0;
    inSizeUsed = 0;
    while (!(in[inSizeUsed]==0&&in[inSizeUsed+1]==0)) {
        outSize += in[inSizeUsed] + 1;
        inSizeUsed += 2;
    }
    inSizeUsed += 2;
    if (outSize < minOutSize) {
        outSize = minOutSize;
    }
    if (outSize > maxOutSize) {
        outSize = maxOutSize;
    }
    short * out = new short[outSize];
    int iOut = 0;
    for (int iIn = 0; iIn < inSizeUsed-2; iIn+=2) {
        for (int i = 0; i < in[iIn]; i++) {
            if (iOut >= maxOutSize) {
                break;
            }
            out[iOut++] = 0;
        }
        if (iOut >= maxOutSize) {
            break;
        }
        out[iOut++] = in[iIn+1];
    }
    if (iOut < outSize) {
        for (;iOut < outSize; iOut++) {
            out[iOut] = 0;
        }
    }
    return out;
}
