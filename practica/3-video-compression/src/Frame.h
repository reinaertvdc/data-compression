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
    bool writeI(std::ofstream &out, bool rle, const ValueBlock4x4 &quantMatrix);

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
                uint16_t merange);

private:
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
    bool loadI(uint8_t *data, int compressedSize, bool rle, const ValueBlock4x4 &quantMatrix);
    bool loadP(uint8_t *vectorBuffer, int vectorStreamSize, const ValueBlock4x4 &quantMatrix,
               const Frame &previousFrame, uint16_t merange, bool motionCompensation);
    void loadPixels() const;
    void readMacroBlock(int16_t *buffer, int macroRow, int macroCol) const;

    void writeMacroBlock(int16_t *buffer, int macroRow, int macroCol);

    void findClosestMatch(int16_t *macroBlock, int row, int col, uint16_t merange, int &matchRow, int &matchCol) const;

    void getMotionCompensation(int16_t *macroBlock, int row, int col);

    void applyMotionCompensation(int16_t *macroBlock, int row, int col);

    int getDifferenceIfLower(const int16_t *macroBlock, int row, int col, int currentMinDifference) const;
};


#endif //INC_3_VIDEO_COMPRESSION_FRAME_H
