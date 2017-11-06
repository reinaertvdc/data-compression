
#include <iostream>
#include <cstring>
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

    Config config = init.getConfig();

    if (config.getLogFilePath().length() == 0) {
        Logger::fileLevel = Logger::Level::OFF;
    }

    Logger::config("Configuration:");
    Logger::config("    rawfile = " + config.getRawFilePath());
    Logger::config("    encfile = " + config.getEncodedFilePath());
    Logger::config("    decfile = " + config.getDecodedFilePath());
    Logger::config("      width = " + config.getWidth());
    Logger::config("     height = " + config.getHeight());
    Logger::config("        rle = " + config.getApplyRle());
    Logger::config("  quantfile = " + config.getQuantMatrixFilePath());
    Logger::config("    logfile = " + config.getLogFilePath());

    int compressedSize;
    uint8_t *compressed = init.getEncodedData(compressedSize);
    int encodedSize;
    int w;
    int h;
    bool rle;
    ValueBlock4x4 quant;
    int16_t *encoded = StorageFormatCodec::fromStorageFormat(compressed, compressedSize, encodedSize, w, h, rle, quant);
    ValueBlock4x4 blockList[h / 4][w / 4];

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

    RawFileParser::writeRawImageFile(config.getDecodedFilePath(), w, h, &blockList[0][0]);

    delete[] encoded;

    Logger::info("Decoder finished");

    Logger::file.close();
}
