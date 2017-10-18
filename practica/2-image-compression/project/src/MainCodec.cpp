
#include <iostream>
#include "Init.h"
#include "RawFileParser.h"

int main(int argc, char *const argv[]) {
    std::cout << "CODEC" << std::endl << std::endl;
    Init init(argc, argv, true, true);
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
    RawFileParser::writeFile(init.getConfig().getEncodedFilePath(), init.getRawImage(), init.getConfig().getWidth(), init.getConfig().getHeight());

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
    RawFileParser::writeFile(init.getConfig().getDecodedFilePath(), init.getRawImage(), init.getConfig().getWidth(), init.getConfig().getHeight());

    //TODO: remove testing code
//    unsigned char tmp[4][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
//    ByteMatrix tmp2(tmp);
//    tmp2.applyDct();
//    for (int i = 0; i < 16; i++) {
//        std::cout << static_cast<int>((char)(tmp2.getBytes()[i]));
//        if (i%4 == 3) std::cout << std::endl;
//        else std::cout << "\t";
//    }
//    std::cout << std::endl;
//    tmp2.applyInverseDct();
//    for (int i = 0; i < 16; i++) {
//        std::cout << static_cast<int>((char)(tmp2.getBytes()[i]));
//        if (i%4 == 3) std::cout << "\n";
//        else std::cout << "\t";
//    }
}
