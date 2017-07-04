//
//  BMPAdditionalLib.h
//  05. Projekt - Kompresja
//
//  Created by Krystian Owoc on 24.05.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef BMPAdditionalLib_h
#define BMPAdditionalLib_h

struct BITMAPFILEHEADER
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct BITMAPINFOHEADER
{
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

struct pixelRGB
{
    unsigned char r=0,g=0,b=0;
    pixelRGB() {}
    pixelRGB(unsigned char r, unsigned char g, unsigned char b): r(r), b(b), g(g) {}
};

struct pixelGray
{
    unsigned char gray=0;
    pixelGray() {}
    pixelGray(unsigned char gray): gray(gray) {}
};

struct doublePixelRGB
{
    double r=0,g=0,b=0;
    doublePixelRGB() {}
    doublePixelRGB(double r, double g, double b): r(r), b(b), g(g) {}
    doublePixelRGB& operator=(pixelRGB& input)
    {
        r=input.r;
        g=input.g;
        b=input.g;
        return *this;
    }
};

struct doublePixelGray
{
    double gray=0;
    doublePixelGray() {}
    doublePixelGray(double gray): gray(gray) {}
    doublePixelGray& operator=(pixelGray& input)
    {
        gray=input.gray;
        return *this;
    }
};

#endif /* BMPAdditionalLib_h */
