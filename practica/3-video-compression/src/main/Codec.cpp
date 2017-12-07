#include "Codec.h"
#include "../configs/Config.h"
#include "../helpers/Logger.h"
#include "../configs/EncoderConfig.h"
#include "../configs/DecoderConfig.h"
#include "../helpers/QuantFileParser.h"

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

    for (int i = 0; i < 4; i++) {
        Logger::info("  " + std::to_string(quant.getValue(i, 0)) + "\t" + std::to_string(quant.getValue(i, 1)) + "\t" +
                     std::to_string(quant.getValue(i, 2)) + "\t" + std::to_string(quant.getValue(i, 3)));
    }

    return 0;
}

int Codec::decode(const std::string &configFilePath) {
    DecoderConfig config(configFilePath);
    if (!config.isLoaded()) { return -1; }

    return 0;
}
