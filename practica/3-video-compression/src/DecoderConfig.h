#ifndef INC_3_VIDEO_COMPRESSION_DECODERCONFIG_H
#define INC_3_VIDEO_COMPRESSION_DECODERCONFIG_H


#include <iostream>
#include "Config.h"

/**
 * Decoder config class
 * Handles config values specific to the decoder
 */
class DecoderConfig : public Config {
public:
    /**
     * Constructor
     * Calls Config super constructor
     * Initializes the config parameters specific to the decoder
     * @param filePath the path to the decoder config file
     */
    explicit DecoderConfig(const std::string &filePath);

    /**
     * The decoded file path
     */
    const std::string decfile;

    /**
     * Then encoded file path
     */
    const std::string encfile;

    /**
     * The logfile file path
     */
    const std::string logfile;

    /**
     * Whether or not motion compensation should be used (true if it should be)
     */
    const bool motioncompensation;
};

#endif //INC_3_VIDEO_COMPRESSION_DECODERCONFIG_H
