#ifndef INC_3_VIDEO_COMPRESSION_VIDEOPROCESSOR_H
#define INC_3_VIDEO_COMPRESSION_VIDEOPROCESSOR_H


#include <fstream>
#include "ValueBlock4x4.h"

class VideoCodec {
public:
    /**
     * Encode a raw video file
     * @param in the raw video file
     * @param inEnd the size of the raw video file
     * @param out the output file (encoded)
     * @param width the width of the frames
     * @param height the height of the frames
     * @param rle whether or not to use rle
     * @param quantMatrix the quantization matrix to quantize dct components for compression
     * @param gop the Group Of Pictures parameter (see EncoderConfig::gop)
     * @param merange the merange parameter (see EncoderConfig::merange)
     * @return true if success, false otherwise
     */
    bool encode(std::ifstream &in, long inEnd, std::ofstream &out,
                uint16_t width, uint16_t height,
                bool rle, ValueBlock4x4 &quantMatrix, uint16_t gop, uint16_t merange);

    /**
     * Decode a (compressed) encoded video file
     * @param in the encoded file
     * @param inEnd the size of the encoded file
     * @param out the output file (raw)
     * @param motionCompensation whether or not motion compensation should be applied during decoding
     * @return true if success, false otherwise
     */
    bool decode(std::ifstream &in, long inEnd, std::ofstream &out, bool motionCompensation);
};


#endif //INC_3_VIDEO_COMPRESSION_VIDEOPROCESSOR_H
