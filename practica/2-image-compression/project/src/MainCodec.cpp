
#include <iostream>
#include "Init.h"
#include "RawFileParser.h"

int main(int argc, char *const argv[]) {
    std::cout << "CODEC" << std::endl << std::endl;
    Init init(argc, argv, true, true, false);
    if (!init.isInitialized()) {
        return 1;
    }

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////// ENCODE //////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////

    // DCT transform
    for (int i = 0; i < init.getConfig().getHeight() / 4; i++) {
        for (int j = 0; j < init.getConfig().getWidth() / 4; j++) {
            init.getRawImageBlock(i, j).applyDct();
        }
    }

    //TODO: remove temporary save
    RawFileParser::writeFile16bit(init.getConfig().getEncodedFilePath(), init.getRawImage(),
                                  init.getConfig().getWidth(), init.getConfig().getHeight());

    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////
    ////////////////////// DECODE //////////////////////
    ////////////////////////////////////////////////////
    ////////////////////////////////////////////////////

    // inverse DCT transform
    for (int i = 0; i < init.getConfig().getHeight() / 4; i++) {
        for (int j = 0; j < init.getConfig().getWidth() / 4; j++) {
            init.getRawImageBlock(i, j).applyInverseDct();
        }
    }

    //TODO: remove temporary save
    RawFileParser::writeFile8bit(init.getConfig().getDecodedFilePath(), init.getRawImage(), init.getConfig().getWidth(),
                                 init.getConfig().getHeight());

}
