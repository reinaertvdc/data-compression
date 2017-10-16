//
// Created by cwout on 13/10/17.
//

#include <cstring>
#include <iostream>
#include "ByteMatrix.h"

ByteMatrix::ByteMatrix(unsigned char matrix[4][4]) : empty(false) {
    std::memcpy(this->matrix, matrix, sizeof(unsigned char)*16);
}

ByteMatrix::ByteMatrix() : empty(true) {}

