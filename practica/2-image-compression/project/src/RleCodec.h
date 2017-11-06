
#ifndef PROJECT_RLEENCODER_H
#define PROJECT_RLEENCODER_H


class RleCodec {
public:
    static int16_t *rleEncode(int16_t *in, int size, int &outSize, bool debug);

    static int16_t *rleDecode(int16_t *in, int outSize, int &inSizeUsed);
};


#endif //PROJECT_RLEENCODER_H
