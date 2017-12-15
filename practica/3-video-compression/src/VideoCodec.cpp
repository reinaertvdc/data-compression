#include <iostream>
#include "VideoCodec.h"
#include "Frame.h"

bool VideoCodec::encode(std::ifstream &in, long inEnd, std::ofstream &out, int width, int height, bool rle,
                            ValueBlock4x4 &quantMatrix, int gop, int merange) {
    out.write(reinterpret_cast<char *>(&width), sizeof(int));
    out.write(reinterpret_cast<char *>(&height), sizeof(int));

    Frame frame = Frame();

    while (in.tellg() < inEnd) {
        frame.readRaw(in, width, height);

        frame.writeI(out);
    }

    return true;
}

bool VideoCodec::decode(std::ifstream &in, long inEnd, std::ofstream &out, bool motionCompensation) {
    int width, height;

    in.read(reinterpret_cast<char *>(&width), sizeof(int));
    in.read(reinterpret_cast<char *>(&height), sizeof(int));

    Frame frame = Frame();

    while (in.tellg() < inEnd) {
        frame.readI(in, width, height);

        frame.writeRaw(out);
    }

    return true;
}
