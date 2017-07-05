//
//  Centroid.hpp
//  06. Projekt - Kompresja
//
//  Created by Krystian Owoc on 03.07.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef Centroid_hpp
#define Centroid_hpp

#include <vector>
#include "BMPAdditionalLib.h"

class Centroids;

class Centroid
{
private:
    std::vector<std::vector<doublePixelRGB>> pixelsRGB;
    std::vector<std::vector<doublePixelGray>> pixelsGray;
    Centroids* centroids;
    unsigned int isClosestForSegments=0;
    bool justGenerated=true;
public:
    Centroid(std::vector<std::vector<doublePixelRGB>> pixels, Centroids* centroids);
    Centroid(std::vector<std::vector<doublePixelGray>> pixels, Centroids* centroids);
    Centroid(Centroids* centroids);
    std::vector<std::vector<doublePixelRGB>> getPixelsRGB();
    std::vector<std::vector<doublePixelGray>> getPixelsGray();
    void setPixelsRGB(std::vector<std::vector<doublePixelRGB>> pixels);
    void setPixelsGray(std::vector<std::vector<doublePixelGray>> pixels);
    unsigned int getClosestForSegments();
    bool getJustGenerated();
    void toggleJustGenerated();
    void setJustGenerated();
    void incrementClosestForSegments(); //Zwieksza wartosc o 1
    void decrementClosestForSegments(); //Zmniejsza wartosc o 1
    void clearClosestForSegments(); //Zeruje wartosc
    friend bool operator==(const Centroid& centroid1, const Centroid& centroid2);
};

#endif /* Centroid_hpp */
