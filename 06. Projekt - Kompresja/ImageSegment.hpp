//
//  ImageSegment.hpp
//  06. Projekt - Kompresja
//
//  Created by Krystian Owoc on 03.07.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef ImageSegment_hpp
#define ImageSegment_hpp

#include <vector>
#include "BMPAdditionalLib.h"

class ImageSegments;

class ImageSegment
{
private:
    std::vector<std::vector<pixelRGB>> orginalPixelsRGB;
    std::vector<std::vector<pixelGray>> orginalPixelsGray;
    bool isModified=false;
    std::vector<std::vector<doublePixelRGB>> modifiedPixelsRGB;
    std::vector<std::vector<doublePixelGray>> modifiedPixelsGray;
    ImageSegments* imageSegments;
public:
    ImageSegment(std::vector<std::vector<pixelRGB>> pixels, ImageSegments* imageSegments);
    ImageSegment(std::vector<std::vector<pixelGray>> pixels, ImageSegments* imageSegments);
    ImageSegment(ImageSegments* imageSegments);
    std::vector<std::vector<pixelRGB>> getOrginalPixelsRGB();
    std::vector<std::vector<pixelGray>> getOrginalPixelsGray();
    bool getIsModified();
    std::vector<std::vector<doublePixelRGB>> getModifiedPixelsRGB();
    std::vector<std::vector<doublePixelGray>> getModifiedPixelsGray();
    void nomralizePixels();
};

#endif /* ImageSegment_hpp */
