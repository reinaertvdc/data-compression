#ifndef INC_3_VIDEO_COMPRESSION_ENCODERCONFIG_H
#define INC_3_VIDEO_COMPRESSION_ENCODERCONFIG_H

#include <iostream>
#include "Config.h"

/**
 * Encoder config class
 * Handles config values specific to the encoder
 */
class EncoderConfig : public Config {
public:
    /**
     * Constructor
     * Calls Config super constructor
     * Initializes the config parameters specific to the encoder
     * @param filePath the path to the encoder config file
     */
    explicit EncoderConfig(const std::string &filePath);

    /**
     * The raw file path
     */
    const std::string rawfile;

    /**
     * The encoded file path
     */
    const std::string encfile;

    /**
     * The width of the video
     */
    const int width;

    /**
     * The height of the video
     */
    const int height;

    /**
     * Whether or not to use rle compression to encode the video (true in case rle should be used)
     */
    const bool rle;

    /**
     * The quantization matrix file path
     */
    const std::string quantfile;

    /**
     * The logfile file path
     */
    const std::string logfile;

    /**
     * Group of pictures parameter
     * (the amount of frames within a single group,
     *  a group starts with an iframe and contains all successive frames that are not iframes)
     */
    const int gop;

    /**
     * Merange parameter, this parameter specifies the size of the search window for motion vectors
     * This value specifies the number of pixels in each direction that should be checked for calculation motion vectors
     */
    const int merange;
};

#endif //INC_3_VIDEO_COMPRESSION_ENCODERCONFIG_H
