
#include <iostream>
#include <cstring>
#include "Init.h"
#include "RawFileParser.h"
#include "RleCodec.h"
#include "StorageFormatCodec.h"

int main(int argc, char *const argv[]) {
    std::cout << "ENCODER" << std::endl << std::endl;
    Init init(argc, argv);
    if (!init.isInitialized()) {
        return 1;
    }

    ValueBlock4x4 *raw = init.getRawImage();
    ValueBlock4x4 quant = init.getQuantMatrix();

    int16_t rleOutput[init.getConfig().getWidth() * init.getConfig().getHeight() * (4 * 4 + 1) / (4 * 4)];
    int iRleTmpOut = 0;

    // DCT transform and quantization and zigzag convert and rle encode
    for (int i = 0; i < init.getConfig().getHeight() / 4; i++) {
        for (int j = 0; j < init.getConfig().getWidth() / 4; j++) {
            if (i == 65 && j == 124) {
                for (int r = 0; r < 4; r++) {
                    for (int c = 0; c < 4; c++) {
                        std::cout << raw[i * init.getConfig().getWidth() / 4 + j].getValue(r, c) << "\t";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
            raw[i * init.getConfig().getWidth() / 4 + j].applyDct();
            if (i == 65 && j == 124) {
                for (int r = 0; r < 4; r++) {
                    for (int c = 0; c < 4; c++) {
                        std::cout << raw[i * init.getConfig().getWidth() / 4 + j].getValue(r, c) << "\t";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
            raw[i * init.getConfig().getWidth() / 4 + j].quantize(quant);
            if (i == 65 && j == 124) {
                for (int r = 0; r < 4; r++) {
                    for (int c = 0; c < 4; c++) {
                        std::cout << raw[i * init.getConfig().getWidth() / 4 + j].getValue(r, c) << "\t";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
            int16_t zzOutput[16];
            raw[i * init.getConfig().getWidth() / 4 + j].zigzag(zzOutput);
            if (i == 65 && j == 124) {
                for (int k = 0; k < 16; k++) {
                    std::cout << zzOutput[k] << "\t";
                }
                std::cout << std::endl << std::endl;
            }
            if (init.getConfig().getApplyRle()) {
                int len;
                int16_t *rle;// = RleCodec::rleEncode(zzOutput, 16, len);
                if (i == 65 && j == 124) {
                    rle = RleCodec::rleEncode(zzOutput, 16, len, true);
                }
                else {
                    rle = RleCodec::rleEncode(zzOutput, 16, len, false);
                }
                memcpy(&rleOutput[iRleTmpOut], rle, len * sizeof(int16_t));
                iRleTmpOut += len;
                if (i == 65 && j == 124) {
                    for (int k = 0; k < len; k++) {
                        std::cout << rle[k] << "\t";
                    }
                    std::cout << std::endl << std::endl;
                }
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

}
