//
// Created by cwout on 13/10/17.
//

#ifndef PROJECT_QUANTMATRIX_H
#define PROJECT_QUANTMATRIX_H


class QuantMatrix {
private:
    int matrix[4][4];
    bool empty;
public:
    QuantMatrix();
    explicit QuantMatrix(int matrix[4][4]);
    virtual ~QuantMatrix() = default;
    bool isEmpty() { return this->empty; }
};


#endif //PROJECT_QUANTMATRIX_H
