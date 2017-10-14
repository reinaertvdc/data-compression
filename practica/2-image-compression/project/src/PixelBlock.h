#ifndef PROJECT_PIXELBLOCK_H
#define PROJECT_PIXELBLOCK_H


class PixelBlock {
public:
    static const int WIDTH = 4;
    static const int HEIGHT = WIDTH;

    unsigned const char pixels[WIDTH][HEIGHT] = {};

    PixelBlock();
    virtual ~PixelBlock();
};


#endif //PROJECT_PIXELBLOCK_H
