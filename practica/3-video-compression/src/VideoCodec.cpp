#include <iostream>
#include "VideoCodec.h"
#include "Frame.h"

bool VideoCodec::encode(std::ifstream &in, long inEnd, std::ofstream &out, uint16_t width, uint16_t height, bool rle,
                        ValueBlock4x4 &quantMatrix, uint16_t gop, uint16_t merange) {
    uint8_t configBuffer[24];

    ((uint16_t*) configBuffer)[0] = static_cast<uint16_t>(width / 4);
    ((uint16_t*) configBuffer)[1] = static_cast<uint16_t>(height / 4);
    ((uint16_t*) configBuffer)[2] = gop;
    ((uint16_t*) configBuffer)[3] = merange;

    if (rle) {
        ((uint16_t*) configBuffer)[2] += 32768;
    }

    quantMatrix.toUint8Buffer(&configBuffer[8], 4);

    out.write(reinterpret_cast<char *>(&configBuffer), sizeof(uint8_t) * 24);

    Frame frame1(width, height), frame2(width, height), frame3(width, height);
    Frame *previousFrame = &frame1;
    Frame *frame = &frame2;
    Frame *workFrame = &frame3;

    while (in.tellg() < inEnd) {
        frame->readRaw(in);
        workFrame->copy(*frame);
        workFrame->writeI(out, rle, quantMatrix);

        for (int i = 1; i < gop && in.tellg() < inEnd; i++) {
            Frame *temp = previousFrame;
            previousFrame = frame;
            frame = temp;

            frame->readRaw(in);
            workFrame->copy(*frame);
            workFrame->writeP(out, rle, quantMatrix, *previousFrame, gop, merange);
        }
    }

    return true;
}

bool VideoCodec::decode(std::ifstream &in, long inEnd, std::ofstream &out, bool motionCompensation) {
    uint8_t configBuffer[24];

    in.read(reinterpret_cast<char *>(&configBuffer), sizeof(uint8_t) * 24);

    bool rle = false;

    if (((uint16_t*) configBuffer)[2] >= 32768) {
        rle = true;
        ((uint16_t*) configBuffer)[2] -= 32768;
    }

    int width = ((uint16_t*) configBuffer)[0] * 4;
    int height = ((uint16_t*) configBuffer)[1] * 4;
    uint16_t gop = ((uint16_t*) configBuffer)[2];
    uint16_t merange = ((uint16_t*) configBuffer)[3];

    ValueBlock4x4 quantMatrix;
    quantMatrix.fromUint8Buffer(&configBuffer[8], 4);

    Frame frame1(width, height), frame2(width, height);
    Frame *previousFrame = &frame1;
    Frame *frame = &frame2;

    while (in.tellg() < inEnd) {
        frame->readI(in, rle, quantMatrix);
        frame->writeRaw(out);

        for (int i = 1; i < gop && in.tellg() < inEnd; i++) {
            Frame *temp = previousFrame;
            previousFrame = frame;
            frame = temp;

            frame->readP(in, rle, quantMatrix, *previousFrame, gop, merange, motionCompensation);
            frame->writeRaw(out);
        }
    }

    return true;
}
