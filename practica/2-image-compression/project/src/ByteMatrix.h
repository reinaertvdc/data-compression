//
// Created by cwout on 13/10/17.
//

#ifndef PROJECT_QUANTMATRIX_H
#define PROJECT_QUANTMATRIX_H


class ByteMatrix {
private:
    unsigned char matrix[4][4];
    bool empty;
    static const double dctTransformMatrix[4][4];
public:
    ByteMatrix();
    explicit ByteMatrix(unsigned char matrix[4][4]);
    virtual ~ByteMatrix() = default;
    bool isEmpty() { return this->empty; }
    unsigned char *getBytes() { return (unsigned char *)this->matrix; }
    void applyDct();
    void applyInverseDct();
};


#endif //PROJECT_QUANTMATRIX_H
