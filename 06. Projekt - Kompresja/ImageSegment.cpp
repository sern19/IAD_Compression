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
void ImageSegment::setModifiedPixelsRGB(std::vector<std::vector<doublePixelRGB>> pixels) { modifiedPixelsRGB=pixels; }
void ImageSegment::setModifiedPixelsGray(std::vector<std::vector<doublePixelGray>> pixels) { modifiedPixelsGray=pixels; }

void ImageSegment::updateMinsMaxs()
{
    unsigned int i,j;
    if (imageSegments->getIsRGB())
    {
        std::pair<doublePixelRGB, doublePixelRGB> output;
        if (isModified)
        {
            output.first=modifiedPixelsRGB[0][0];
            output.second=modifiedPixelsRGB[0][0];
            j=1;
            for (i=0;i<modifiedPixelsRGB.size();i++)
            {
                for (;j<modifiedPixelsRGB[i].size();j++)
                {
                    //Mins
                    if (output.first.r>modifiedPixelsRGB[i][j].r) output.first.r=modifiedPixelsRGB[i][j].r;
                    if (output.first.g>modifiedPixelsRGB[i][j].g) output.first.g=modifiedPixelsRGB[i][j].g;
                    if (output.first.b>modifiedPixelsRGB[i][j].b) output.first.b=modifiedPixelsRGB[i][j].b;
                    //Maxs
                    if (output.second.r<modifiedPixelsRGB[i][j].r) output.second.r=modifiedPixelsRGB[i][j].r;
                    if (output.second.g<modifiedPixelsRGB[i][j].g) output.second.g=modifiedPixelsRGB[i][j].g;
                    if (output.second.b<modifiedPixelsRGB[i][j].b) output.second.b=modifiedPixelsRGB[i][j].b;
                }
                j=0;
            }
        }
        else
        {
            output.first=orginalPixelsRGB[0][0];
            output.second=orginalPixelsRGB[0][0];
            j=1;
            for (i=0;i<orginalPixelsRGB.size();i++)
            {
                for (;j<orginalPixelsRGB[i].size();j++)
                {
                    //Mins
                    if (output.first.r>orginalPixelsRGB[i][j].r) output.first.r=orginalPixelsRGB[i][j].r;
                    if (output.first.g>orginalPixelsRGB[i][j].g) output.first.g=orginalPixelsRGB[i][j].g;
                    if (output.first.b>orginalPixelsRGB[i][j].b) output.first.b=orginalPixelsRGB[i][j].b;
                    //Maxs
                    if (output.second.r<orginalPixelsRGB[i][j].r) output.second.r=orginalPixelsRGB[i][j].r;
                    if (output.second.g<orginalPixelsRGB[i][j].g) output.second.g=orginalPixelsRGB[i][j].g;
                    if (output.second.b<orginalPixelsRGB[i][j].b) output.second.b=orginalPixelsRGB[i][j].b;
                }
                j=0;
            }
        }
        minsMaxsRGB=output;
    }
    else
    {
        std::pair<doublePixelGray, doublePixelGray> output;
        if (isModified)
        {
            output.first=modifiedPixelsGray[0][0];
            output.second=modifiedPixelsGray[0][0];
            j=1;
            for (i=0;i<modifiedPixelsGray.size();i++)
            {
                for (;j<modifiedPixelsGray[i].size();j++)
                {
                    //Mins
                    if (output.first.gray>modifiedPixelsGray[i][j].gray) output.first.gray=modifiedPixelsGray[i][j].gray;
                    //Maxs
                    if (output.second.gray<modifiedPixelsGray[i][j].gray) output.second.gray=modifiedPixelsGray[i][j].gray;
                }
                j=0;
            }
        }
        else
        {
            output.first=orginalPixelsGray[0][0];
            output.second=orginalPixelsGray[0][0];
            j=1;
            for (i=0;i<orginalPixelsGray.size();i++)
            {
                for (;j<orginalPixelsGray[i].size();j++)
                {
                    //Mins
                    if (output.first.gray>orginalPixelsGray[i][j].gray) output.first.gray=orginalPixelsGray[i][j].gray;
                    //Maxs
                    if (output.second.gray<orginalPixelsGray[i][j].gray) output.second.gray=orginalPixelsGray[i][j].gray;
                }
                j=0;
            }
        }
        minsMaxsGray=output;
    }
    isMinsMaxsCalculated=true;
}

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
    updateMinsMaxs();
}

void ImageSegment::denomralizePixels()
{
    unsigned int i,j;
    if (isModified)
    {
        if (imageSegments->getIsRGB())
        {
            for (i=0;i<modifiedPixelsRGB.size();i++)
                for (j=0;j<modifiedPixelsRGB[i].size();j++)
                {
                    modifiedPixelsRGB[i][j].r=modifiedPixelsRGB[i][j].r*imageSegments->getDeviationValue(MODE_R, i, j);
                    modifiedPixelsRGB[i][j].g=modifiedPixelsRGB[i][j].g*imageSegments->getDeviationValue(MODE_G, i, j);
                    modifiedPixelsRGB[i][j].b=modifiedPixelsRGB[i][j].b*imageSegments->getDeviationValue(MODE_B, i, j);
                }
        }
        else
        {
            for (i=0;i<modifiedPixelsGray.size();i++)
                for (j=0;j<modifiedPixelsGray[i].size();j++)
                    modifiedPixelsGray[i][j].gray=modifiedPixelsGray[i][j].gray*imageSegments->getDeviationValue(MODE_GRAY, i, j);
        }
    }
}

std::pair<doublePixelRGB, doublePixelRGB> ImageSegment::getMinsMaxsRGB()
{
    if (!isMinsMaxsCalculated) updateMinsMaxs();
    return minsMaxsRGB;
}

std::pair<doublePixelGray, doublePixelGray> ImageSegment::getMinsMaxsGray()
{
    if (!isMinsMaxsCalculated) updateMinsMaxs();
    return minsMaxsGray;
}

unsigned int ImageSegment::getClosestCentroidNumber() { return closestCentroidNumber; }
void ImageSegment::setClosestCentroidNumber(unsigned int number) { closestCentroidNumber=number; }
