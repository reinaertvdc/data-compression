
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

    Logger::info("Encoder started");

    const Config &config = init.getConfig();

    if (config.getLogFilePath().length() == 0) {
        Logger::fileLevel = Logger::Level::OFF;
    }

    Logger::config("Configuration:");
    Logger::config("    rawfile = " + config.getRawFilePath());
    Logger::config("    encfile = " + config.getEncodedFilePath());
    Logger::config("      width = " + std::to_string(config.getWidth()));
    Logger::config("     height = " + std::to_string(config.getHeight()));
    Logger::config("        rle = " + (config.getApplyRle() ? std::string("true") : std::string("false")));
    Logger::config("  quantfile = " + config.getQuantMatrixFilePath());
    Logger::config("    logfile = " + config.getLogFilePath());

    ValueBlock4x4 quant = init.getQuantMatrix();

    Logger::info("Quantization matrix:");

    for (int i = 0; i < 4; i++) {
        Logger::info("  " + std::to_string(quant.getValue(i, 0)) + "\t" + std::to_string(quant.getValue(i, 1)) + "\t" +
                     std::to_string(quant.getValue(i, 2)) + "\t" + std::to_string(quant.getValue(i, 3)));
    }

    ValueBlock4x4 *raw = init.getRawImage();

    int16_t rleOutput[init.getConfig().getWidth() * init.getConfig().getHeight() * (4 * 4 + 1) / (4 * 4)];
    int iRleTmpOut = 0;

    Logger::info("Compressing image");

    // DCT transform and quantization and zigzag convert and rle encode
    for (int i = 0; i < init.getConfig().getHeight() / 4; i++) {
        for (int j = 0; j < init.getConfig().getWidth() / 4; j++) {
            raw[i * init.getConfig().getWidth() / 4 + j].applyDct();
            raw[i * init.getConfig().getWidth() / 4 + j].quantize(quant);
            int16_t zzOutput[16];
            raw[i * init.getConfig().getWidth() / 4 + j].zigzag(zzOutput);
            if (init.getConfig().getApplyRle()) {
                int len;
                int16_t *rle = RleCodec::rleEncode(zzOutput, 16, len);
                memcpy(&rleOutput[iRleTmpOut], rle, len * sizeof(int16_t));
                delete[] rle;
                iRleTmpOut += len;
            } else {
                memcpy(&rleOutput[iRleTmpOut], zzOutput, 16 * sizeof(int16_t));
                iRleTmpOut += 16;
            }
        }
    }

    int size;
    uint8_t *data = StorageFormatCodec::toStorageFormat(rleOutput, iRleTmpOut, size, init.getConfig().getWidth(),
                                                        init.getConfig().getHeight(), init.getConfig().getApplyRle(),
                                                        quant);

    Logger::info("Image compressed");

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << (double) size / (config.getWidth() * config.getHeight()) * 100;
    std::string compressionRatio = stream.str();

    Logger::info(compressionRatio + "% of original size, from " +
                 std::to_string((int) round((double) (config.getWidth() * config.getHeight()) / 1000)) + "KB to " +
                 std::to_string((int) round((double) (size / 1000))) + "KB");

    RawFileParser::writeEncodedFile(config.getEncodedFilePath(), size, data);

    Logger::info("Encoder finished");

    Logger::file.close();

    delete[] raw;
    delete[] data;
}
