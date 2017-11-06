
#include <iostream>
#include <cstring>
#include "Init.h"
#include "RawFileParser.h"
#include "RleCodec.h"
#include "StorageFormatCodec.h"

int main(int argc, char *const argv[]) {
    std::cout << "DECODER" << std::endl << std::endl;
    Init init(argc, argv);
    if (!init.isInitialized()) {
        return 1;
    }

    int compressedSize;
    uint8_t *compressed = init.getEncodedData(compressedSize);
    int encodedSize;
    int w;
    int h;
    bool rle;
    ValueBlock4x4 quant;
    int16_t *encoded = StorageFormatCodec::fromStorageFormat(compressed, compressedSize, encodedSize, w, h, rle, quant);
    ValueBlock4x4 blockList[h / 4][w / 4];

    int indexEncodedImage = 0;
    for (int i = 0; i < h / 4; i++) {
        for (int j = 0; j < w / 4; j++) {
            int tmpInSizeUsed;
            int16_t *zzPattern;
            ValueBlock4x4 block;
            if (rle) {
                zzPattern = RleCodec::rleDecode(&encoded[indexEncodedImage], 16, tmpInSizeUsed);
                if (i == 65 && j == 124) {
                    for (int k = 0; k < 16; k++) {
                        std::cout << zzPattern[k] << "\t";
                    }
                    std::cout << std::endl << std::endl;
                }
                indexEncodedImage += tmpInSizeUsed;
                block = ValueBlock4x4(zzPattern);
                if (i == 65 && j == 124) {
                    for (int r = 0; r < 4; r++) {
                        for (int c = 0; c < 4; c++) {
                            std::cout << block.getValue(r, c) << "\t";
                        }
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;
                }
                delete[] zzPattern;
            } else {
                zzPattern = &encoded[indexEncodedImage];
                indexEncodedImage += 16;
                block = ValueBlock4x4(zzPattern);
            }
            block.deQuantize(init.getQuantMatrix());
            if (i == 65 && j == 124) {
                for (int r = 0; r < 4; r++) {
                    for (int c = 0; c < 4; c++) {
                        std::cout << block.getValue(r, c) << "\t";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
            block.applyInverseDct();
            if (i == 65 && j == 124) {
                for (int r = 0; r < 4; r++) {
                    for (int c = 0; c < 4; c++) {
                        std::cout << block.getValue(r, c) << "\t";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
            blockList[i][j] = block;
        }
    }

    RawFileParser::writeRawImageFile(init.getConfig().getDecodedFilePath(), w, h, &blockList[0][0]);

    delete[] encoded;

}
