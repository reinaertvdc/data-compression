
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

ValueBlock4x4::ValueBlock4x4(short matrix[4][4]) : empty(false) {
    std::memcpy(this->matrix, matrix, sizeof(short) * 16);
}

ValueBlock4x4::ValueBlock4x4() : empty(true) {}

void ValueBlock4x4::applyDct() {
    if (this->isEmpty()) {
        return;
    }
    double tmp[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double sum = 0.0;
            for (int k = 0; k < 4; k++) {
                sum += ValueBlock4x4::dctTransformMatrix[i][k] * static_cast<double>(this->matrix[k][j]);
            }
            tmp[i][j] = sum;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double sum = 0.0;
            for (int k = 0; k < 4; k++) {
                sum += tmp[i][k] * ValueBlock4x4::dctTransformMatrix[j][k];
            }
            this->matrix[i][j] = static_cast<short>(round(sum));
        }
    }
}

void ValueBlock4x4::applyInverseDct() {
    if (this->isEmpty()) {
        return;
    }
    double tmp[4][4] = {{0.0, 0.0, 0.0, 0.0},
                        {0.0, 0.0, 0.0, 0.0},
                        {0.0, 0.0, 0.0, 0.0},
                        {0.0, 0.0, 0.0, 0.0}};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double sum = 0.0;
            for (int k = 0; k < 4; k++) {
                sum += dctTransformMatrix[k][i] * static_cast<double>(this->matrix[k][j]);
            }
            tmp[i][j] = sum;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double sum = 0.0;
            for (int k = 0; k < 4; k++) {
                sum += tmp[i][k] * dctTransformMatrix[k][j];
            }
            this->matrix[i][j] = static_cast<short>(round(sum));
        }
    }
}

void ValueBlock4x4::quantize(const ValueBlock4x4 &quant) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->matrix[i][j] = static_cast<short>(nearbyint(
                    static_cast<double>(this->matrix[i][j]) / static_cast<double>(quant.matrix[i][j])));
        }
    }
}

void ValueBlock4x4::deQuantize(const ValueBlock4x4 &quant) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            this->matrix[i][j] = static_cast<short>(static_cast<int>(this->matrix[i][j]) * static_cast<int>(quant.matrix[i][j]));
        }
    }
}

bool ValueBlock4x4::zigzag(short* out) const {
    if (out == nullptr) return false;
    if (this->empty) return false;
    for (int i = 0; i < 16; i++) {
        out[i] = this->matrix[ValueBlock4x4::zzPatternRow[i]][ValueBlock4x4::zzPatternCol[i]];
    }
    return true;
}

ValueBlock4x4::ValueBlock4x4(const short *array) {
    if (array == nullptr) return;
    for (int i = 0; i < 16; i++) {
        this->matrix[ValueBlock4x4::zzPatternRow[i]][ValueBlock4x4::zzPatternCol[i]] = array[i];
    }
    this->setFilled();
}
