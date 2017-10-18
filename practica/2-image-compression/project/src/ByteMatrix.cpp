//
// Created by cwout on 13/10/17.
//

#include <cstring>
#include <iostream>
#include <cmath>
#include "ByteMatrix.h"

const double ByteMatrix::dctTransformMatrix[4][4] = {{0.5,0.5,0.5,0.5},{0.653,0.271,-0.271,-0.653},{0.5,-0.5,-0.5,0.5},{0.271,-0.653,0.653,-0.271}};
//const double ByteMatrix::dctTransformMatrix[4][4] = {{1,1,1,1},{1,1,1,1},{1,1,1,1},{1,1,1,1}};

ByteMatrix::ByteMatrix(unsigned char matrix[4][4]) : empty(false) {
    std::memcpy(this->matrix, matrix, sizeof(unsigned char)*16);
}

ByteMatrix::ByteMatrix() : empty(true) {}

void ByteMatrix::applyDct() {
    if (this->isEmpty()) {
        return;
    }
    double tmp[4][4] = {{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0}};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double sum = 0.0;
            for (int k = 0; k < 4; k++) {
                sum += ByteMatrix::dctTransformMatrix[i][k] * static_cast<double>(this->matrix[k][j]);
            }
            tmp[i][j] = sum;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double sum = 0.0;
            for (int k = 0; k < 4; k++) {
                sum += tmp[i][k] * ByteMatrix::dctTransformMatrix[j][k];
            }
            this->matrix[i][j] = (unsigned char)(static_cast<char>(round(sum)));
        }
    }
}

void ByteMatrix::applyInverseDct() {
    if (this->isEmpty()) {
        return;
    }
    double tmp[4][4] = {{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0}};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double sum = 0.0;
            for (int k = 0; k < 4; k++) {
                sum += dctTransformMatrix[k][i] * static_cast<double>((char)(this->matrix[k][j]));
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
            this->matrix[i][j] = (unsigned char)(static_cast<char>(round(sum)));
        }
    }
}
