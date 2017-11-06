
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <tgmath.h>
#include "Init.h"
#include "RawFileParser.h"
#include "RleCodec.h"
#include "StorageFormatCodec.h"
#include "Logger.h"

int main(int argc, char *const argv[]) {
    Init init(argc, argv);

    if (!init.isInitialized()) {
        return 1;
    }

    Logger::file.open(init.getConfig().getLogFilePath());

    Logger::info("Decoder started");

    const Config &config = init.getConfig();

    if (config.getLogFilePath().length() == 0) {
        Logger::fileLevel = Logger::Level::OFF;
    }

    Logger::config("Configuration:");
    Logger::config("  encfile = " + config.getEncodedFilePath());
    Logger::config("  decfile = " + config.getDecodedFilePath());
    Logger::config("  logfile = " + config.getLogFilePath());

    int compressedSize;
    uint8_t *compressed = init.getEncodedData(compressedSize);
    int encodedSize;
    int w;
    int h;
    bool rle;
    ValueBlock4x4 quant;

    Logger::info("Analyzing image data");

    int16_t *encoded = StorageFormatCodec::fromStorageFormat(compressed, compressedSize, encodedSize, w, h, rle, quant);
    ValueBlock4x4 blockList[h / 4][w / 4];

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << (double) compressedSize / (w * h) * 100;
    std::string compressionRatio = stream.str();

    Logger::info("  " + compressionRatio + "% of original size, from " +
                 std::to_string((int) round((double) (w * h) / 1000)) + "KB to " +
                 std::to_string((int) round((double) (compressedSize / 1000))) + "KB");

    Logger::info("  " + std::to_string(w) + "x" + std::to_string(h) + ", " + (rle ? "and" : "not") +
                 " run-length encoded");

    Logger::info("Quantization matrix:");

    for (int i = 0; i < 4; i++) {
        Logger::info("  " + std::to_string(quant.getValue(i, 0)) + "\t" + std::to_string(quant.getValue(i, 1)) + "\t" +
                     std::to_string(quant.getValue(i, 2)) + "\t" + std::to_string(quant.getValue(i, 3)));
    }

    Logger::info("Decompressing image");

    int indexEncodedImage = 0;
    for (int i = 0; i < h / 4; i++) {
        for (int j = 0; j < w / 4; j++) {
            int tmpInSizeUsed;
            int16_t *zzPattern;
            ValueBlock4x4 block;
            if (rle) {
                zzPattern = RleCodec::rleDecode(&encoded[indexEncodedImage], 16, tmpInSizeUsed);
                indexEncodedImage += tmpInSizeUsed;
                block = ValueBlock4x4(zzPattern);
                delete[] zzPattern;
            } else {
                zzPattern = &encoded[indexEncodedImage];
                indexEncodedImage += 16;
                block = ValueBlock4x4(zzPattern);
            }
            block.deQuantize(quant);
            block.applyInverseDct();
            blockList[i][j] = block;
        }
    }

    Logger::info("Image decompressed");

    RawFileParser::writeRawImageFile(config.getDecodedFilePath(), w, h, &blockList[0][0]);

    delete[] encoded;

    Logger::info("Decoder finished");

    Logger::file.close();
}
