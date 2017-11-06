
#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>
#include <iterator>
#include "RawFileParser.h"


ValueBlock4x4 *RawFileParser::readRawImageFile(std::string filename, int width, int height) {
    if (width % 4 != 0) return nullptr;
    if (height % 4 != 0) return nullptr;
    ValueBlock4x4 *image = new ValueBlock4x4[(width / 4) * (height / 4)];
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return nullptr;
    for (int rowMajor = 0; rowMajor < height / 4; rowMajor++) {
        for (int rowMinor = 0; rowMinor < 4; rowMinor++) {
            for (int colMajor = 0; colMajor < width / 4; colMajor++) {
                unsigned char tmp[4];
                file.read(reinterpret_cast<char *>(tmp), 4);
                for (int colMinor = 0; colMinor < 4; colMinor++) {
                    image[rowMajor * width / 4 + colMajor].getData()[rowMinor * 4 +
                                                                     colMinor] = static_cast<int16_t>(tmp[colMinor]);
                }
            }
        }
        for (int colMajor = 0; colMajor < width / 4; colMajor++) {
            image[rowMajor * width / 4 + colMajor].setFilled();
        }
    }
    file.close();
    return image;
}

uint8_t *RawFileParser::readEncodedFile(std::string filename, int &size) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return nullptr;
    uint8_t sizeArray[4];
    file.read(reinterpret_cast<char *>(&sizeArray[0]), sizeof(uint8_t));
    file.read(reinterpret_cast<char *>(&sizeArray[1]), sizeof(uint8_t));
    file.read(reinterpret_cast<char *>(&sizeArray[2]), sizeof(uint8_t));
    file.read(reinterpret_cast<char *>(&sizeArray[3]), sizeof(uint8_t));
    size = static_cast<int>(sizeArray[0]) * 16777216 + static_cast<int>(sizeArray[1]) * 65536 +
           static_cast<int>(sizeArray[2]) * 256 + static_cast<int>(sizeArray[3]);
    uint8_t *dataArray = new uint8_t[size];
    file.read(reinterpret_cast<char *>(dataArray), size);
    file.close();
    return dataArray;
}

bool RawFileParser::writeEncodedFile(std::string filename, int size, uint8_t *data) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) return false;
    uint8_t sizeArray[4] = {static_cast<uint8_t>(size / 16777216), static_cast<uint8_t>((size % 16777216) / 65536),
                            static_cast<uint8_t>((size % 65536) / 256), static_cast<uint8_t>(size % 256)};
    file.write(reinterpret_cast<const char *>(&sizeArray[0]), 1);
    file.write(reinterpret_cast<const char *>(&sizeArray[1]), 1);
    file.write(reinterpret_cast<const char *>(&sizeArray[2]), 1);
    file.write(reinterpret_cast<const char *>(&sizeArray[3]), 1);
    file.write(reinterpret_cast<const char *>(data), size);
    file.close();
    return true;
}

bool RawFileParser::writeRawImageFile(std::string filename, int width, int height, ValueBlock4x4 *data) {
    std::ofstream file(filename, std::ofstream::binary);
    if (!file.is_open()) return false;
    for (int rowMajor = 0; rowMajor < height / 4; rowMajor++) {
        for (int rowMinor = 0; rowMinor < 4; rowMinor++) {
            for (int colMajor = 0; colMajor < width / 4; colMajor++) {
                uint8_t tmp[4];
                for (int colMinor = 0; colMinor < 4; colMinor++) {
                    int val = static_cast<int>(data[rowMajor * width / 4 + colMajor].getData()[rowMinor * 4 +
                                                                                               colMinor]);
                    if (val < 0) val = 0;
                    if (val > 255) val = 255;
                    tmp[colMinor] = static_cast<uint8_t>(val);
                }
                file.write(reinterpret_cast<const char *>(tmp), 4);
            }
        }
    }
    file.close();
    return true;
}
