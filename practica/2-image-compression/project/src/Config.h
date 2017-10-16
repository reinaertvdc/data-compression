#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H

#include <string>
#include <vector>
#include "configreader.h"

/**
 * Configuration class, contains the information read from the config file
 */
class Config {
private:
    /**
     * Contains the values that are missing
     */
    mutable std::vector<std::string> missingValues;
    /**
     * Configuration values
     */
    std::string rawFilePath;
    std::string encodedFilePath;
    std::string decodedFilePath;
    int width;
    int height;
    bool applyRle;
    std::string quantMatrixFilePath;
    std::string logFilePath;
public:
    //TODO
    Config();
    /**
     * Constructor
     * @param configReader the configuration reader to initialize the configuration from
     */
    explicit Config(ConfigReader &configReader);
    /**
     * Get the amount of missing keys in the configuration file
     * @return int amount of missing keys
     */
    int getMissingKeyCount() const;
    /**
     * Get a specific missing key name (if index is out of range, an empty string is returned)
     * @param index the index of the missing key to request the name of
     * @return std::string name of the missing key
     */
    const std::string getMissingKey(int index) const;
    /**
     * Get a comma-seperate string containing all missing keys in the configuration file
     * @return std::string containing all missing keys (comma-seperated)
     */
    const std::string getMissingKeysAsString() const;
    const std::string &getRawFilePath() const { return this->rawFilePath; }
    const std::string &getEncodedFilePath() const { return this->encodedFilePath; }
    const std::string &getDecodedFilePath() const { return this->decodedFilePath; }
    const std::string &getQuantMatrixFilePath() const { return this->quantMatrixFilePath; }
    const std::string &getLogFilePath() const { return this->logFilePath; }
    const int getWidth() const { return this->width; }
    const int getHeight() const { return this->height; }
    const bool getApplyRle() const { return this->applyRle; }
    //TODO
    Config& operator=(Config&&);
    Config(const Config&);
private:
    /**
     * Get a value (as string) from a config reader based on a key
     * If the key does not exist, the value will be an empty string, the key will be added as missing key
     * @param configReader the config reader that should be used
     * @param key the key to be searched for
     * @param value output of the value
     * @return true if value has been found, false if not
     */
    bool getValue(ConfigReader &configReader, const std::string &key, std::string &value);
    /**
     * Get a boolean value from a config reader based on a key
     * If the key does not exist a default value is returned as normal, but a flag will be set in this class
     * @param configReader the config reader that should be used
     * @param key the key to be searched for
     * @return bool value, true or false
     */
    bool getBool(ConfigReader &configReader, const std::string &key);
    /**
     * Get an integer value from a config reader based on a key
     * If the key does not exist a default value is returned as normal, but a flag will be set in this class
     * @param configReader the config reader that should be used
     * @param key the key to be searched for
     * @return int value
     */
    int getInt(ConfigReader &configReader, const std::string &key);
    /**
     * Get a string value from a config reader based on a key
     * If the key does not exist a default value is returned as normal, but a flag will be set in this class
     * @param configReader the config reader that should be used
     * @param key the key to be searched for
     * @return std::string value
     */
    std::string getString(ConfigReader &configReader, const std::string &key);
};


#endif //PROJECT_CONFIG_H
