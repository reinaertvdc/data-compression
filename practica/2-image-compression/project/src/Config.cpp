#include <iostream>
#include "Config.h"

Config::Config() :
        missingValues(),
        rawFilePath(""),
        encodedFilePath(""),
        decodedFilePath(""),
        width(0),
        height(0),
        applyRle(false),
        quantMatrixFilePath(""),
        logFilePath("") {
    this->missingValues.emplace_back("rawfile");
    this->missingValues.emplace_back("encfile");
    this->missingValues.emplace_back("decfile");
    this->missingValues.emplace_back("width");
    this->missingValues.emplace_back("height");
    this->missingValues.emplace_back("rle");
    this->missingValues.emplace_back("quantfile");
    this->missingValues.emplace_back("logfile");
}

Config::Config(ConfigReader &configReader) :
        missingValues(),
        rawFilePath(this->getString(configReader, "rawfile")),
        encodedFilePath(this->getString(configReader, "encfile")),
        decodedFilePath(this->getString(configReader, "decfile")),
        width(this->getInt(configReader, "width")),
        height(this->getInt(configReader, "height")),
        applyRle(this->getBool(configReader, "rle")),
        quantMatrixFilePath(this->getString(configReader, "quantfile")),
        logFilePath(this->getString(configReader, "logfile")) {}

bool Config::getValue(ConfigReader &configReader, const std::string &key, std::string &value) {
    std::string val;
    if (!configReader.getKeyValue(key, val)) {
        this->missingValues.push_back(key);
        value = std::string("");
        return false;
    }
    else {
        value = val;
        return true;
    }
}

bool Config::getBool(ConfigReader &configReader, const std::string &key) {
    std::string value;
    bool valueFound = this->getValue(configReader, key, value);
    if (valueFound) {
        return value == "1" || value == "t" || value == "T" || value == "true" || value == "True";
    }
    else {
        return false;
    }
}

int Config::getInt(ConfigReader &configReader, const std::string &key) {
    std::string value;
    bool valueFound = this->getValue(configReader, key, value);
    if (valueFound) {
        return std::stoi(value);
    }
    else {
        return 0;
    }
}

std::string Config::getString(ConfigReader &configReader, const std::string &key) {
    std::string value;
    this->getValue(configReader, key, value);
    return value;
}

int Config::getMissingKeyCount() const {
    return static_cast<int>(this->missingValues.size());
}

const std::string Config::getMissingKey(int index) const {
    if (index < 0 || index >= this->getMissingKeyCount()) {
        return std::string("");
    }
    return this->missingValues[index];
}

const std::string Config::getMissingKeysAsString() const {
    std::string s;
    bool first = true;
    for (const std::string &key : this->missingValues) {
        if (first) {
            first = false;
        }
        else {
            s += ",";
        }
        s += key;
    }
    return s;
}

Config &Config::operator=(Config && other) {
    this->missingValues = std::vector<std::string>();
    for (std::string key : other.missingValues) {
        this->missingValues.emplace_back(key);
    }
    this->rawFilePath = other.rawFilePath;
    this->encodedFilePath = other.encodedFilePath;
    this->decodedFilePath = other.decodedFilePath;
    this->logFilePath = other.logFilePath;
    this->quantMatrixFilePath = other.quantMatrixFilePath;
    this->width = other.width;
    this->height = other.height;
    this->applyRle = other.applyRle;
    return *this;
}

Config::Config(const Config & other) {
    this->missingValues = std::vector<std::string>();
    for (std::string key : other.missingValues) {
        this->missingValues.emplace_back(key);
    }
    this->rawFilePath = other.rawFilePath;
    this->encodedFilePath = other.encodedFilePath;
    this->decodedFilePath = other.decodedFilePath;
    this->logFilePath = other.logFilePath;
    this->quantMatrixFilePath = other.quantMatrixFilePath;
    this->width = other.width;
    this->height = other.height;
    this->applyRle = other.applyRle;
}
