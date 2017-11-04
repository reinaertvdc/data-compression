
#ifndef PROJECT_QUANTFILEPARSER_H
#define PROJECT_QUANTFILEPARSER_H


#include "ValueBlock4x4.h"
#include <string>
#include <vector>

class QuantFileParser {
public:
    static ValueBlock4x4 parseFile(std::string filename);

private:
    static std::vector<std::string> splitBySpaces(std::string line);
};


#endif //PROJECT_QUANTFILEPARSER_H
