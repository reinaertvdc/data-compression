
#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>
#include "RawFileParser.h"

//ValueBlock4x4 *RawFileParser::parseFile8bit(std::string filename, int width, int height) {
//    auto *image = new ValueBlock4x4[(width/4)*(height/4)];
//    std::ifstream file(filename, std::ios::in|std::ios::binary);
//    if (!file.is_open()) return nullptr;
//    for (int rowMajor = 0; rowMajor < height/4; rowMajor++) {
//        for (int rowMinor = 0; rowMinor < 4; rowMinor++) {
//            for (int colMajor = 0; colMajor < width/4; colMajor++) {
//                unsigned char tmp[4];
//                file.read((char *)tmp, 4*sizeof(unsigned char));
//                for (int colMinor = 0; colMinor < 4; colMinor++) {
//                    image[rowMajor * width / 4 + colMajor].getData()[rowMinor*4+colMinor] = tmp[colMinor];
//                }
//            }
//        }
//        for (int colMajor = 0; colMajor < width/4; colMajor++) {
//            image[rowMajor*width/4+colMajor].setFilled();
//        }
//    }
//    file.close();
//    return image;
//}
//
//bool RawFileParser::writeFile8bit(std::string filename, ValueBlock4x4 *matrix, int width, int height) {
//    std::ofstream file(filename, std::ofstream::out|std::ofstream::binary);
//    if (!file.is_open()) return false;
//    for (int rowMajor = 0; rowMajor < height/4; rowMajor++) {
//        for (int rowMinor = 0; rowMinor < 4; rowMinor++) {
//            for (int colMajor = 0; colMajor < width/4; colMajor++) {
//                char tmp[4];
//                for (int colMinor = 0; colMinor < 4; colMinor++) {
//                    tmp[colMinor] = (char)(static_cast<unsigned char>(matrix[rowMajor * width / 4 + colMajor].getData()[rowMinor*4+colMinor]));
//                }
//                file.write(tmp, 4*sizeof(char));
//            }
//        }
//    }
//    file.close();
//    return true;
//}
//
//ValueBlock4x4 *RawFileParser::parseFile16bit(std::string filename, int width, int height) {
//    auto *image = new ValueBlock4x4[(width/4)*(height/4)];
//    std::ifstream file(filename, std::ios::in|std::ios::binary);
//    if (!file.is_open()) return nullptr;
//    for (int rowMajor = 0; rowMajor < height/4; rowMajor++) {
//        for (int rowMinor = 0; rowMinor < 4; rowMinor++) {
//            for (int colMajor = 0; colMajor < width/4; colMajor++) {
//                unsigned char tmp[8];
//                file.read((char *)tmp, 8*sizeof(unsigned char));
//                for (int colMinor = 0; colMinor < 4; colMinor++) {
//                    short value = (((short)tmp[colMinor*2])&0x00ff)|((((short)tmp[colMinor*2+1])&0x00ff)<<8);
//                    image[rowMajor * width / 4 + colMajor].getData()[rowMinor*4+colMinor] = value;
//                }
//            }
//        }
//        for (int colMajor = 0; colMajor < width/4; colMajor++) {
//            image[rowMajor*width/4+colMajor].setFilled();
//        }
//    }
//    file.close();
//    return image;
//}
//
//bool RawFileParser::writeFile16bit(std::string filename, ValueBlock4x4 *matrix, int width, int height) {
//    std::ofstream file(filename, std::ofstream::out|std::ofstream::binary);
//    if (!file.is_open()) return false;
//    for (int rowMajor = 0; rowMajor < height/4; rowMajor++) {
//        for (int rowMinor = 0; rowMinor < 4; rowMinor++) {
//            for (int colMajor = 0; colMajor < width/4; colMajor++) {
//                char tmp[8];
//                for (int colMinor = 0; colMinor < 4; colMinor++) {
//                    tmp[colMinor*2+1] = (char)((matrix[rowMajor*width/4+colMajor].getData()[rowMinor*4+colMinor] & 0xff00) >> 8);
//                    tmp[colMinor*2] = (char)(matrix[rowMajor*width/4+colMajor].getData()[rowMinor*4+colMinor] & 0x00ff);
//                }
//                file.write(tmp, 8*sizeof(char));
//            }
//        }
//    }
//    file.close();
//    return true;
//}
//
//bool RawFileParser::WriteFile16bit(std::string filename, short *array, int size) {
//    std::ofstream file(filename, std::ofstream::out|std::ofstream::binary);
//    if (!file.is_open()) return false;
//    char tmp[size*2];
//    for (int i = 0; i < size; i++) {
//        tmp[2*i+1] = (char)((array[i] & 0xff00) >> 8);
//        tmp[2*i] = (char)(array[i] & 0x00ff);
//    }
//    file.write(tmp, size*2*sizeof(char));
//    file.close();
//    return true;
//}
//
//short *RawFileParser::parseFile16bit(std::string filename, int &size) {
//    std::vector<short> data;
//    std::ifstream file(filename, std::ios::in|std::ios::binary);
//    if (!file.is_open()) return nullptr;
//    while(!file.eof()) {
//        char tmp[4];
//        file.read(tmp, 4*sizeof(char));
//        short v1 = ((short)tmp[0]&0x00ff)|((((short)tmp[1])&0x00ff)<<8);
//        short v2 = ((short)tmp[2]&0x00ff)|((((short)tmp[3])&0x00ff)<<8);
//        data.emplace_back(v1);
//        data.emplace_back(v2);
//    }
//    file.close();
//    size = static_cast<int>(data.size());
//    short* dataArray = new short[size];
//    memcpy(dataArray, &data[0], sizeof(short)*size);
//    return dataArray;
//}
//
//bool RawFileParser::writeFile8bit(std::string filename, uint8_t *data, int size) {
//    std::ofstream file(filename, std::ofstream::out|std::ofstream::binary);
//    if (!file.is_open()) return false;
//    file.write(reinterpret_cast<const char *>(data), size * sizeof(char));
//    file.close();
//    return true;
//}

