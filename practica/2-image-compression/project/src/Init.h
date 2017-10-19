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
    ValueBlock4x4 quantMatrix;
    ValueBlock4x4* rawImage;
    //TODO: remove tmp fields
    short* tmpEncodedImage;
    int tmpEncodedImageFileSize;
    bool init(int argc, char *const *argv);
    bool initQuantMatrix();
    bool initRawFile();
    bool initEncodedFile();
    void outputProgress(int step, int totalSteps, std::string status, std::string message);
public:
    explicit Init(int argc, char *const *argv, bool readQuantFile, bool readRawFile, bool readEncodedFile);
    bool isInitialized() const { return this->initialized; }
    const Config &getConfig() const;
    const ValueBlock4x4 &getQuantMatrix() const { return this->quantMatrix; }
    ValueBlock4x4 &getRawImageBlock(int row, int col);
    ValueBlock4x4* getRawImage() { return this->rawImage; }
    //TODO: remove tmp functions
    short* getTmpEncodedImage() { return this->tmpEncodedImage; }
    int getTmpEncodedImageFileSize() { return this->tmpEncodedImageFileSize; }
};


#endif //PROJECT_INIT_H
