//
// Created by cwout on 16/10/17.
//

#ifndef PROJECT_INIT_H
#define PROJECT_INIT_H


#include "Config.h"
#include "ByteMatrix.h"

class Init {
private:
    bool initialized;
    std::string confFileDir;
    std::string confFileName;
    Config conf;
    ByteMatrix quantMatrix;
    bool init(int argc, char *const *argv);
    bool initQuantMatrix();
public:
    explicit Init(int argc, char *const *argv);
    bool isInitialized() const { return this->initialized; }
    const Config &getConfig() const;
    const ByteMatrix &getQuantMatrix() const { return this->quantMatrix; }
};


#endif //PROJECT_INIT_H
