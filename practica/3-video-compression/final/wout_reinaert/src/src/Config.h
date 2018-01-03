#ifndef INC_3_VIDEO_COMPRESSION_CONFIG_H
#define INC_3_VIDEO_COMPRESSION_CONFIG_H

#include <map>

class Config {
public:
    /**
     * Check if the config file has been loaded correctly
     * @return
     */
    bool isLoaded() const;

protected:
    /**
     * Constructor: reads the config file from a given path
     * @param filePath the path from which to read the config file
     */
    explicit Config(const std::string &filePath);

    /**
     * Get a config value as a boolean
     * @param key the key to look for
     * @return the boolean value for the key, if the value is not recognized as an existing boolean, the value will be false
     */
    bool getBool(const std::string &key);

    /**
     * Get a config value as an integer
     * @param key the key to look for
     * @return the integer value for the key, if the value is not recognized as an integer value, the value will be 0
     */
    //TODO: out_of_range expception
    //TODO: invalid_argument exception
    int getInt(const std::string &key);

    /**
     * Get a config string value
     * @param key the key to look for
     * @return the value for the key, if the value is non-existant an empty std::string() will be returned
     */
    std::string getString(const std::string &key);

private:
    /**
     * Keeps track if the config file has been loaded
     */
    bool isLoaded_ = true;

    /**
     * Contains all loaded config values
     */
    std::map<std::string, std::string> values;

    /**
     * Reads a line from a file
     * @param file the file to read the line from
     * @param line out the line that has been read
     * @return true in case of success, false in case of failure
     */
    static bool readLine(FILE *file, std::string &line);


};

#endif //INC_3_VIDEO_COMPRESSION_CONFIG_H
