#ifndef INC_3_VIDEO_COMPRESSION_IFRAMECODEC_H
#define INC_3_VIDEO_COMPRESSION_IFRAMECODEC_H


#include <cstdint>
#include "ValueBlock4x4.h"

/**
 * Class to convert from and to the format to store the compressed data in a file
 */
class IFrameCodec {
public:
    /**
     * Convert a int16_t array to a uint8_t array as compact as possible making use of the format of the arrays
     * @param data the input data array
     * @param size the size of the input array
     * @param outSize out the size of the output array
     * @param width the width of the image (in pixels)
     * @param height the height of the image (in pixels)
     * @param rle whether or not RLE is used
     * @param quant the quantization matrix
     * @return uint8_t array, the memory should be deleted by the calling class
     */
    static uint8_t *
    toStorageFormat(int16_t *data, int size, int &outSize, int width, int height, bool rle, const ValueBlock4x4 &quant);

    /**
     * Convert a compressed uint8_t array back to a normal short (int16_t) array making use of the format of the arrays
     * @param data the compressed data as input
     * @param size the size of the compressed data
     * @param outSize out the size of the output array
     * @param wOut out the width of the image (in pixels)
     * @param hOut out the height of the image (in pixels)
     * @param rleOut out whether of not RLE is used
     * @param quant out the quantization matrix
     * @return short (int16_t) array, the memory should be deleted by the calling class
     */
    static int16_t *
    fromStorageFormat(uint8_t *data, int size, int &outSize, int width, int height, bool rle, const ValueBlock4x4 &quant);
};


#endif //INC_3_VIDEO_COMPRESSION_IFRAMECODEC_H
