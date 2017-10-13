#include <string>
#include <iostream>
#include "configreader.h"

int main(int argc, char *const argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect amount of arguments specified!"
                  << std::endl;
        return 1;
    }
    ConfigReader reader = ConfigReader();
    if (!reader.read(argv[1])) {
        std::cout << "Could not read config file <"
                  << argv[1]
                  << ">"
                  << std::endl
                  << reader.getErrorDescription()
                  << std::endl;
        return 1;
    }
    //TODO: make config class
    bool fileComplete = true;
    std::string rawFileName;
    bool keyExistsRawFile = reader.getKeyValue(std::string("rawfile"), rawFileName);
    if (!keyExistsRawFile) {
        std::cout << "config: <rawfile> key missing" << std::endl;
        fileComplete = false;
    }
    std::string encFileName;
    bool keyExistsEncFile = reader.getKeyValue(std::string("encfile"), encFileName);
    if (!keyExistsEncFile) {
        std::cout << "config: <encfile> key missing" << std::endl;
        fileComplete = false;
    }
    std::string decFileName;
    bool keyExistsDecFile = reader.getKeyValue(std::string("decfile"), decFileName);
    if (!keyExistsDecFile) {
        std::cout << "config: <decfile> key missing" << std::endl;
        fileComplete = false;
    }
    std::string widthStr;
    bool keyExistsWidth = reader.getKeyValue(std::string("width"), widthStr);
    if (!keyExistsWidth) {
        std::cout << "config: <width> key missing" << std::endl;
        fileComplete = false;
    }
    std::string heightStr;
    bool keyExistsHeight = reader.getKeyValue(std::string("height"), heightStr);
    if (!keyExistsHeight) {
        std::cout << "config: <height> key missing" << std::endl;
        fileComplete = false;
    }
    std::string rleStr;
    bool keyExistsRle = reader.getKeyValue(std::string("rle"), rleStr);
    if (!keyExistsRle) {
        std::cout << "config: <rle> key missing" << std::endl;
        fileComplete = false;
    }
    std::string quantFileName;
    bool keyExistsQuantFile = reader.getKeyValue(std::string("quantfile"), quantFileName);
    if (!keyExistsQuantFile) {
        std::cout << "config: <quantfile> key missing" << std::endl;
        fileComplete = false;
    }
    std::string logFilename;
    bool keyExistsLogFile = reader.getKeyValue(std::string("logfile"), logFilename);
    if (!keyExistsLogFile) {
        std::cout << "config: <logfile> key missing" << std::endl;
        fileComplete = false;
    }
    if (!fileComplete) {
        return 1;
    }

    return 0;
}
