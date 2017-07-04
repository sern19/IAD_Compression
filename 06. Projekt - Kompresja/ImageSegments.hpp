//
//  ImageSegments.hpp
//  06. Projekt - Kompresja
//
//  Created by Krystian Owoc on 08.06.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef ImageSegments_hpp
#define ImageSegments_hpp

#include <vector>
#include "GlobalDefines.hpp"
#include "BMPLib.hpp"
#include "BMPAdditionalLib.h"
#include "ImageSegment.hpp"

class ImageSegments
{
private:
    unsigned int prefferedSize;
    unsigned int imageHeight;
    unsigned int imageWidth;
    bool isRGB;
    
    std::vector<std::vector<pixelRGB>> pixelsRGB;
    std::vector<std::vector<pixelGray>> pixelsGray;
    
    //Odchylenie standartowe do normalizacji
    std::vector<std::vector<doublePixelRGB>> standartDeviationRGB;
    std::vector<std::vector<doublePixelGray>> standartDeviationGray;
    
    std::vector<ImageSegment> imageSegments;
    void generateSegment(unsigned int x, unsigned int y); //X,Y startuje od 0
    void updatePixelsWithSegment(unsigned int x, unsigned int y);
    //Uzywane glownie przy wielu watkach
    void calculateMean(unsigned int mode, double* valueToSet, unsigned int x, unsigned int y);
    void calculateStandartDeviation(unsigned int mode, double mean, unsigned int x, unsigned int y);
public:
    ImageSegments(unsigned int prefferedSize, BMPImage image);
    ImageSegment* getSegment(unsigned int x);
    ImageSegment* getSegment(unsigned int x, unsigned int y);
    std::vector<std::vector<pixelRGB>> getPixelsRGB();
    std::vector<std::vector<pixelGray>> getPixelsGray();
    double getDeviationValue(unsigned int mode, unsigned int x, unsigned int y);
    bool getIsRGB();
    unsigned int getNumberOfSegments();
    unsigned int getPrefferedSize();
    void setSegment(unsigned int x, unsigned int y, ImageSegment segment);
    void clearClosestsCentroids();
    void normalizeSegments();
    void denormalizeSegments();
    void generateFinalPixels();
};

#endif /* ImageSegments_hpp */
