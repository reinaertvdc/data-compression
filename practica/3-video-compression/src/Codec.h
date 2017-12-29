#ifndef INC_3_VIDEO_COMPRESSION_CODEC_H
#define INC_3_VIDEO_COMPRESSION_CODEC_H


#include <cstring>
#include <iostream>

/**
 * Main class acting as a controller to issue the correct action (encode or decode)
 */
class Codec {
public:
    /**
     * Enumeration to specify action for the coded (encode or decode)
     */
    enum Action {
        /**
         * encode action
         */
                ENCODE,
        /**
         * decode action
         */
                DECODE
    };

    /**
     * Runs the codec with a specified action, reads base arguments from the given commandline arguments
     * @param action the action to execute (encode/decode)
     * @param argc the count of arguments passed via <argv>
     * @param argv array of c-strings containing the arguments passed to the program
     * @return 0 on success, -1 (or any non 0 value) in case of failure
     */
    static int run(Action action, int argc, char *const argv[]);

private:
    /**
     * Encode action
     * @param configFilePath the path to the config file
     * @return 0 on success, -1 (or any non 0 value) in case of failure
     */
    static int encode(const std::string &configFilePath);

    /**
     *
     * @param configFilePath the path to the config file
     * @return 0 on success, -1 (or any non 0 value) in case of failure
     */
    static int decode(const std::string &configFilePath);
};


#endif //INC_3_VIDEO_COMPRESSION_CODEC_H
