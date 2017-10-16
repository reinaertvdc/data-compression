//
// Created by cwout on 13/10/17.
//

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
            if (i == line.length()-1) {
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

ByteMatrix QuantFileParser::parseFile(std::string filename) {
    std::fstream file;
    file.open(filename, std::ios::in);
    std::string line;
    unsigned char mat[4][4];
    if (!file.is_open()) {
        return ByteMatrix();
    }
    int iRow = 0;
    while (getline(file, line)) {
        if (iRow >= 4) continue;
        std::vector<std::string> lineparts = QuantFileParser::splitBySpaces(line);
        if (lineparts.size() < 4) {
            file.close();
            return ByteMatrix();
        }
        int iCol = 0;
        for (std::string spart : lineparts) {
            if (iCol >= 4) continue;
            mat[iRow][iCol] = static_cast<unsigned char>(std::stoi(spart));
            iCol++;
        }
        iRow++;
    }
    file.close();
    return ByteMatrix(mat);
}
