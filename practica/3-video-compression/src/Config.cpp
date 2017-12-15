#include <cstring>
#include "Config.h"
#include "Logger.h"

#define MAX_LINE_LENGTH 16384

Config::Config(const std::string &filePath) {
    FILE *file = fopen(filePath.c_str(), "rt");

    if (file == nullptr) {
        Logger::severe("Could not open configuration file '" + filePath + "'");
        return;
    } else {
        Logger::info("Loading configuration file '" + filePath + "'");
    }

    std::string line;

    while (readLine(file, line)) {

        if (line.length() == 0) {
            continue;
        }

        size_t separatorIndex = line.find('=');

        if (separatorIndex == std::string::npos) {
            Logger::severe("Missing '=' sign: '" + line + "'");
            isLoaded_ = false;
            break;
        }

        std::string key = line.substr(0, separatorIndex);
        std::string value = line.substr(separatorIndex + 1);

        if (key.length() == 0) {
            Logger::severe("Empty key: '" + line + "'");
            isLoaded_ = false;
            break;
        }

        if (values.find(key) != values.end()) {
            Logger::severe("Key found more than once: '" + line + "'");
            isLoaded_ = false;
            break;
        }

        values[key] = value;
    }

    fclose(file);
}

bool Config::readLine(FILE *file, std::string &line) {
    if (file == nullptr) {
        return false;
    }

    char lineBuffer[MAX_LINE_LENGTH];
    size_t lineLength;

    if (fgets(lineBuffer, MAX_LINE_LENGTH - 1, file) == nullptr) {
        return false;
    }

    lineBuffer[MAX_LINE_LENGTH - 1] = 0;
    lineLength = strlen(lineBuffer);

    while (lineLength > 0) {
        char lastChar = lineBuffer[lineLength - 1];

        if (lastChar != '\r' && lastChar != '\n') {
            break;
        }

        lineBuffer[lineLength - 1] = NULL;
        lineLength--;
    }

    line = std::string(lineBuffer);

    return true;
}

bool Config::isLoaded() const {
    return isLoaded_;
}

bool Config::getBool(const std::string &key) {
    std::string s = getString(key);
    return s == "1" || s == "t" || s == "T" || s == "true" || s == "True" || s == "TRUE";
}

int Config::getInt(const std::string &key) {
    return std::stoi(getString(key));
}

std::string Config::getString(const std::string &key) {
    auto iterator = values.find(key);

    return iterator->second;
}
