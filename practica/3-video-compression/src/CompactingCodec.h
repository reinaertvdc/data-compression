//
// Created by cwout on 1/01/18.
//

#ifndef INC_3_VIDEO_COMPRESSION_COMPACTINGCODEC_H
#define INC_3_VIDEO_COMPRESSION_COMPACTINGCODEC_H


#include <cstdint>

#include "BitStream.h"

class CompactingCodec {
public:
    static bool compactSingleValue(uint32_t value, util::BitStreamWriter &bits, int valueBitsizeBitcount);
    static uint32_t decompactSingleValue(util::BitStreamReader &bits, int valueBitsizeBitcount);
    static bool compact(const int16_t *data, int inputSize, util::BitStreamWriter &bits, int offsetBitsizeBitcount, int valueBitsizeBitcount);
    static int16_t *decompact(int outputSize, util::BitStreamReader &bits, int offsetBitsizeBitcount, int valueBitsizeBitcount);
    static bool getMinMax(const int16_t *data, int inputSize, int &min, int &max);
    static uint8_t getRequiredBitcount(uint32_t value);
};


#endif //INC_3_VIDEO_COMPRESSION_COMPACTINGCODEC_H
