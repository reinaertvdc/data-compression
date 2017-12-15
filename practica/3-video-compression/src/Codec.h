#ifndef INC_3_VIDEO_COMPRESSION_CODEC_H
#define INC_3_VIDEO_COMPRESSION_CODEC_H


#include <cstring>
#include <iostream>

class Codec {
public:
    enum Action {
        ENCODE,
        DECODE
    };

    static int run(Action action, int argc, char *const argv[]);

private:
    static int encode(const std::string &configFilePath);
    static int decode(const std::string &configFilePath);
};


#endif //INC_3_VIDEO_COMPRESSION_CODEC_H
