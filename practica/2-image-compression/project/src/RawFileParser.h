
#ifndef PROJECT_RAWFILEPARSER_H
#define PROJECT_RAWFILEPARSER_H


#include "ValueBlock4x4.h"
#include <string>

class RawFileParser {
public:
    static ValueBlock4x4 *readRawImageFile(std::string filename, int width, int height);

    static uint8_t *readEncodedFile(std::string filename, int &size);

    static bool writeEncodedFile(std::string filename, int size, uint8_t *data);

    static bool writeRawImageFile(std::string filename, int width, int height, ValueBlock4x4 *data);
};


#endif //PROJECT_RAWFILEPARSER_H
