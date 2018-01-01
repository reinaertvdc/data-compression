#include <vector>
#include <cstring>
#include "RleCodec.h"

int16_t *RleCodec::rleEncode(int16_t *in, int size, int &outSize) {
    int nonZeroSize = size;
    while (nonZeroSize > 0 && static_cast<int>(in[nonZeroSize - 1]) == 0) {
        nonZeroSize--;
    }
    std::vector<int16_t> tmpOut;
    tmpOut.emplace_back(static_cast<int16_t>(nonZeroSize));
    for (int i = 0; i < nonZeroSize; i++) {
        tmpOut.emplace_back(in[i]);
    }
    outSize = static_cast<int>(tmpOut.size());
    auto *out = new int16_t[outSize];
    memcpy(out, &tmpOut[0], sizeof(int16_t) * outSize);
    return out;
}

short *RleCodec::rleDecode(const int16_t *in, int outSize, int &inSizeUsed) {
    inSizeUsed = 0;
    int size = in[0];
    inSizeUsed += 1 + size;
    auto *out = new int16_t[outSize];
    for (int i = 0; i < size; i++) {
        out[i] = in[i + 1];
    }
    for (int i = size; i < outSize; i++) {
        out[i] = 0;
    }
    return out;
}

bool
RleCodec::rleEncode(const int16_t *data, int16_t *&rleCounts, int16_t *&rleValues, int inputSize, int &outputSize) {
    outputSize = 0;
    if (inputSize <= 0 || data == nullptr) {
        rleCounts = nullptr;
        rleValues = nullptr;
        return true;
    }
    int16_t tmpCounts[inputSize];
    int16_t tmpValues[inputSize];
    for (int i = 0; i < inputSize; outputSize++) {
        int16_t count = 1;
        int16_t value = data[i++];
        for (; i < inputSize && data[i] == value; count++, i++);
        tmpCounts[outputSize] = count;
        tmpValues[outputSize] = value;
    }
    rleCounts = new int16_t[outputSize];
    rleValues = new int16_t[outputSize];
    memcpy(rleCounts, tmpCounts, sizeof(int16_t) * outputSize);
    memcpy(rleValues, tmpValues, sizeof(int16_t) * outputSize);
    return true;
}

bool
RleCodec::rleDecode(int16_t *&data, const int16_t *rleCounts, const int16_t *rleValues, int rleSize, int &outputSize) {
    outputSize = 0;
    if (rleSize <= 0 || rleCounts == nullptr || rleValues == nullptr) {
        data = nullptr;
        return false;
    }
    for (int i = 0; i < rleSize; i++) {
        outputSize += rleCounts[i];
    }
    data = new int16_t[outputSize];
    int iData = 0;
    for (int i = 0; i < rleSize; i++) {
        for (int j = 0; j < rleCounts[i]; j++, iData++) {
            data[iData] = rleValues[i];
        }
    }
    return true;
}

bool
RleCodec::rleEncodeZeros(const int16_t *data, int16_t *&zeroCounts, int16_t *nonZeros, int inputSize, int &outputSize, int maxZeros) {
    outputSize = 0;
    if (inputSize <= 0 || data == nullptr) {
        zeroCounts = nullptr;
        nonZeros = nullptr;
        return false;
    }
    int16_t tmpCounts[inputSize];
    int16_t tmpValues[inputSize];
    for (int i = 0; i < inputSize; outputSize++) {
        int16_t count = 0;
        for (; i < inputSize && data[i] == 0 && count < maxZeros; count++, i++);
        if (i == inputSize) {
            count--;
            i--;
        }
        tmpCounts[outputSize] = count;
        tmpValues[outputSize] = data[i++];
    }
    zeroCounts = new int16_t[outputSize];
    nonZeros = new int16_t[outputSize];
    memcpy(zeroCounts, tmpCounts, sizeof(int16_t) * outputSize);
    memcpy(nonZeros, tmpValues, sizeof(int16_t) * outputSize);
    return true;
}

bool RleCodec::rleDecodeZeros(int16_t *&data, const int16_t *zeroCounts, const int16_t *nonZeros, int rleSize,
                              int &outputSize) {
    outputSize = 0;
    if (rleSize <= 0 || zeroCounts == nullptr || nonZeros == nullptr) {
        data = nullptr;
        return false;
    }
    for (int i = 0; i < rleSize; i++) {
        outputSize += zeroCounts[i];
        outputSize++;
    }
    data = new int16_t[outputSize];
    int iData = 0;
    for (int i = 0; i < rleSize; i++, iData++) {
        for (int j = 0; j < zeroCounts[i]; j++, iData++) {
            data[iData] = 0;
        }
        data[iData] = nonZeros[i];
    }
    return true;
}
