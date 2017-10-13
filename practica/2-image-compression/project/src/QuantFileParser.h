//
// Created by cwout on 13/10/17.
//

#ifndef PROJECT_QUANTFILEPARSER_H
#define PROJECT_QUANTFILEPARSER_H


#include "QuantMatrix.h"
#include <string>

class QuantFileParser {

public:
    static QuantMatrix parseFile(std::string filename);


};


#endif //PROJECT_QUANTFILEPARSER_H
