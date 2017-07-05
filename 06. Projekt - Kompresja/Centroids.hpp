//
//  Centroids.hpp
//  06. Projekt - Kompresja
//
//  Created by Krystian Owoc on 03.07.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef Centroids_hpp
#define Centroids_hpp

#include <vector>
#include "Centroid.hpp"
#include "BMPAdditionalLib.h"

class ImageSegments;

class Centroids
{
private:
    unsigned int prefferedSize;
    std::vector<Centroid> centroids;
    std::pair<doublePixelRGB,doublePixelRGB> minsMaxsRGB;
    std::pair<doublePixelGray,doublePixelGray> minsMaxsGray;
    bool isRGB;
    void generateCentroid(unsigned int x);
    void tryToRegenerateCentroid(unsigned int x);
    void calculateMinsMaxs(unsigned int mode, ImageSegments* imageSegments);
public:
    Centroids(unsigned int numberOfCentroids, unsigned int prefferedSize, bool isRGB, ImageSegments* imageSegments);
    void setCentroids(std::vector<Centroid> centroids);
    unsigned int getNumberOfCentroids();
    Centroid* getCentroid(unsigned int x);
    std::pair<doublePixelRGB,doublePixelRGB> getMinsMaxsRGB();
    std::pair<doublePixelGray,doublePixelGray> getMinsMaxsGray();
    unsigned int regenerateDeadCentroids();
    void clearCentroidsClosestForSegments();
    void toggleCentroidsJustGenerated();
    friend bool operator==(const Centroids& centroids1, const Centroids& centroids2);
};

#endif /* Centroids_hpp */
