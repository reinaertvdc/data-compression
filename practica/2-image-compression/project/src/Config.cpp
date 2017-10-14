#include <iostream>
#include "Config.h"

Config::Config(ConfigReader &configReader) :
        configReader(configReader),

        rawFilePath(getString("rawfile")),
        encodedFilePath(getString("encfile")),
        decodedFilePath(getString("decfile")),

        width(getInt("width")),
        height(getInt("height")),

        applyRle(getBool("rle")),

        quantMatrixFilePath(getString("quantfile")),
        logFilePath(getString("logfile")) {}

std::string Config::getValue(const std::string &key) {
    std::string value;

    if (!configReader.getKeyValue(key, value)) {
        throw "Key '" + key + "' not found";
    }

    return value;
}

bool Config::getBool(const std::string &key) {
    std::string value = getValue(key);

    return value == "1" ||
           value == "t" || value == "T" ||
           value == "true" || value == "True";
}

int Config::getInt(const std::string &key) {
    return std::stoi(getValue(key));
}

std::string Config::getString(const std::string &key) {
    return getValue(key);
}
