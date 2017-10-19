//
// Created by cwout on 16/10/17.
//

#ifndef PROJECT_RAWFILEPARSER_H
#define PROJECT_RAWFILEPARSER_H


#include "ValueBlock4x4.h"
#include <string>

class RawFileParser {
public:
    static ValueBlock4x4* parseFile8bit(std::string filename, int width, int height);
    static bool writeFile8bit(std::string filename, ValueBlock4x4 *matrix, int width, int height);
    static ValueBlock4x4* parseFile16bit(std::string filename, int width, int height);
    static short* parseFile16bit(std::string filename, int size);
    static bool writeFile16bit(std::string filename, ValueBlock4x4 *matrix, int width, int height);
    static bool WriteFile16bit(std::string filename, short *array, int size);
};


#endif //PROJECT_RAWFILEPARSER_H
