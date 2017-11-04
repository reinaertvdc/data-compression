
#include "Init.h"
#include "QuantFileParser.h"
#include "RawFileParser.h"
#include <iostream>
#include <unistd.h>
#include <cstring>

bool Init::init(int argc, char *const *argv) {
    // Validate argument count
    if (argc != 2) {
        std::string programPath = std::string(argv[0]);
        std::string programFilename = programPath.substr(programPath.find_last_of('/') + 1, programPath.length());
        std::cerr << "Usage: " << programFilename << " CONFIG_FILE" << std::endl;
        return false;
    }

    // Read configuration file
    this->confFileDir = std::string(argv[1]).substr(0, std::string(argv[1]).find_last_of('/') + 1);
    char* tmp = realpath(this->confFileDir.c_str(), NULL);
    this->confFileDir = std::string(tmp);
    free(tmp);
    this->confFileName = std::string(argv[1]).substr(std::string(argv[1]).find_last_of('/') + 1, strlen(argv[1]));
    chdir(this->confFileDir.c_str());
    ConfigReader configReader = ConfigReader();
    if (!configReader.read(this->confFileName)) {
        std::cerr << "Could not read configuration file: " << configReader.getErrorDescription() << std::endl;
        return false;
    }

    // Load configuration
    this->conf = Config(configReader);
    if (this->conf.getMissingKeyCount() > 0) {
        std::cerr << "Invalid configuration! Missing keys: " << this->conf.getMissingKeysAsString() << std::endl;
        return false;
    }
    return true;
}

const Config &Init::getConfig() const {
    return this->conf;
}

Init::Init(int argc, char *const *argv) {
    this->initialized = this->init(argc, argv);
}

ValueBlock4x4 Init::getQuantMatrix() const {
    return QuantFileParser::parseFile(this->conf.getQuantMatrixFilePath());
}

ValueBlock4x4 *Init::getRawImage() const {
    return RawFileParser::readRawImageFile(this->conf.getRawFilePath(), this->conf.getWidth(), this->conf.getHeight());
}

uint8_t *Init::getEncodedData(int &size) const {
    int tmpSize;
    uint8_t *data = RawFileParser::readEncodedFile(this->conf.getEncodedFilePath(), tmpSize);
    size = tmpSize;
    return data;
}
