
#include <fstream>
#include <cstring>
#include <iostream>
#include "RawFileParser.h"

ValueBlock4x4 *RawFileParser::parseFile8bit(std::string filename, int width, int height) {
    auto *image = new ValueBlock4x4[(width/4)*(height/4)];
    std::ifstream file(filename, std::ios::in|std::ios::binary);
    if (!file.is_open()) return nullptr;
    for (int rowMajor = 0; rowMajor < height/4; rowMajor++) {
        for (int rowMinor = 0; rowMinor < 4; rowMinor++) {
            for (int colMajor = 0; colMajor < width/4; colMajor++) {
                unsigned char tmp[4];
                file.read((char *)tmp, 4*sizeof(unsigned char));
                for (int colMinor = 0; colMinor < 4; colMinor++) {
                    image[rowMajor * width / 4 + colMajor].getData()[rowMinor*4+colMinor] = tmp[colMinor];
                }
            }
        }
        for (int colMajor = 0; colMajor < width/4; colMajor++) {
            image[rowMajor*width/4+colMajor].setFilled();
        }
    }
    file.close();
    return image;
}

bool RawFileParser::writeFile8bit(std::string filename, ValueBlock4x4 *matrix, int width, int height) {
    std::ofstream file(filename, std::ofstream::out|std::ofstream::binary);
    if (!file.is_open()) return false;
    for (int rowMajor = 0; rowMajor < height/4; rowMajor++) {
        for (int rowMinor = 0; rowMinor < 4; rowMinor++) {
            for (int colMajor = 0; colMajor < width/4; colMajor++) {
                char tmp[4];
                for (int colMinor = 0; colMinor < 4; colMinor++) {
                    tmp[colMinor] = (char)(static_cast<unsigned char>(matrix[rowMajor * width / 4 + colMajor].getData()[rowMinor*4+colMinor]));
                }
                file.write(tmp, 4*sizeof(char));
            }
        }
    }
    file.close();
    return true;
}

ValueBlock4x4 *RawFileParser::parseFile16bit(std::string filename, int width, int height) {
    auto *image = new ValueBlock4x4[(width/4)*(height/4)];
    std::ifstream file(filename, std::ios::in|std::ios::binary);
    if (!file.is_open()) return nullptr;
    for (int rowMajor = 0; rowMajor < height/4; rowMajor++) {
        for (int rowMinor = 0; rowMinor < 4; rowMinor++) {
            for (int colMajor = 0; colMajor < width/4; colMajor++) {
                unsigned char tmp[8];
                file.read((char *)tmp, 8*sizeof(unsigned char));
                for (int colMinor = 0; colMinor < 4; colMinor++) {
                    short value = (((short)tmp[colMinor*2])&0x00ff)|((((short)tmp[colMinor*2+1])&0x00ff)<<8);
                    image[rowMajor * width / 4 + colMajor].getData()[rowMinor*4+colMinor] = value;
                }
            }
        }
        for (int colMajor = 0; colMajor < width/4; colMajor++) {
            image[rowMajor*width/4+colMajor].setFilled();
        }
    }
    file.close();
    return image;
}

bool RawFileParser::writeFile16bit(std::string filename, ValueBlock4x4 *matrix, int width, int height) {
    std::ofstream file(filename, std::ofstream::out|std::ofstream::binary);
    if (!file.is_open()) return false;
    for (int rowMajor = 0; rowMajor < height/4; rowMajor++) {
        for (int rowMinor = 0; rowMinor < 4; rowMinor++) {
            for (int colMajor = 0; colMajor < width/4; colMajor++) {
                char tmp[8];
                for (int colMinor = 0; colMinor < 4; colMinor++) {
                    tmp[colMinor*2+1] = (char)((matrix[rowMajor*width/4+colMajor].getData()[rowMinor*4+colMinor] & 0xff00) >> 8);
                    tmp[colMinor*2] = (char)(matrix[rowMajor*width/4+colMajor].getData()[rowMinor*4+colMinor] & 0x00ff);
                }
                file.write(tmp, 8*sizeof(char));
            }
        }
    }
    file.close();
    return true;
}

bool RawFileParser::WriteFile16bit(std::string filename, short *array, int size) {
    std::ofstream file(filename, std::ofstream::out|std::ofstream::binary);
    if (!file.is_open()) return false;
    char tmp[size*2];
    for (int i = 0; i < size; i++) {
        tmp[2*i+1] = (char)((array[i] & 0xff00) >> 8);
        tmp[2*i] = (char)(array[i] & 0x00ff);
    }
    file.write(tmp, size*2*sizeof(char));
    file.close();
    return true;
}

short *RawFileParser::parseFile16bit(std::string filename, int size) {
    short *data = new short[size];
    std::ifstream file(filename, std::ios::in|std::ios::binary);
    if (!file.is_open()) return nullptr;
    char tmp[size*2];
    file.read(tmp, size*2*sizeof(char));
    file.close();
    for (int i = 0; i < size; i++) {
        data[i] = ((short)tmp[i*2]&0x00ff)|((((short)tmp[i*2+1])&0x00ff)<<8);
    }
    return data;
}
