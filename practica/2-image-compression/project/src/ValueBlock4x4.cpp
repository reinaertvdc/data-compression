
#include <cstring>
#include <iostream>
#include <cmath>
#include "ValueBlock4x4.h"

const double ValueBlock4x4::dctTransformMatrix[4][4] = {{0.5,   0.5,    0.5,    0.5},
                                                        {0.653, 0.271,  -0.271, -0.653},
                                                        {0.5,   -0.5,   -0.5,   0.5},
                                                        {0.271, -0.653, 0.653,  -0.271}};
const int ValueBlock4x4::zzPatternRow[16] = {0,0,1,2,1,0,0,1,2,3,3,2,1,2,3,3};
const int ValueBlock4x4::zzPatternCol[16] = {0,1,0,0,1,2,3,2,1,0,1,2,3,3,2,3};

ValueBlock4x4::ValueBlock4x4(int16_t matrix[4][4]) : empty(false) {
    std::memcpy(this->matrix, matrix, sizeof(int16_t) * 16);
}

ValueBlock4x4::ValueBlock4x4() : empty(true) {}

void ValueBlock4x4::applyDct() {
    if (this->isEmpty()) {
        return;
    }
    double tmp[4][4];
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            double sum = 0.0;
            for (int k = 0; k < 4; k++) {
                sum += ValueBlock4x4::dctTransformMatrix[r][k] * static_cast<double>(this->matrix[k][c]);
            }
            tmp[r][c] = sum;
        }
    }
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            double sum = 0.0;
            for (int k = 0; k < 4; k++) {
                sum += tmp[r][k] * ValueBlock4x4::dctTransformMatrix[c][k];
            }
            this->matrix[r][c] = static_cast<int16_t>(round(sum));
        }
    }
}

void ValueBlock4x4::applyInverseDct() {
    if (this->isEmpty()) {
        return;
    }
    double tmp[4][4];
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            double sum = 0.0;
            for (int k = 0; k < 4; k++) {
                sum += dctTransformMatrix[k][r] * static_cast<double>(this->matrix[k][c]);
            }
            tmp[r][c] = sum;
        }
    }
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            double sum = 0.0;
            for (int k = 0; k < 4; k++) {
                sum += tmp[r][k] * dctTransformMatrix[k][c];
            }
            this->matrix[r][c] = static_cast<int16_t>(round(sum));
        }
    }
}

void ValueBlock4x4::quantize(const ValueBlock4x4 &quant) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            this->matrix[r][c] = static_cast<int16_t>(nearbyint(
                    static_cast<double>(this->matrix[r][c]) / static_cast<double>(quant.matrix[r][c])));
        }
    }
}

void ValueBlock4x4::deQuantize(const ValueBlock4x4 &quant) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            this->matrix[r][c] = static_cast<int16_t>(static_cast<int>(this->matrix[r][c]) * static_cast<int>(quant.matrix[r][c]));
        }
    }
}

bool ValueBlock4x4::zigzag(int16_t* out) const {
    if (out == nullptr) return false;
    if (this->empty) return false;
    for (int i = 0; i < 16; i++) {
        out[i] = this->matrix[ValueBlock4x4::zzPatternRow[i]][ValueBlock4x4::zzPatternCol[i]];
    }
    return true;
}

ValueBlock4x4::ValueBlock4x4(const int16_t *array) {
    if (array == nullptr) return;
    for (int i = 0; i < 16; i++) {
        this->matrix[ValueBlock4x4::zzPatternRow[i]][ValueBlock4x4::zzPatternCol[i]] = array[i];
    }
    this->setFilled();
}
