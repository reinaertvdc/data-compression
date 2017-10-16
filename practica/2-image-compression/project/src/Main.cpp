#include <string>
#include <iostream>
#include "configreader.h"
#include "Config.h"
#include "Main.h"

int Main::main(int argc, char *const *argv, bool encode, bool decode) {
    // Validate argument count
    if (argc != 2) {
        std::string programPath = std::string(argv[0]);
        std::string programFilename = programPath.substr(programPath.find_last_of('/') + 1, programPath.length());
        std::cerr << "Usage: " << programFilename << " CONFIG_FILE" << std::endl;
        return 1;
    }

    // Read configuration file
    ConfigReader configReader = ConfigReader();
    if (!configReader.read(argv[1])) {
        std::cerr << "Could not read configuration file: " << configReader.getErrorDescription() << std::endl;
        return 1;
    }

    // Load configuration
    Config config(configReader);
    if (config.getMissingKeyCount() > 0) {
        std::cerr << "Invalid configuration! Missing keys: " << config.getMissingKeysAsString() << std::endl;
        return 1;
    }

    if (encode) {
        //TODO: encode
    }

    if (decode) {
        //TODO: decode
    }

    return 0;
}
