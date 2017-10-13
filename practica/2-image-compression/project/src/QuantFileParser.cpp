//
// Created by cwout on 13/10/17.
//

#include "QuantFileParser.h"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>

std::vector<std::string> splitBySpaces(std::string line) {
    std::vector<std::string> ret = {};
    std::string cur = std::string();
    for (int i = 0; i < line.length(); i++) {
        if (line[i] != ' ') {
            cur += line[i];
        } else if (cur.length() > 0) {
            ret.push_back(cur);
        }
    }
}

QuantMatrix QuantFileParser::parseFile(std::string filename) {
    std::ifstream file(filename);
    std::string line;
    if (!file.is_open()) {
        //TODO
    }
    while (getline(file, line)) {
        std::cout << splitBySpaces(line) << std::endl;
    }


    return QuantMatrix(nullptr);
}
