#ifndef INC_3_VIDEO_COMPRESSION_FRAME_H
#define INC_3_VIDEO_COMPRESSION_FRAME_H


#include <vector>
#include "ValueBlock4x4.h"

class Frame {
public:
    /**
     * Constructor
     * Ensures the Frame::frameBuffer and Frame::pixels arrays are large
     *     enough to contain the raw frame data and the frame pixel data
     * @param width The width of the frame
     * @param height The height of the frame
     */
    Frame(int width, int height);

    /**
     * Destructor
     */
    ~Frame();

//    bool copy(const Frame &other);

    /**
     * Read a frame from a file in raw format
     * @param in the file to read from
     * @return true in case of success, false in case of failure
     */
    bool readRaw(std::ifstream &in);

    /**
     * Read a frame from a file in compressed iframe format
     * @param in the file to read from
     * @param rle whether or not the iframe was compressed using rle
     * @param quantMatrix the quantization matrix used to quantize the dct components in the iframe
     * @return true in case of success, false in case of failure
     */
    bool readI(std::ifstream &in, bool rle, const ValueBlock4x4 &quantMatrix);

    /**
     * Read a frame from a file in compressed pframe format
     * @param in the file to read from
     * @param rle whether or not the pframe was compressed using rle
     * @param quantMatrix the quantization matrix used to quantize the motion compensation values
     * @param previousFrame the frame used as reference for the pframe, on which motion vectors are based
     * @param merange the merange parameter for the video compression (see EncoderConfig::merange)
     * @param motionCompensation whether or not motion compensation was used for the encoded file
     * @return true in case of success, false in case of failure
     */
    bool readP(std::ifstream &in, bool rle, const ValueBlock4x4 &quantMatrix, const Frame &previousFrame,
               uint16_t merange, bool motionCompensation);

    /**
     * Write the frame as a raw frame to a file
     * @param out the file to write to
     * @return true in case of success, false in case of failure
     */
    bool writeRaw(std::ofstream &out);

    /**
     * Write the frame as a compressed iframe to a file
     * @param out the file to write to
     * @param rle whether or not to use rle to compress the iframe
     * @param quantMatrix the quantization matrix to use for quantizing the dct components
     * @return true in case of success, false in case of failure
     */
    bool writeI(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix, double &compressionTime);

    /**
     * Write the frame as a compressed pframe to a file
     * @param out the file to write to
     * @param rle whether or not to use rle to compress the pframe
     * @param quantMatrix the quantization matrix to use for quantizing the dct components
     * @param previousFrame the frame used as reference for the pframe, on which motion vectors are based
     * @param merange whether or not motion compensation was used for the encoded file
     * @return true in case of success, false in case of failure
     */
    bool writeP(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix, const Frame &previousFrame,
                uint16_t merange, double &compressionTime);

private:
    static constexpr double rawFrameSizeToPixelsRatio = 1.5;
    static constexpr int blockWidth = 4;
    static constexpr int blockHeight = blockWidth;
    static constexpr int blockSize = blockWidth * blockHeight;
    static constexpr int blocksPerMacroRow = 4;
    static constexpr int blocksPerMacroCol = blocksPerMacroRow;
    static constexpr int macroBlockWidth = blockWidth * blocksPerMacroRow;
    static constexpr int macroBlockHeight = blockWidth * blocksPerMacroCol;
    static constexpr int macroBlockSize = macroBlockWidth * macroBlockHeight;
    /**
     * Number of Frame class instances (constructor/destructor changes this value)
     */
    static int numInstances;
    /**
     * The size of the dynamically allocated frame buffer array
     */
    static int frameBufferSize;
    /**
     * Dynamically allocated frame buffer array
     * Used for reading/writing frames from/to files
     */
    static uint8_t *frameBuffer;
    /**
     * Dynamically allocated pixel array
     */
    static int16_t *pixels;
    /**
     * Width/height of the frame
     */
    const int width, height;
    /**
     * The size of the raw frame data (w*h*1.5 bytes)
     */
    const int rawSize;
    /**
     * The amount of ValueBlock4x4's to represent the pixel data
     */
    const int numBlocks;
    /**
     * The amount of 16x16 macroblocks to represent to pixel data
     */
    const int numMacroBlocks;
    /**
     * The horizontal amount of ValueBlock4x4's to represent the pixel data
     */
    const int numBlocksPerRow;
    /**
     * The vertical amount of ValueBlock4x4's to represent the pixel data
     */
    const int numBlocksPerCol;
    /**
     * The horizontal amount of ValueBlock4x4's to represent the pixel data
     */
    const int numMacroBlocksPerRow;
    /**
     * The vertical amount of ValueBlock4x4's to represent the pixel data
     */
    const int numMacroBlocksPerCol;
    /**
     * Valueblock array representing the pixels in the frame
     */
    ValueBlock4x4 **const blocks;

