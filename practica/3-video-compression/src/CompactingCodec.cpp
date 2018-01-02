//
// Created by cwout on 1/01/18.
//

#include <iostream>
#include "CompactingCodec.h"


bool CompactingCodec::compactSingleValue(uint32_t value, util::BitStreamWriter &bits, int valueBitsizeBitcount) {
    uint8_t valueBitsize = CompactingCodec::getRequiredBitcount(value);
    uint8_t valueBitsizeBitcountRequired = CompactingCodec::getRequiredBitcount(static_cast<uint32_t>(valueBitsize - 1));
    if (valueBitsizeBitcountRequired > valueBitsizeBitcount) { return false; }
    bits.put(valueBitsizeBitcount, static_cast<uint32_t>(valueBitsize - 1));
    bits.put(valueBitsize, value);
    return true;
}

uint32_t CompactingCodec::decompactSingleValue(util::BitStreamReader &bits, int valueBitsizeBitcount) {
    uint32_t valueBitsize = bits.get(valueBitsizeBitcount) + 1;
    uint32_t value = bits.get(valueBitsize);
    return value;
}

bool CompactingCodec::compact(const int16_t *data, int inputSize, util::BitStreamWriter &bits, int offsetBitsizeBitcount, int valueBitsizeBitcount) {
    int min, max;
    if (!CompactingCodec::getMinMax(data, inputSize, min, max)) { return false; }
    int range = max - min;
    uint8_t offsetSign = static_cast<uint8_t>(min < 0 ? 1 : 0);
    uint16_t offsetVal = static_cast<uint16_t>(std::abs(min));

    uint8_t offsetBitsize = CompactingCodec::getRequiredBitcount(offsetVal);
    uint8_t offsetBitsizeBitcountRequired = CompactingCodec::getRequiredBitcount(static_cast<uint16_t>(offsetBitsize - 1));
    if (offsetBitsizeBitcountRequired > offsetBitsizeBitcount) {return false;}

    uint8_t valueBitsize = CompactingCodec::getRequiredBitcount(static_cast<uint16_t>(range));
    uint8_t valueBitsizeBitcountRequired = CompactingCodec::getRequiredBitcount(static_cast<uint16_t>(valueBitsize - 1));
    if (valueBitsizeBitcountRequired > valueBitsizeBitcount) { return false; }

    bits.put_bit(offsetSign);
    bits.put(offsetBitsizeBitcount, static_cast<uint32_t>(offsetBitsize - 1));
    bits.put(offsetBitsize, offsetVal);

    bits.put(valueBitsizeBitcount, static_cast<uint32_t>(valueBitsize - 1));
    for (int i = 0; i < inputSize; i++) {
        bits.put(valueBitsize, static_cast<uint32_t>(data[i] - min));
    }

    return true;
}

int16_t *CompactingCodec::decompact(int outputSize, util::BitStreamReader &bits, int offsetBitsizeBitcount, int valueBitsizeBitcount) {
    int16_t *data = new int16_t[outputSize];
    uint8_t offsetSign = bits.get_bit();
    uint8_t offsetBitsize = static_cast<uint8_t>(bits.get(offsetBitsizeBitcount) + 1);
    uint16_t offsetVal = static_cast<uint16_t>(bits.get(offsetBitsize));
    uint8_t valueBitsize = static_cast<uint8_t>(bits.get(valueBitsizeBitcount) + 1);
    int min = offsetVal;
    if (offsetSign == 1) { min = -min; }
    for (int i = 0; i < outputSize; i++) {
        data[i] = static_cast<int16_t>(min + bits.get(valueBitsize));
    }
    return data;
}

bool CompactingCodec::getMinMax(const int16_t *data, int inputSize, int &min, int &max) {
    if (data == nullptr || inputSize <= 0) {
        min = 0;
        max = 0;
        return false;
    }
    min = data[0];
    max = data[0];
    for (int i = 1; i < inputSize; i++) {
        int16_t tmp = data[i];
        if (tmp < min) {
            min = tmp;
        }
        else if (tmp > max) {
            max = tmp;
        }
    }
    return true;
}

uint8_t CompactingCodec::getRequiredBitcount(uint32_t value) {
    int bitcount = 1;
    int valcount = 2;
    while (valcount <= value) {
        bitcount++;
        valcount *= 2;
    }
    return static_cast<uint8_t>(bitcount);
}
