//
//  Zarzadca.hpp
//  06. Projekt - Kompresja
//
//  Created by Krystian Owoc on 03.07.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef Zarzadca_hpp
#define Zarzadca_hpp

#include "Interface.hpp"
#include "BMPLib.hpp"
#include "ImageSegments.hpp"
#include "Centroids.hpp"
#include "GlobalDefines.hpp"

class Zarzadca
{
private:
    BMPImage sourceImage;
    ImageSegments* imageSegments;
    Centroids* centroids;
    std::string imageFilename;
    Interface* interfejs;
    bool hasRunAtLeastOnce=false;
    void calculateDistance(unsigned int segmentNumber, unsigned int centroidNumber, double* result);
    void findClosestCentroidForSegment(unsigned int x);
    void averageClosestsCentroids();
    double calculateQuantizationError();
    double calculatePSNR(BMPImage source, BMPImage result);
public:
    Zarzadca(std::string imageFilename, unsigned int numberOfCentroids, unsigned int segmentSize, Interface* interfejs);
    ~Zarzadca();
    void glownaPetlaKmeans(unsigned int numberOfRetries, unsigned int numberOfIterations); //iloscProb oznacza rozna ilosc generowanych centroidow, a iloscIteracji oznacza maksymalna ilosc iteracji dla jednej proby
    void zapiszDoPliku(std::string imageFilename);
};

#endif /* Zarzadca_hpp */
