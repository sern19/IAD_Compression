//
//  BMPLib.hpp
//  05. Projekt - Kompresja
//
//  Created by Krystian Owoc on 24.05.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef BMPLib_hpp
#define BMPLib_hpp

#include <vector>
#include <string>
#include "BMPAdditionalLib.h"

class BMPImage
{
private:
    std::string filename;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    int32_t stride=0;
    std::vector<std::vector<pixelRGB>> pixelsRGB;
    std::vector<std::vector<pixelGray>> pixelsGray;
    void loadFile();
public:
    BMPImage();
    BMPImage(std::string filename);
    void openFile(std::string filename);
    void saveFile(std::string filename);
    void reloadFile();
    bool isRGB();
    void setRGBPixel(int x, int y, pixelRGB pixel);
    void setGrayPixel(int x, int y, pixelGray pixel);
    unsigned int getHeight();
    unsigned int getWidth();
    std::vector<std::vector<pixelRGB>> getRGBPixels();
    std::vector<std::vector<pixelGray>> getGrayPixels();
    void setRGBPixels(std::vector<std::vector<pixelRGB>> pixels);
    void setGrayPixels(std::vector<std::vector<pixelGray>> pixels);
};

#endif /* BMPLib_hpp */
