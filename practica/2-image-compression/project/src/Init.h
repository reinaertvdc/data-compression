//
// Created by cwout on 16/10/17.
//

#ifndef PROJECT_INIT_H
#define PROJECT_INIT_H


#include "Config.h"
#include "ValueBlock4x4.h"

class Init {
private:
    bool initialized;
    std::string confFileDir;
    std::string confFileName;
    Config conf;
    bool init(int argc, char *const *argv);
public:
    explicit Init(int argc, char *const *argv);
    bool isInitialized() const { return this->initialized; }
    const Config &getConfig() const;
    ValueBlock4x4 getQuantMatrix() const;
    ValueBlock4x4* getRawImage() const;
    uint8_t *getEncodedData(int &size) const;
};


#endif //PROJECT_INIT_H
