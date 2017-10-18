
#include "Init.h"
#include "QuantFileParser.h"
#include "RawFileParser.h"
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
    char *configRealPath = realpath(argv[1], NULL);
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

Init::Init(int argc, char *const *argv, bool readQuantFile, bool readRawFile) {
    int step = 0;
    int totalStep = 3;
    this->initialized = true;
    //CONFIG
    if (this->init(argc, argv)) {
        this->outputProgress(++step, totalStep, "OK", "Configuration read, all key values found");
    }
    else {
        this->initialized = false;
        this->outputProgress(++step, totalStep, "FAIL", "Could not read configuration file");
        return;
    }
    //QUANTIZATION matrix
    if (readQuantFile) {
        if (this->initQuantMatrix()) {
            this->outputProgress(++step, totalStep, "OK", "Quantization matrix file read");
        }
        else {
            this->initialized = false;
            this->outputProgress(++step, totalStep, "FAIL", "Quantization matrix file could not be read");
        }
    }
    else {
        this->outputProgress(++step, totalStep, "SKIP", "(Quantization matrix file)");
    }
    //RAW FILE
    if (readRawFile) {
        if (this->initRawFile()) {
            this->outputProgress(++step, totalStep, "OK", "Raw image file read");
        }
        else {
            this->initialized = false;
            this->outputProgress(++step, totalStep, "FAIL", "Raw image file could not be read");
        }
    } else {
        this->outputProgress(++step, totalStep, "SKIP", "(Raw image file)");
    }
    if (this->initialized) {
        std::cout << "Initialization done" << std::endl << std::endl;
    }
    else {
        std::cout << "Initialization failed" << std::endl << std::endl;
    }
}

bool Init::initQuantMatrix() {
    ByteMatrix matrix = QuantFileParser::parseFile(this->conf.getQuantMatrixFilePath());
    return !matrix.isEmpty();
}

bool Init::initRawFile() {
    if (this->conf.getRawFilePath().empty()) return true;
    this->rawImage = RawFileParser::parseFile(this->conf.getRawFilePath(), this->conf.getWidth(),
                                              this->conf.getHeight());
    return this->rawImage != nullptr;
}

void Init::outputProgress(int step, int totalSteps, std::string status, std::string message) {
    std::cout << "[" << step << "/" << totalSteps << " " << status << "] " << message << std::endl;
}

ByteMatrix &Init::getRawImageBlock(int row, int col) {
    return this->rawImage[this->conf.getWidth()/4*row+col];
}
