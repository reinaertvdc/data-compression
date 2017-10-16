//
// Created by cwout on 13/10/17.
//

#include <cstring>
#include <iostream>
#include "QuantMatrix.h"

QuantMatrix::QuantMatrix(int matrix[4][4]) : empty(false) {
    std::memcpy(this->matrix, matrix, sizeof(int)*16);
}

QuantMatrix::QuantMatrix() : empty(true) {}

