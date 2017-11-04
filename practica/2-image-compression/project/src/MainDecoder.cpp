
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
                indexEncodedImage += tmpInSizeUsed;
                block = ValueBlock4x4(zzPattern);
                delete[] zzPattern;
            } else {
                zzPattern = &encoded[indexEncodedImage];
                indexEncodedImage += 16;
                block = ValueBlock4x4(zzPattern);
            }
            block.deQuantize(init.getQuantMatrix());
            block.applyInverseDct();
            blockList[i][j] = block;
        }
    }

    RawFileParser::writeRawImageFile(init.getConfig().getDecodedFilePath(), w, h, &blockList[0][0]);

}
