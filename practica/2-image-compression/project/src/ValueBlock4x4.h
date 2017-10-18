//
// Created by cwout on 13/10/17.
//

#ifndef PROJECT_QUANTMATRIX_H
#define PROJECT_QUANTMATRIX_H


class ValueBlock4x4 {
private:
    short matrix[4][4];
    bool empty;
    static const double dctTransformMatrix[4][4];
public:
    ValueBlock4x4();
    explicit ValueBlock4x4(short matrix[4][4]);
    virtual ~ValueBlock4x4() = default;
    bool isEmpty() { return this->empty; }
    short *getData() { return (short *)this->matrix; }
    void applyDct();
    void applyInverseDct();
    void setFilled() { this->empty = false; }
};


#endif //PROJECT_QUANTMATRIX_H
