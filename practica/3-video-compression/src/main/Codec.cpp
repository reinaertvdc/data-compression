#include <fstream>
#include <cstring>
#include <iostream>
#include "Codec.h"
#include "../configs/Config.h"
#include "../helpers/Logger.h"
#include "../configs/EncoderConfig.h"
#include "../configs/DecoderConfig.h"
#include "../helpers/QuantFileParser.h"
#include "../frames/Frame.h"

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

    ValueBlock4x4 quant = QuantFileParser::parseFile(config.quantfile);

    std::ifstream f(config.rawfile, std::ios::binary | std::ios::ate);
    long fileSize = f.tellg();
    f.close();

    auto frameSize = (int)(config.width * config.height * 1.5);
    auto numFrames = (int)(fileSize / frameSize);

    std::ifstream rawFile(config.rawfile, std::ios::binary);
    std::ofstream encFile(config.encfile, std::ios::binary);

    for (int i = 0; i < numFrames; i++) {
        uint8_t inBuffer[frameSize];
        uint8_t outBuffer[frameSize];

        rawFile.read(reinterpret_cast<char *>(inBuffer), frameSize);

        Frame frame(inBuffer, config.width, config.height);

        frame.raw(outBuffer);

        encFile.write(reinterpret_cast<const char *>(outBuffer), frameSize);
    }

    rawFile.close();
    encFile.close();

    return 0;
}

int Codec::decode(const std::string &configFilePath) {
    DecoderConfig config(configFilePath);
    if (!config.isLoaded()) { return -1; }

    return 0;
}
