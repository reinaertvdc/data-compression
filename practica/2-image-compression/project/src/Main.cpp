#include <string>
#include <iostream>
#include "configreader.h"
#include "Config.h"
#include "Main.h"

int Main::main(int argc, char *const *argv, bool encode, bool decode) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " CONFIG_FILE" << std::endl;
        return 1;
    }

    std::string configFilePath = argv[1];

    ConfigReader configReader = ConfigReader();

    if (!configReader.read(argv[1])) {
        std::cout << "Could not read config file <"
                  << argv[1]
                  << ">"
                  << std::endl
                  << configReader.getErrorDescription()
                  << std::endl;
        return 1;
    }

    Config config = Config(configReader);

    return 0;
}
