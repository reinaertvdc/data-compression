
#include <cstring>
#include <iostream>
#include <cmath>
#include "ValueBlock4x4.h"

const double ValueBlock4x4::dctTransformMatrix[4][4] = {{0.5,0.5,0.5,0.5},{0.653,0.271,-0.271,-0.653},{0.5,-0.5,-0.5,0.5},{0.271,-0.653,0.653,-0.271}};

ValueBlock4x4::ValueBlock4x4(short matrix[4][4]) : empty(false) {
    std::memcpy(this->matrix, matrix, sizeof(short)*16);
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
    double tmp[4][4] = {{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0}};
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
