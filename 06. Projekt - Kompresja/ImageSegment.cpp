//
//  ImageSegment.cpp
//  06. Projekt - Kompresja
//
//  Created by Krystian Owoc on 03.07.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include "ImageSegment.hpp"
#include "ImageSegments.hpp"

ImageSegment::ImageSegment(std::vector<std::vector<pixelRGB>> pixels, ImageSegments* imageSegments): orginalPixelsRGB(pixels), imageSegments(imageSegments) {}
ImageSegment::ImageSegment(std::vector<std::vector<pixelGray>> pixels, ImageSegments* imageSegments): orginalPixelsGray(pixels), imageSegments(imageSegments) {}
ImageSegment::ImageSegment(ImageSegments* imageSegments): imageSegments(imageSegments) {}
std::vector<std::vector<pixelRGB>> ImageSegment::getOrginalPixelsRGB() { return orginalPixelsRGB; }
std::vector<std::vector<pixelGray>> ImageSegment::getOrginalPixelsGray() { return orginalPixelsGray; }
bool ImageSegment::getIsModified() { return isModified; }
std::vector<std::vector<doublePixelRGB>> ImageSegment::getModifiedPixelsRGB() { return modifiedPixelsRGB; }
std::vector<std::vector<doublePixelGray>> ImageSegment::getModifiedPixelsGray() { return modifiedPixelsGray; }

void ImageSegment::nomralizePixels()
{
    unsigned int i,j;
    isModified=true;
    if (imageSegments->getIsRGB())
    {
        //Ustawianie rozmiaru vectora zmodyfikowanych pixeli
        modifiedPixelsRGB.resize(orginalPixelsRGB.size());
        for (i=0;i<modifiedPixelsRGB.size();i++) modifiedPixelsRGB[i].resize(orginalPixelsRGB[i].size());
        
        for (i=0;i<modifiedPixelsRGB.size();i++)
            for (j=0;j<modifiedPixelsRGB[i].size();j++)
            {
                modifiedPixelsRGB[i][j].r=orginalPixelsRGB[i][j].r/imageSegments->getDeviationValue(MODE_R, i, j);
                modifiedPixelsRGB[i][j].g=orginalPixelsRGB[i][j].g/imageSegments->getDeviationValue(MODE_G, i, j);
                modifiedPixelsRGB[i][j].b=orginalPixelsRGB[i][j].b/imageSegments->getDeviationValue(MODE_B, i, j);
            }
    }
    else
    {
        //Ustawianie rozmiaru vectora zmodyfikowanych pixeli
        modifiedPixelsGray.resize(orginalPixelsGray.size());
        for (i=0;i<modifiedPixelsGray.size();i++) modifiedPixelsGray[i].resize(orginalPixelsGray[i].size());
        
        for (i=0;i<modifiedPixelsGray.size();i++)
            for (j=0;j<modifiedPixelsGray[i].size();j++)
                modifiedPixelsGray[i][j].gray=orginalPixelsGray[i][j].gray/imageSegments->getDeviationValue(MODE_GRAY, i, j);
    }
}
