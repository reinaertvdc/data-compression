#ifndef INC_3_VIDEO_COMPRESSION_RLECODEC_H
#define INC_3_VIDEO_COMPRESSION_RLECODEC_H


#include <cstdint>

/**
 * Handles compression of an array of shorts based on the rle method
 */
class RleCodec {
public:
    static bool
    rleEncode(const int16_t *data, int16_t *&rleCounts, int16_t *&rleValues, int inputSize, int &outputSize);

    static bool rleDecode(int16_t *&data, const int16_t *rleCounts, const int16_t *rleValues, int rleSize, int &outputSize);

    static bool rleEncodeZeros(const int16_t *data, int16_t *&zeroCounts, int16_t *&nonZeros, int inputSize, int &outputSize, int maxZeros);

    static bool rleDecodeZeros(int16_t *&data, const int16_t *zeroCounts, const int16_t *nonZeros, int rleSize, int &outputSize);

    /**
     * Compresses an array of shorts (int16_t) using RLE
     * @param in the array to compress
     * @param size the size of the input array
     * @param outSize out the size of the output array
     * @return an array containing the compressed data of size (outSize), the calling class should delete the memory of the array
     */
//    static int16_t *rleEncode(int16_t *in, int size, int &outSize) __attribute__ ((deprecated));

    /**
     * Decompresses an array of shorts (int16_t) using RLE
     * @param in the compressed array
     * @param outSize the total size of the output, missing values are padded with 0's
     * @param inSizeUsed out the amount of array elements used
     * @return an array containing the decompressed data of size (outSize), the calling class should delete the memory of the array
     */
//    static int16_t *rleDecode(const int16_t *in, int outSize, int &inSizeUsed) __attribute__ ((deprecated));
};


#endif //INC_3_VIDEO_COMPRESSION_RLECODEC_H
