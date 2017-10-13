//
// Created by cwout on 13/10/17.
//

#include <cstring>
#include "QuantMatrix.h"

QuantMatrix::~QuantMatrix() {

}

QuantMatrix::QuantMatrix(int matrix[4][4]) {
    std::memcpy(this->matrix, matrix, sizeof(int)*16);
}

