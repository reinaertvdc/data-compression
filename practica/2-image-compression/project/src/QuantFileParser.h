//
// Created by cwout on 13/10/17.
//

#ifndef PROJECT_QUANTFILEPARSER_H
#define PROJECT_QUANTFILEPARSER_H


#include "ByteMatrix.h"
#include <string>
#include <vector>

class QuantFileParser {
public:
    static ByteMatrix parseFile(std::string filename);
private:
    static std::vector<std::string> splitBySpaces(std::string line);
};


#endif //PROJECT_QUANTFILEPARSER_H
