//
// Created by cwout on 19/10/17.
//

#ifndef PROJECT_RLEENCODER_H
#define PROJECT_RLEENCODER_H


class RleCodec {
public:
    static short *rleEncode(short* in, int size, int &outSize);
    static short *rleDecode(short* in, int minOutSize, int maxOutSize, int &outSize, int &inSizeUsed);
};


#endif //PROJECT_RLEENCODER_H
