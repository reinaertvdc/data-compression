
#ifndef PROJECT_INIT_H
#define PROJECT_INIT_H


#include "Config.h"
#include "ValueBlock4x4.h"

/**
 * Class handling the initialization of all input values
 * Reads the config file
 */
class Init {
private:
    /**
     * Flag specifying whether or not the initialization has succeeded
     */
    bool initialized;
    /**
     * The config file directory
     */
    std::string confFileDir;
    /**
     * The config file name
     */
    std::string confFileName;
    /**
     * The configuration
     */
    Config conf;

    /**
     * Initialization function (called by constructor)
     * Initializes the configuration
     * @param argc amount of arguments
     * @param argv the values of the arguments
     * @return bool true if initialized correctly, false otherwise
     */
    bool init(int argc, char *const *argv);

public:
    /**
     * Constructor, initializes the configuration
     * @param argc c style main function argc
     * @param argv c style main function argv
     */
    explicit Init(int argc, char *const *argv);

    /**
     * Check whether or not the initialization has succeeded
     * @return bool true if initialized, false otherwise
     */
    bool isInitialized() const { return this->initialized; }

    /**
     * Get the configuration
     * @return Config
     */
    const Config &getConfig() const;

    /**
     * Retrieve the quantization matrix from the file specified in the configuration
     * @return ValueBlock4x4 containing the quantization matrix in int16_t format
     */
    ValueBlock4x4 getQuantMatrix() const;

    /**
     * Retrieve the raw image data from the file specified in the configuration
     * @return An array of ValueBlock4x4 of size (width/4 * height/4). The calling class should delete the allocated memory for the array
     */
    ValueBlock4x4 *getRawImage() const;
    /**
     * Retrieve the raw encoded data (bytes) fromt eh file specified in the configuration
     * @param size out the amount of bytes read from the file
     * @return An array of bytes (uint8_t) of size (size). The calling class should delete the allocated memory of the array
     */
    uint8_t *getEncodedData(int &size) const;
};


#endif //PROJECT_INIT_H
