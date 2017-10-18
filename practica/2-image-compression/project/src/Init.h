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
    ByteMatrix* rawImage;
    bool init(int argc, char *const *argv);
    bool initQuantMatrix();
    bool initRawFile();
    void outputProgress(int step, int totalSteps, std::string status, std::string message);
public:
    explicit Init(int argc, char *const *argv, bool readQuantFile, bool readRawFile);
    bool isInitialized() const { return this->initialized; }
    const Config &getConfig() const;
    const ByteMatrix &getQuantMatrix() const { return this->quantMatrix; }
    ByteMatrix &getRawImageBlock(int row, int col);
    ByteMatrix* getRawImage() { return this->rawImage; }
};


#endif //PROJECT_INIT_H
