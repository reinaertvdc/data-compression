
#include <iostream>
#include <cstring>
#include "Init.h"
#include "RawFileParser.h"
#include "RleCodec.h"

int main(int argc, char *const argv[]) {
    std::cout << "ENCODER" << std::endl << std::endl;
    Init init(argc, argv, true, true, false);
    if (!init.isInitialized()) {
        return 1;
    }

    short rleTmpOutput[2 * init.getConfig().getWidth() * init.getConfig().getHeight() + 2];
    int iRleTmpOut = 0;
    short zzOutput[16];

    // DCT transform and quantization and zigzag convert and rle encode
    for (int i = 0; i < init.getConfig().getHeight() / 4; i++) {
        for (int j = 0; j < init.getConfig().getWidth() / 4; j++) {
            init.getRawImageBlock(i, j).applyDct();
            init.getRawImageBlock(i, j).quantize(init.getQuantMatrix());
            init.getRawImageBlock(i, j).zigzag(zzOutput);
            int len;
            short* rle = RleCodec::rleEncode(zzOutput, 16, len);
            memcpy(&rleTmpOutput[iRleTmpOut], rle, len*sizeof(short));
            iRleTmpOut += len;
        }
    }

    // Copy rle encoded shorts to smaller array
    auto * rleOutput = new short[iRleTmpOut];
    memcpy(rleOutput, rleTmpOutput, iRleTmpOut*sizeof(short));

    //TODO: remove temporary file write
    RawFileParser::WriteFile16bit(init.getConfig().getEncodedFilePath(), rleOutput, iRleTmpOut);

    delete[] rleOutput;

}
