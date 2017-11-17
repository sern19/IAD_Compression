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
    std::vector<std::vector<double>> distances; //Uzywane przy szukani najblizszego
    bool hasRunAtLeastOnce=false;
    void calculateDistanceForCentroids(unsigned int centroid1Number, unsigned int centroid2Number, double* result);
    void calculateDistance(unsigned int segmentNumber, unsigned int centroidNumber, double* result);
    void findClosestCentroidForSegment(unsigned int x);
    void sortCentroidsForSegment(unsigned int x);
    void averageClosestsCentroids();
    void resetDistances();
    void searchForClosestCentroidsLoop();
    void calculatePixelForNeuralGas(unsigned int mode, unsigned int numberOfSegment, unsigned int numberOfCentroid, unsigned int x, unsigned int y, double eps, double lambda, double* result);
    void calculatePixelForKohonen(unsigned int mode, unsigned int numberOfSegment, unsigned int numberOfCentroid, unsigned int x, unsigned int y, double eps, double lambda, double squareDistance, double* result);
    std::vector<std::vector<doublePixelRGB>> calculatePixelsRGBForNeuralGas(unsigned int numberOfSegment, unsigned int numberOfCentroid, double eps, double lambda);
    std::vector<std::vector<doublePixelGray>> calculatePixelsGrayForNeuralGas(unsigned int numberOfSegment, unsigned int numberOfCentroid, double eps, double lambda);
    std::vector<std::vector<doublePixelRGB>> calculatePixelsRGBForKohonen(unsigned int numberOfSegment, unsigned int numberOfCentroid, double eps, double lambda);
    std::vector<std::vector<doublePixelGray>> calculatePixelsGrayForKohonen(unsigned int numberOfSegment, unsigned int numberOfCentroid, double eps, double lambda);
    double calculateQuantizationError();
    double calculatePSNR(BMPImage source, BMPImage result);
public:
    Zarzadca(std::string imageFilename, unsigned int numberOfCentroids, unsigned int segmentSize, Interface* interfejs);
    ~Zarzadca();
    void glownaPetlaKmeans(unsigned int numberOfRetries, unsigned int numberOfIterations); //iloscProb oznacza rozna ilosc generowanych centroidow, a iloscIteracji oznacza maksymalna ilosc iteracji dla jednej proby
    void glownaPetlaNeuralGas(unsigned int numberOfIterations, double initialLambda=INITIAL_LAMBDA, double initialEps=INITIAL_EPS);
    void glownaPetlaKohonen(unsigned int numberOfIterations, double initialEps=INITIAL_EPS);
    void zapiszDoPliku(std::string imageFilename);
};

#endif /* Zarzadca_hpp */
