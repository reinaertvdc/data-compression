
#include "Init.h"
#include "QuantFileParser.h"
#include <iostream>
#include <unistd.h>

bool Init::init(int argc, char *const *argv) {
    // Validate argument count
    if (argc != 2) {
        std::string programPath = std::string(argv[0]);
        std::string programFilename = programPath.substr(programPath.find_last_of('/') + 1, programPath.length());
        std::cerr << "Usage: " << programFilename << " CONFIG_FILE" << std::endl;
        return false;
    }

    // Read configuration file
    char* configRealPath = realpath(argv[1], NULL);
    this->confFileDir = std::string(configRealPath);
    free(configRealPath);
    this->confFileName = this->confFileDir.substr(this->confFileDir.find_last_of('/') + 1, this->confFileDir.length());
    this->confFileDir = this->confFileDir.substr(0, this->confFileDir.find_last_of('/') + 1);
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
    int step = 0;
    int totalStep = 2;
    if (!(this->initialized = this->init(argc, argv))) return;
    std::cout << "[" << ++step << "/" << totalStep << " OK] Configuration read, all key values found" << std::endl;
    if (!(this->initialized = this->initQuantMatrix())) return;
    std::cout << "[" << ++step << "/" << totalStep << " OK] Quantization matrix file read, no major problem detected" << std::endl;
    std::cout << "Initialization done" << std::endl;
}

bool Init::initQuantMatrix() {
    QuantMatrix matrix = QuantFileParser::parseFile(this->conf.getQuantMatrixFilePath());
    return !matrix.isEmpty();
}

