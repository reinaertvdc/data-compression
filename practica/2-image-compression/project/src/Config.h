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
    /**
     * Default contructors
     * Sets all values as missing
     */
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
    /**
     * Getter for the raw file path
     * @return std::string reference to the raw file path
     */
    const std::string &getRawFilePath() const { return this->rawFilePath; }
    /**
     * Getter for the encoded file path
     * @return std::string reference to the encoded file path
     */
    const std::string &getEncodedFilePath() const { return this->encodedFilePath; }
    /**
     * Getter for the decoded file path
     * @return std::string reference to the decoded file path
     */
    const std::string &getDecodedFilePath() const { return this->decodedFilePath; }
    /**
     * Getter for the quantization matrix file path
     * @return std::string reference to the quantization matrix file path
     */
    const std::string &getQuantMatrixFilePath() const { return this->quantMatrixFilePath; }
    /**
     * Getter for the logfile path
     * @return std::string reference to the logfile path
     */
    const std::string &getLogFilePath() const { return this->logFilePath; }
    /**
     * Getter for the width of the image (in pixels)
     * @return int the width of the image in pixels
     */
    const int getWidth() const { return this->width; }
    /**
     * Getter for the height of the image (in pixels)
     * @return int the height of the image in pixels
     */
    const int getHeight() const { return this->height; }
    /**
     *Getter for the boolean that specifies is rle should be used by the encoder or not
     * @return bool true if rle should be used, false otherwise
     */
    const bool getApplyRle() const { return this->applyRle; }
    /**
     * Assignment operator, copies all values to this object (deep copy)
     * @return *this
     */
    Config& operator=(Config&&);
    /**
     * Copies all values to this (new) object (deep copy)
     */
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
