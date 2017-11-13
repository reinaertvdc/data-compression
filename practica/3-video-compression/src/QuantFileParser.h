
#ifndef PROJECT_QUANTFILEPARSER_H
#define PROJECT_QUANTFILEPARSER_H


#include "ValueBlock4x4.h"
#include <string>
#include <vector>

/**
 * Helper class to parse the quantization matrix file
 */
class QuantFileParser {
public:
    /**
     * Parse a quantization matrix file
     * @param filename the name of the file that contains the quantization matrix
     * @return ValueBlock4x4 contains tha quantization matrix, if an error occurs while reading, the ValueBlock4x4 will be empty
     */
    static ValueBlock4x4 parseFile(std::string filename);

private:
    /**
     * Helper function to split a std::string by spaces, does not include the spaces
     * @param line the string to split
     * @return std::vector<std::string> containing the parts of the string that were seperated by spaces
     */
    static std::vector<std::string> splitBySpaces(std::string line);
};


#endif //PROJECT_QUANTFILEPARSER_H
