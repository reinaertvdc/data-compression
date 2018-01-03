#include <fstream>
#include <cstring>
#include <iostream>
#include "Codec.h"
#include "Config.h"
#include "Logger.h"
#include "EncoderConfig.h"
#include "DecoderConfig.h"
#include "QuantFileParser.h"
#include "VideoCodec.h"
#include "RleCodec.h"
#include "BitStream.h"
#include "CompactingCodec.h"

int Codec::run(Action action, int argc, char *const *argv) {
    if (argc != 2) {
        std::string programPath = std::string(argv[0]);
        std::string programFilename = programPath.substr(programPath.find_last_of('/') + 1, programPath.length());
        Logger::severe("Usage: " + programFilename + " CONFIG_FILE");
        return -1;
    }

    return action == Action::ENCODE ? encode(argv[1]) : decode(argv[1]);
}

int Codec::encode(const std::string &configFilePath) {
    EncoderConfig config(configFilePath);

    if (!config.isLoaded()) { return -1; }

    if (!config.logfile.empty())
        Logger::file.open(config.logfile);

    ValueBlock4x4 quant = QuantFileParser::parseFile(config.quantfile);

    std::ifstream tempRawFile(config.rawfile, std::ios::binary | std::ios::ate);
    long rawSize = tempRawFile.tellg();
    tempRawFile.close();

    std::ifstream rawFile(config.rawfile, std::ios::binary);
    std::ofstream encFile(config.encfile, std::ios::binary);

    VideoCodec().encode(rawFile, rawSize, encFile,
                        (uint16_t) config.width, (uint16_t) config.height,
                        config.rle, quant, (uint16_t) config.gop, (uint16_t) config.merange);

    rawFile.close();
    encFile.close();
    if (Logger::file.is_open())
        Logger::file.close();

    return 0;
}

int Codec::decode(const std::string &configFilePath) {
    DecoderConfig config(configFilePath);

    if (!config.isLoaded()) { return -1; }

    if (!config.logfile.empty())
        Logger::file.open(config.logfile);

    std::ifstream tempEncFile(config.encfile, std::ios::binary | std::ios::ate);
    long encSize = tempEncFile.tellg();
    tempEncFile.close();

    std::ifstream encFile(config.encfile, std::ios::binary);
    std::ofstream decFile(config.decfile, std::ios::binary);

    VideoCodec().decode(encFile, encSize, decFile, config.motioncompensation);

    encFile.close();
    decFile.close();
    if (Logger::file.is_open())
        Logger::file.close();

    return 0;
}
