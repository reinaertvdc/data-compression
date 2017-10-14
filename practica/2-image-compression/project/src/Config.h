#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include <string>
#include "configreader.h"


class Config {
private:
    ConfigReader &configReader;
public:
    explicit Config(ConfigReader &configReader);

    const std::string rawFilePath;
    const std::string encodedFilePath;
    const std::string decodedFilePath;

    const int width;
    const int height;

    const bool applyRle;

    const std::string quantMatrixFilePath;
    const std::string logFilePath;
private:
    std::string getValue(const std::string &key);

    bool getBool(const std::string &key);

    int getInt(const std::string &key);

    std::string getString(const std::string &key);
};


#endif //PROJECT_CONFIG_H
