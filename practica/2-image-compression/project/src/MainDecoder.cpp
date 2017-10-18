
#include <iostream>
#include "Init.h"
#include "RawFileParser.h"

int main(int argc, char *const argv[]) {
    std::cout << "DECODER" << std::endl << std::endl;
    Init init(argc, argv, true, false, true);
    if (!init.isInitialized()) {
        return 1;
    }

    // inverse DCT transform
    for (int i = 0; i < init.getConfig().getHeight() / 4; i++) {
        for (int j = 0; j < init.getConfig().getWidth() / 4; j++) {
            init.getTmpEncodedImageBlock(i, j).applyInverseDct();
        }
    }

    //TODO: remove temporary save
    RawFileParser::writeFile(init.getConfig().getDecodedFilePath(), init.getTmpEncodedImage(), init.getConfig().getWidth(), init.getConfig().getHeight());
}
