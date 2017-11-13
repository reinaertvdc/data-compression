
#ifndef PROJECT_QUANTMATRIX_H
#define PROJECT_QUANTMATRIX_H


#include <cstdint>

/**
 * Class containing a 4x4 block of int16_t (short) values
 */
class ValueBlock4x4 {
private:
    /**
     * The values
     */
    int16_t matrix[4][4];
    /**
     * Flag to enable an empty (uninitialized block)
     */
    bool empty;
    /**
     * The DCT transformation matrix
     */
    static const double dctTransformMatrix[4][4];
    /**
     * The zigzag pattern, enables faster zigzag transformation
     */
    static const int zzPatternRow[16];
    static const int zzPatternCol[16];
public:
    /**
     * Constructor, sets the empty flag
     */
    ValueBlock4x4();
    /**
     * Constructor, initializes the block with values
     * @param matrix the values to initialize the block with, copies the values
     */
    explicit ValueBlock4x4(int16_t matrix[4][4]);
    /**
     * Constructor, initializes the block with values in a zigzag pattern
     * @param array an array of values in zigzag pattern
     */
    explicit ValueBlock4x4(const int16_t *array);
    /**
     * Set the block to contain values, useful when setting values using the matrix point
     */
    void setFilled() { this->empty = false; }
    /**
     * Check whether or not the block is empty (does not contain values)
     * @return true if empty, false otherwise
     */
    bool isEmpty() { return this->empty; }
    /**
     * Get a pointer to the matrix in this class
     * @return pointer to the matrix (DO NOT DELETE)
     */
    int16_t *getData() { return (int16_t *) this->matrix; }
    /**
     * Apply the DCT algorithm to the matrix in this class
     */
    void applyDct();
    /**
     * Apply the inverse DCT algorithm to the matrix in this class
     */
    void applyInverseDct();
    /**
     * Apply quantization to the matrix in this class given a quantization matrix
     * @param quant the quantization matrix to use
     */
    void quantize(const ValueBlock4x4 &quant);
    /**
     * Apply inverse quantization to the matrix in this class given a quantization matrix
     * @param quant the quantization matrix to use
     */
    void deQuantize(const ValueBlock4x4 &quant);
    /**
     * Get a specific value of the matrix in this class given a row and column in range [ 0 , 3 ]
     * @param row
     * @param col
     * @return the requested value
     */
    int16_t getValue(int row,
                     int col) const { if (this->empty) return static_cast<int16_t>(0); else return this->matrix[row][col]; }

    /**
     * Apply a zigzag pattern to the matrix and store the values in a array
     * @param out the array to store the zigzagged values in, this array should have a size of at least 16 int16_t values
     * @return true if succes, false otherwise
     */
    bool zigzag(int16_t *out) const;
};


#endif //PROJECT_QUANTMATRIX_H
