
#ifndef PROJECT_QUANTMATRIX_H
#define PROJECT_QUANTMATRIX_H


class ValueBlock4x4 {
private:
    int16_t matrix[4][4];
    bool empty;
    static const double dctTransformMatrix[4][4];
    static const int zzPatternRow[16];
    static const int zzPatternCol[16];
public:
    ValueBlock4x4();

    explicit ValueBlock4x4(int16_t matrix[4][4]);

    explicit ValueBlock4x4(const int16_t *array);

    virtual ~ValueBlock4x4() = default;

    void setFilled() { this->empty = false; }

    bool isEmpty() { return this->empty; }

    int16_t *getData() { return (int16_t *) this->matrix; }

    void applyDct();

    void applyInverseDct();

    void quantize(const ValueBlock4x4 &quant);

    void deQuantize(const ValueBlock4x4 &quant);

    int16_t getValue(int row,
                     int col) const { if (this->empty) return static_cast<int16_t>(0); else return this->matrix[row][col]; }

    short getValueAsShort(int row,
                          int col) const { if (this->empty) return static_cast<short>(0); else return static_cast<short>(this->matrix[row][col]); }

    bool zigzag(int16_t *out) const;
};


#endif //PROJECT_QUANTMATRIX_H
