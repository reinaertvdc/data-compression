#ifndef INC_3_VIDEO_COMPRESSION_IFRAMECODEC_H
#define INC_3_VIDEO_COMPRESSION_IFRAMECODEC_H


#include <cstdint>
#include "ValueBlock4x4.h"

/**
 * Class to convert from and to the format to store the compressed data in a file
 */
class IFrameStorageCodec {
public:
    //TODO: remove function
    /**
     * Convert a int16_t array to a uint8_t array as compact as possible making use of the format of the arrays
     * @param data the input data array
     * @param size the size of the input array
     * @param outSize out the size of the output array
     * @param width the width of the image (in pixels)
     * @param height the height of the image (in pixels)
     * @param rle whether or not to use rle for compression
     * @return uint8_t array, the memory should be deleted by the calling class
     */
    static uint8_t *toStorageFormat(int16_t *data, int size, int &outSize, int width, int height, bool rle) __attribute__ ((deprecated));

    static uint8_t *toStorageFormat(ValueBlock4x4 *pixelBlocks, const ValueBlock4x4 &quantMatrix, int &storageSize, int width, int height, bool rle, bool differentialEncodeDc);
    static uint8_t *toStorageFormat(ValueBlock4x4 **pixelBlocks, const ValueBlock4x4 &quantMatrix, int &storageSize, int width, int height, bool rle, bool differentialEncodeDc);

    //TODO: remove function
    /**
     * Convert a compressed uint8_t array back to a normal short (int16_t) array making use of the format of the arrays
     * @param data the compressed data as input
     * @param size the size of the compressed data
     * @param outSize out the size of the output array
     * @param wOut out the width of the image (in pixels)
     * @param hOut out the height of the image (in pixels)
     * @param rleOut out whether of not RLE was used for compression
     * @return short (int16_t) array, the memory should be deleted by the calling class
     */
    static int16_t *fromStorageFormat(uint8_t *data, int size, int &outSize, int width, int height, bool rle) __attribute__ ((deprecated));

    static bool fromStorageFormat(uint8_t *data, int inSize, const ValueBlock4x4 &quantMatrix, int &outSize, int width, int height, bool rle, bool differentialEncodeAc, ValueBlock4x4 * pixelBlocks);
    static bool fromStorageFormat(uint8_t *data, int inSize, const ValueBlock4x4 &quantMatrix, int &outSize, int width, int height, bool rle, bool differentialEncodeAc, ValueBlock4x4 ** pixelBlocks);
private:
    static bool differentialEncode(int16_t* data, int size);
};


#endif //INC_3_VIDEO_COMPRESSION_IFRAMECODEC_H
