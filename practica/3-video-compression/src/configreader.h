#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <string>
#include <map>


/**
 * Helper class to read config files containing 'key=value' lines.
 * If a function returns 'false', the getErrorDescription() member function provides an error string
 */
class ConfigReader {
public:
    /**
     * Default contructor
     */
    ConfigReader() = default;

    /**
     * Read all 'key=value' config values from a file and store them inside this class
     * @param fileName the name of the config file
     * @return bool true if config has been read correctly, false otherwise
     */
    bool read(const std::string &fileName);

    /**
     * Obtain a value based on a key from the read configuration file (file has to be read in advance)
     * @param key the key to search for
     * @param value out the value that has been found for the key
     * @return bool true if the key exists and a value has been found, false otherwise
     */
    bool getKeyValue(const std::string &key, std::string &value);

    /**
     * Remove all values from the reader
     */
    void clear();

    /**
     * Get the description of the last occurred error in the reader
     * @return std::string the error description
     */
    std::string getErrorDescription() const;

private:
    /**
     * A 'key->value' mapping of the configuration file
     */
    std::map<std::string, std::string> m_keyValues;
    /**
     * The description of the last error that occurred in the reader
     */
    std::string m_errStr;
};

#endif // CONFIGREADER_H