    /**
     * Load iframe blocks (ValueBlock4x4) from a buffer and decompress the blocks
     * @param data the compressed frame data buffer
     * @param compressedSize the size of the compressed frame data
     * @param rle whether or not rle was used to compress the frame data
     * @param quantMatrix the quantization matrix used to quantize the dct components
     * @return true if success, false otherwise
     */
    bool loadI(uint8_t *data, int compressedSize, bool rle, const ValueBlock4x4 &quantMatrix);

    /**
     * Load pframe macroblocks from a buffer and decompress the macroblocks
     * @param vectorBuffer the buffer containing the (compressed) motion vectors
     * @param vectorStreamSize the size of the motion vector buffer
     * @param quantMatrix the quantization matrix used to quantize the dct components for the motion compensation blocks
     * @param previousFrame the frame on which to base the motion vectors
     * @param merange the merange parameter (see EncoderConfig::merange)
     * @param motionCompensation whether or not motion compensation was used
     * @return true if success, false otherwise
     */
    bool loadP(int16_t *vectorBufferX, int16_t *vectorBufferY, int vectorStreamSize, const ValueBlock4x4 &quantMatrix,
               const Frame &previousFrame, uint16_t merange, bool motionCompensation);

    /**
     * Load pixels from ValueBlock4x4's into a continuous array
     */
    void loadPixels() const;

    /**
     * Copy a 16x16 macroblock into a buffer
     * @param buffer the buffer to copy to
     * @param macroRow the macroblock row
     * @param macroCol the macroblock column
     */
    void readMacroBlock(int16_t *buffer, int macroRow, int macroCol) const;

    /**
     * Copy a buffer into a 16x16 macroblock
     * @param buffer the buffer that contains the data
     * @param macroRow the macroblock row
     * @param macroCol the macroblock column
     */
    void writeMacroBlock(int16_t *buffer, int macroRow, int macroCol);

    /**
     * Calculate the closest motion vector for a given macroblock
     * @param macroBlock the macroblock data buffer to calculate motion vectors for
     * @param row the row (in actual pixel size) which is the topmost row of the 16x16 macroBlock
     * @param col the column (in actual pixel size) which is the leftmost column of the 16x16 macroBlock
     * @param merange the merange parameter (see EncoderConfig::merange)
     * @param matchRow out the topmost row (in actual pixel size) which contains the closest matching macroblock
     * @param matchCol out the leftmost column (in actual pixel size) which contains the closest matching macroblock
     */
    void findClosestMatch(int16_t *macroBlock, int row, int col, uint16_t merange, int &matchRow, int &matchCol) const;

    /**
     * Calculate the motion compensation data
     * @param macroBlock in the macroblock to calculate the motion compensation for
     *                   out the motion compensation values are stored in the buffer itself
     * @param row the row (topmost, in actual pixel size) to compare the macroblock against
     * @param col the column (lefmost, in actual pixel size) to compare the macroblock against
     */
    void getMotionCompensation(int16_t *macroBlock, int row, int col);

    /**
     * Apply a motion compenstation to a specific macroblock in the frame
     * @param macroBlock the motion compensation values to apply
     * @param row the row (topmost, in actual pixel size) to apply the motion compensation to
     * @param col the column (lefmost, in actual pixel size) to apply the motion compensation to
     */
    void applyMotionCompensation(int16_t *macroBlock, int row, int col);

    /**
     * Find the difference between a macroblock buffer and a macroblock given by the left/top-most row/column
     * If the difference is >= currentMinDifference, then return a value >= currentMinValue
     * @param macroBlock the macroblock to compare against
     * @param row the row (topmost, in actual pixel size) of the macroblock to compare
     * @param col the column (leftmost, in actual pixel size) of the macroblock to compare
     * @param currentMinDifference the difference value that should not be surpassed
     * @return the difference between the macroblocks if the difference is smaller than currentMinDifference
     *         otherwise a value >= currentMinDifference
     */
    inline int getDifferenceNotHigher(const int16_t *macroBlock, int row, int col, int currentMinDifference) const;
};


#endif //INC_3_VIDEO_COMPRESSION_FRAME_H
