//
// Created by cwout on 16/10/17.
//

#ifndef PROJECT_RAWFILEPARSER_H
#define PROJECT_RAWFILEPARSER_H


#include "ByteMatrix.h"
#include <string>

class RawFileParser {
public:
    static ByteMatrix* parseFile(std::string filename, int width, int height);
};


#endif //PROJECT_RAWFILEPARSER_H
