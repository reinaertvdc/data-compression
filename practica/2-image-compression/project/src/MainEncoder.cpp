
#include <iostream>
#include <cstring>
#include "Init.h"
#include "RawFileParser.h"
#include "RleCodec.h"
#include "StorageFormatCodec.h"
#include "Logger.h"

int main(int argc, char *const argv[]) {
    std::cout << "ENCODER" << std::endl << std::endl;
    Init init(argc, argv);
    if (!init.isInitialized()) {
        return 1;
    }

    Logger::file.open(init.getConfig().getLogFilePath());

    ValueBlock4x4 *raw = init.getRawImage();
    ValueBlock4x4 quant = init.getQuantMatrix();

    int16_t rleOutput[init.getConfig().getWidth() * init.getConfig().getHeight() * (4 * 4 + 1) / (4 * 4)];
    int iRleTmpOut = 0;

    // DCT transform and quantization and zigzag convert and rle encode
    for (int i = 0; i < init.getConfig().getHeight() / 4; i++) {
        for (int j = 0; j < init.getConfig().getWidth() / 4; j++) {
            raw[i * init.getConfig().getWidth() / 4 + j].applyDct();
            raw[i * init.getConfig().getWidth() / 4 + j].quantize(quant);
            int16_t zzOutput[16];
            raw[i * init.getConfig().getWidth() / 4 + j].zigzag(zzOutput);
            if (init.getConfig().getApplyRle()) {
                int len;
                int16_t *rle = RleCodec::rleEncode(zzOutput, 16, len);
                memcpy(&rleOutput[iRleTmpOut], rle, len * sizeof(int16_t));
                iRleTmpOut += len;
            }
            else {
                memcpy(&rleOutput[iRleTmpOut], zzOutput, 16 * sizeof(int16_t));
                iRleTmpOut += 16;
            }
        }
    }

    int size;
    uint8_t *data = StorageFormatCodec::toStorageFormat(rleOutput, iRleTmpOut, size, init.getConfig().getWidth(),
                                                        init.getConfig().getHeight(), init.getConfig().getApplyRle(),
                                                        quant);

    RawFileParser::writeEncodedFile(init.getConfig().getEncodedFilePath(), size, data);

    delete[] data;

//    int tmpSize;
//    int tmpW;
//    int tmpH;
//    bool tmpRle;
//    ValueBlock4x4 tmpQuant;
//    int16_t * test = StorageFormatCodec::fromStorageFormat(data, size, tmpSize, tmpW, tmpH, tmpRle, tmpQuant);
//
//    if (tmpSize != iRleTmpOut) {
//        std::cout << "ERROR " << tmpSize << " != " << iRleTmpOut << std::endl;
//    }
//    else {
//        for (int i = 0; i < tmpSize && i < iRleTmpOut; i++) {
//            if (rleOutput[i] != test[i]) {
//                std::cout << "ERROR" << i << std::endl;
//            }
//            break;
//        }
//    }

    Logger::file.close();
}
