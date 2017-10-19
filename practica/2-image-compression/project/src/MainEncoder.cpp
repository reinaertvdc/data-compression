
#include <iostream>
#include "Init.h"
#include "RawFileParser.h"

int main(int argc, char *const argv[]) {
    std::cout << "ENCODER" << std::endl << std::endl;
    Init init(argc, argv, true, true, false);
    if (!init.isInitialized()) {
        return 1;
    }

    short zzOutput[init.getConfig().getWidth() * init.getConfig().getHeight()];

    // DCT transform and quantization and zigzag convert
    for (int i = 0; i < init.getConfig().getHeight() / 4; i++) {
        for (int j = 0; j < init.getConfig().getWidth() / 4; j++) {
            init.getRawImageBlock(i, j).applyDct();
            init.getRawImageBlock(i, j).quantize(init.getQuantMatrix());
            init.getRawImageBlock(i, j).zigzag(&zzOutput[(init.getConfig().getWidth() / 4 * i + j)*16]);
        }
    }

    //TODO: remove temporary file write
    RawFileParser::WriteFile16bit(init.getConfig().getEncodedFilePath(), zzOutput,
                                  init.getConfig().getWidth() * init.getConfig().getHeight());

}
