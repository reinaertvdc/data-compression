#ifndef INC_3_VIDEO_COMPRESSION_CONFIG_H
#define INC_3_VIDEO_COMPRESSION_CONFIG_H


#include <map>

class Config {
public:
    bool isLoaded() const;

protected:
    explicit Config(const std::string &filePath);

    bool getBool(const std::string &key);

    int getInt(const std::string &key);

    std::string getString(const std::string &key);

private:
    bool isLoaded_ = true;

    std::map<std::string, std::string> values;

    static bool readLine(FILE *file, std::string &line);


};


#endif //INC_3_VIDEO_COMPRESSION_CONFIG_H
