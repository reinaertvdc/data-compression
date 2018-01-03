#include <iostream>
#include <sstream>
#include <iomanip>
#include "VideoCodec.h"
#include "Frame.h"
#include "Logger.h"

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

    Frame frame1(width, height), frame2(width, height);
    Frame *previousFrame = &frame1;
    Frame *frame = &frame2;

    long totalOutputSize = out.tellp();
    long totalPframeSize = 0;
    long totalIframeSize = 0;
    long totalPframeCount = 0;
    long totalIframeCount = 0;
    double totalPframeCompressionTime = 0;
    double totalIframeCompressionTime = 0;

    while (in.tellg() < inEnd) {
        frame->readRaw(in);
        double iCompressTimeTmp;
        frame->writeI(out, rle, quantMatrix, iCompressTimeTmp);
        totalIframeCompressionTime += iCompressTimeTmp;

        totalIframeCount++;
        long newOutputSize = out.tellp();
        totalIframeSize += newOutputSize - totalOutputSize;
        totalOutputSize = newOutputSize;

        for (int i = 1; i < gop && in.tellg() < inEnd; i++) {
            Frame *temp = previousFrame;
            previousFrame = frame;
            frame = temp;
            frame->readRaw(in);
            double pCompressTimeTmp;
            frame->writeP(out, rle, quantMatrix, *previousFrame, merange, pCompressTimeTmp);
            totalPframeCompressionTime += pCompressTimeTmp;

            totalPframeCount++;
            newOutputSize = out.tellp();
            totalPframeSize += newOutputSize - totalOutputSize;
            totalOutputSize = newOutputSize;
        }
    }

    long totalFrameCount = totalIframeCount + totalPframeCount;
    long totalFrameSizeUncompressed = inEnd;
    long totalFrameSizeCompressed = totalOutputSize;
    long averageFrameSizeUncompressed = totalFrameSizeUncompressed / totalFrameCount;
    long averageFrameSizeCompressed = totalFrameSizeCompressed / totalFrameCount;
    long averagePframeSize = (totalPframeCount > 0) ? (totalPframeSize / totalPframeCount) : 0;
    long averageIframeSize = totalIframeSize / totalIframeCount;
    double compressionRateTotal = static_cast<double>(totalFrameSizeCompressed) / static_cast<double>(totalFrameSizeUncompressed) * 100.0;
    double compressionRateIframe = static_cast<double>(totalIframeSize) / static_cast<double>(totalIframeCount * averageFrameSizeUncompressed) * 100.0;
    double compressionRatePframe = static_cast<double>(totalPframeSize) / static_cast<double>(totalPframeCount * averageFrameSizeUncompressed) * 100.0;
    double totalCompressionTime = totalPframeCompressionTime + totalIframeCompressionTime;
    double averageCompressionTime = totalCompressionTime / totalFrameCount;
    double averagePcompressionTime = totalPframeCompressionTime / totalPframeCount;
    double averageIcompressionTime = totalIframeCompressionTime / totalIframeCount;

    std::cout << totalCompressionTime << std::endl;

    std::stringstream ss;
    ss << std::fixed;
    ss << "Compression rates and sizes" << std::endl;
    ss << "        "
       << "      rate"
       << "       avg.comp.size"
       << "       avg.norm.size"
       << "   avg.eff.comp.time"
       << "         frame.count"
       << "       tot.comp.size"
       << "       tot.norm.size"
       << "   tot.eff.comp.time"
       << std::endl;
    ss << "total   "
       << std::setprecision(2) << std::setw(9)  << compressionRateTotal << "%"
       << std::setprecision(0) << std::setw(20) << averageFrameSizeCompressed
       << std::setprecision(0) << std::setw(20) << averageFrameSizeUncompressed
       << std::setprecision(1) << std::setw(18) << averageCompressionTime*1000.0 << "ms"
       << std::setprecision(0) << std::setw(20) << totalFrameCount
       << std::setprecision(0) << std::setw(20) << totalFrameSizeCompressed
       << std::setprecision(0) << std::setw(20) << totalFrameSizeUncompressed
       << std::setprecision(1) << std::setw(18) << totalCompressionTime*1000.0 << "ms"
       << std::endl;
    ss << "iframe  "
       << std::setprecision(2) << std::setw(9) << compressionRateIframe << "%"
       << std::setprecision(0) << std::setw(20) << averageIframeSize
       << std::setprecision(0) << std::setw(20) << averageFrameSizeUncompressed
       << std::setprecision(1) << std::setw(18) << averageIcompressionTime*1000.0 << "ms"
       << std::setprecision(0) << std::setw(20) << totalIframeCount
       << std::setprecision(0) << std::setw(20) << totalIframeSize
       << std::setprecision(0) << std::setw(20) << (totalIframeCount * averageFrameSizeUncompressed)
       << std::setprecision(1) << std::setw(18) << totalIframeCompressionTime*1000.0 << "ms"
       << std::endl;
    ss << "pframe  "
       << std::setprecision(2) << std::setw(9) << compressionRatePframe << "%"
       << std::setprecision(0) << std::setw(20) << averagePframeSize
       << std::setprecision(0) << std::setw(20) << averageFrameSizeUncompressed
       << std::setprecision(1) << std::setw(18) << averagePcompressionTime*1000.0 << "ms"
       << std::setprecision(0) << std::setw(20) << totalPframeCount
       << std::setprecision(0) << std::setw(20) << totalPframeSize
       << std::setprecision(0) << std::setw(20) << (totalPframeCount * averageFrameSizeUncompressed)
       << std::setprecision(1) << std::setw(18) << totalPframeCompressionTime*1000.0 << "ms"
       << std::endl;
    Logger::info(ss.str());

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

            frame->readP(in, rle, quantMatrix, *previousFrame, merange, motionCompensation);
            frame->writeRaw(out);
        }
    }

    return true;
}
