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
public:
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
    /**
     * Configuration values
     */
    const std::string rawFilePath;
    const std::string encodedFilePath;
    const std::string decodedFilePath;
    const int width;
    const int height;
    const bool applyRle;
    const std::string quantMatrixFilePath;
    const std::string logFilePath;
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
