
#ifndef PROJECT_RAWFILEPARSER_H
#define PROJECT_RAWFILEPARSER_H


#include "ValueBlock4x4.h"
#include <string>

/**
 * Helper class to read and write raw image files and encoded image files as an array of bytes to/from a file
 */
class RawFileParser {
public:
    /**
     * Read a raw image file from a file given a width and a height of the image
     * @param filename the name of the file to read
     * @param width the width of the image (in pixels)
     * @param height the height of the image (in pixels)
     * @return an array of (width/4 * height/4) ValueBlock4x4's containg the image data, the calling class should delete the array
     */
    static ValueBlock4x4 *readRawImageFile(std::string filename, int width, int height);

    /**
     * Read a encoded image file into a byte array
     * @param filename the name of the file to read
     * @param size out the size of the byte array
     * @return uint8_t array of size (size), the calling class should delete the array
     */
    static uint8_t *readEncodedFile(std::string filename, int &size);

    /**
     * Write an array of bytes to a file
     * @param filename the name of the file to write to
     * @param size the size of the array
     * @param data the array of bytes
     * @return bool true if success, false otherwise
     */
    static bool writeEncodedFile(std::string filename, int size, uint8_t *data);

    /**
     * Write an array of ValueBlock4x4 as a raw image file (array of bytes)
     * @param filename the name of the file to write to
     * @param width the width of the image (in pixels)
     * @param height the height of the image (in pixels)
     * @param data the ValueBlock4x4 array of blocks of size (width/4 * height/4)
     * @return bool true if success, false otherwise
     */
    static bool writeRawImageFile(std::string filename, int width, int height, ValueBlock4x4 *data);
};


#endif //PROJECT_RAWFILEPARSER_H
