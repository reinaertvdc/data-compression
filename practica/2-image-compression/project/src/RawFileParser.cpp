
#include <fstream>
#include <cstring>
#include "RawFileParser.h"

ByteMatrix *RawFileParser::parseFile(std::string filename, int width, int height) {
    ByteMatrix *image = new ByteMatrix[(width/4)*(height/4)];
    unsigned char rowPixels[width/4][4][4];
    std::ifstream file(filename, std::ios::in|std::ios::binary);
    if (!file.is_open()) return nullptr;
    for (int rowMajor = 0; rowMajor < height/4; rowMajor++) {
        for (int rowMinor = 0; rowMinor < 4; rowMinor++) {
            for (int colMajor = 0; colMajor < width/4; colMajor++) {
                file.read((char *)(rowPixels[colMajor][rowMinor]), 4);
            }
        }
        for (int colMajor = 0; colMajor < width/4; colMajor++) {
            memcpy(&image[rowMajor*(width/4)+colMajor], &rowPixels[colMajor], 16*sizeof(unsigned char));
        }
    }
    file.close();
    return image;
}
