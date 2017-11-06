
#include "QuantFileParser.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>

std::vector<std::string> QuantFileParser::splitBySpaces(std::string line) {
    std::vector<std::string> ret = {};
    std::string cur = std::string();
    for (int i = 0; i < line.length(); i++) {
        if (line[i] != ' ') {
            cur += line[i];
            if (i == line.length() - 1) {
                ret.push_back(cur);
                cur = "";
            }
        } else if (cur.length() > 0) {
            ret.push_back(cur);
            cur = "";
        }
    }
    return ret;
}

ValueBlock4x4 QuantFileParser::parseFile(std::string filename) {
    std::fstream file;
    file.open(filename, std::ios::in);
    std::string line;
    int16_t mat[4][4];
    if (!file.is_open()) {
        return ValueBlock4x4();
    }
    int iRow = 0;
    while (getline(file, line)) {
        if (line.length() <= 3) continue;
        if (iRow >= 4) continue;
        std::vector<std::string> lineparts = QuantFileParser::splitBySpaces(line);
        if (lineparts.size() < 4) {
            file.close();
            return ValueBlock4x4();
        }
        int iCol = 0;
        for (const std::string &spart : lineparts) {
            if (iCol >= 4) continue;
            mat[iRow][iCol] = static_cast<int16_t>(std::stoi(spart));
            iCol++;
        }
        iRow++;
    }
    file.close();
    return ValueBlock4x4(mat);
}
