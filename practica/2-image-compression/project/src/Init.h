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
    //TODO: remove tmp field
    ByteMatrix* tmpEncodedImage;
    bool init(int argc, char *const *argv);
    bool initQuantMatrix();
    bool initRawFile();
    void outputProgress(int step, int totalSteps, std::string status, std::string message);
public:
    explicit Init(int argc, char *const *argv, bool readQuantFile, bool readRawFile, bool readEncodedFile);
    bool isInitialized() const { return this->initialized; }
    const Config &getConfig() const;
    const ByteMatrix &getQuantMatrix() const { return this->quantMatrix; }
    ByteMatrix &getRawImageBlock(int row, int col);
    ByteMatrix* getRawImage() { return this->rawImage; }
    //TODO: remove tmp functions
    ByteMatrix &getTmpEncodedImageBlock(int row, int col) { return this->tmpEncodedImage[row*this->conf.getWidth()/4+col]; }
    ByteMatrix* getTmpEncodedImage() { return this->tmpEncodedImage; }


    bool initEncodedFile();
};


#endif //PROJECT_INIT_H