ValueBlock4x4 *RawFileParser::readRawImageFile(std::string filename, int width, int height) {
    if (width % 4 != 0) return nullptr;
    if (height % 4 != 0) return nullptr;
    ValueBlock4x4 *image = new ValueBlock4x4[(width/4)*(height/4)];
    std::ifstream file(filename, std::ios::in|std::ios::binary);
    if (!file.is_open()) return nullptr;
    for (int rowMajor = 0; rowMajor < height/4; rowMajor++) {
        for (int rowMinor = 0; rowMinor < 4; rowMinor++) {
            for (int colMajor = 0; colMajor < width/4; colMajor++) {
                char tmp[4];
                file.read(tmp, 4*sizeof(char));
                for (int colMinor = 0; colMinor < 4; colMinor++) {
                    image[rowMajor * width / 4 + colMajor].getData()[rowMinor*4+colMinor] = static_cast<int16_t>((unsigned char)tmp[colMinor]);
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

uint8_t *RawFileParser::readEncodedFile(std::string filename, int &size) {
    std::vector<uint8_t> data;
    std::ifstream file(filename, std::ios::in|std::ios::binary);
    if (!file.is_open()) return nullptr;
    while (!file.eof()) {
        char tmp;
        file.read(&tmp, sizeof(char));
        data.emplace_back(static_cast<uint8_t>(tmp));
    }
    file.close();
    size = static_cast<int>(data.size());
    uint8_t* dataArray = new uint8_t[size];
    memcpy(dataArray, &data[0], sizeof(uint8_t)*size);
    return dataArray;
}

bool RawFileParser::writeEncodedFile(std::string filename, int size, uint8_t *data) {
    std::ofstream file(filename, std::ofstream::out|std::ofstream::binary);
    if (!file.is_open()) return false;
    for (int i = 0; i < size; i++) {
        char tmp = static_cast<char>(data[i]);
        file.write(&tmp, sizeof(char));
    }
    file.close();
    return true;
}

bool RawFileParser::writeRawImageFile(std::string filename, int width, int height, ValueBlock4x4 *data) {
    std::ofstream file(filename, std::ofstream::out|std::ofstream::binary);
    if (!file.is_open()) return false;
    for (int rowMajor = 0; rowMajor < height/4; rowMajor++) {
        for (int rowMinor = 0; rowMinor < 4; rowMinor++) {
            for (int colMajor = 0; colMajor < width/4; colMajor++) {
                char tmp[4];
                for (int colMinor = 0; colMinor < 4; colMinor++) {
                    tmp[colMinor] = (char)(static_cast<unsigned char>(data[rowMajor * width / 4 + colMajor].getData()[rowMinor*4+colMinor]));
                }
                file.write(tmp, 4*sizeof(char));
            }
        }
    }
    file.close();
    return true;
}
