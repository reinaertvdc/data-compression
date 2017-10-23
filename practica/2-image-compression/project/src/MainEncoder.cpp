
#include <iostream>
#include <cstring>
#include "Init.h"
#include "RawFileParser.h"
#include "RleCodec.h"
#include "StorageFormatCodec.h"

int main(int argc, char *const argv[]) {
    std::cout << "ENCODER" << std::endl << std::endl;
    Init init(argc, argv, true, true, false);
    if (!init.isInitialized()) {
        return 1;
    }

    short rleOutput[init.getConfig().getWidth() * init.getConfig().getHeight() * (4 * 4 + 1) / (4 * 4)];
    int iRleTmpOut = 0;
    short zzOutput[16];

    // DCT transform and quantization and zigzag convert and rle encode
    for (int i = 0; i < init.getConfig().getHeight() / 4; i++) {
        for (int j = 0; j < init.getConfig().getWidth() / 4; j++) {
            init.getRawImageBlock(i, j).applyDct();
            init.getRawImageBlock(i, j).quantize(init.getQuantMatrix());
            init.getRawImageBlock(i, j).zigzag(zzOutput);
            int len;
            short *rle = RleCodec::rleEncode(zzOutput, 16, len);
            memcpy(&rleOutput[iRleTmpOut], rle, len * sizeof(short));
            iRleTmpOut += len;
        }
    }

    int tmp;
    uint8_t* tmp2 = StorageFormatCodec::toStorageFormat(rleOutput, iRleTmpOut, tmp, init.getConfig().getWidth(), init.getConfig().getHeight(),
                                        init.getConfig().getApplyRle(),
                                        const_cast<ValueBlock4x4 &>(init.getQuantMatrix()));
    int tmp3;
    int w;
    int h;
    bool rle;
    ValueBlock4x4 tmpMat;
    StorageFormatCodec::fromStorageFormat(tmp2, tmp, tmp3, w, h, rle, tmpMat);
    std::cout << w << "x" << h << std::endl;
    std::cout << rle << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << tmpMat.getData()[i*4+j] << "\t";
        }
        std::cout << std::endl;
    }

    //TODO: remove temporary file write
    RawFileParser::WriteFile16bit(init.getConfig().getEncodedFilePath(), rleOutput, iRleTmpOut);

}
