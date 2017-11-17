//
//  Centroids.cpp
//  06. Projekt - Kompresja
//
//  Created by Krystian Owoc on 03.07.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include "Centroids.hpp"
#include "ImageSegments.hpp"
#include "GlobalDefines.hpp"
#include <thread>
#include <float.h> //DBL_MAX
#include <cstdlib>
#include <random>
#include <time.h>

Centroids::Centroids(unsigned int numberOfCentroids, unsigned int prefferedSize, bool isRGB, ImageSegments* imageSegments): isRGB(isRGB), prefferedSize(prefferedSize)
{
    unsigned int i,k;
    std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA];
    
    std::random_device device;
    std::mt19937 generator(device());
    
    //Inicjalizacja vectoru centroidow
    centroids.reserve(numberOfCentroids);
    for (i=0;i<numberOfCentroids;i++) centroids.push_back(Centroid(this));
    
    //Wyszukiwanie minimum i maksimum w segmentach
    if (isRGB)
    {
        std::thread watkiRGB[3];
        watkiRGB[0]=std::thread(&Centroids::calculateMinsMaxs, this, MODE_R, imageSegments);
        watkiRGB[1]=std::thread(&Centroids::calculateMinsMaxs, this, MODE_G, imageSegments);
        watkiRGB[2]=std::thread(&Centroids::calculateMinsMaxs, this, MODE_B, imageSegments);
        for (k=0;k<3;k++) if (watkiRGB[k].joinable()) watkiRGB[k].join();
    }
    else calculateMinsMaxs(MODE_GRAY, imageSegments);
    
    //Generowanie centroidow w watkach
    for (i=0;i<numberOfCentroids;i+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
    {
        for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+i)<numberOfCentroids);k++)
            watki[k]=std::thread(&Centroids::generateCentroid,this,i+k,generator());
        for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
            if (watki[k].joinable()) watki[k].join();
    }
}

void Centroids::setCentroids(std::vector<Centroid> centroids) { this->centroids=centroids; }

void Centroids::generateCentroid(unsigned int x, unsigned int seed)
{
    unsigned int i,j;
    
    std::mt19937 generator(seed);
    std::uniform_real_distribution<double> realNumbers(0,1.0);
    
    if (isRGB)
    {
        std::vector<std::vector<doublePixelRGB>> tymczasowePixele(prefferedSize,std::vector<doublePixelRGB>(prefferedSize));
        for (i=0;i<prefferedSize;i++)
            for (j=0;j<prefferedSize;j++)
            {
                tymczasowePixele[i][j].r=realNumbers(generator)*(minsMaxsRGB.second.r-minsMaxsRGB.first.r)+minsMaxsRGB.first.r;
                tymczasowePixele[i][j].g=realNumbers(generator)*(minsMaxsRGB.second.g-minsMaxsRGB.first.g)+minsMaxsRGB.first.g;
                tymczasowePixele[i][j].b=realNumbers(generator)*(minsMaxsRGB.second.b-minsMaxsRGB.first.b)+minsMaxsRGB.first.b;
            }
        centroids[x]=Centroid(tymczasowePixele,this);
    }
    else
    {
        std::vector<std::vector<doublePixelGray>> tymczasowePixele(prefferedSize,std::vector<doublePixelGray>(prefferedSize));
        for (i=0;i<prefferedSize;i++)
            for (j=0;j<prefferedSize;j++)
            {
                tymczasowePixele[i][j].gray=realNumbers(generator)*(minsMaxsGray.second.gray-minsMaxsGray.first.gray)+minsMaxsGray.first.gray;
            }
        centroids[x]=Centroid(tymczasowePixele,this);
    }
}

void Centroids::calculateMinsMaxs(unsigned int mode, ImageSegments *imageSegments)
{
    std::pair<double, double> output;
    unsigned int i;
    if (mode==MODE_R)
    {
        unsigned int numberOfSegments=imageSegments->getNumberOfSegments();
        output.first=imageSegments->getSegment(0)->getMinsMaxsRGB().first.r;
        output.second=imageSegments->getSegment(0)->getMinsMaxsRGB().second.r;
        for (i=1;i<numberOfSegments;i++)
        {
            if (output.first>imageSegments->getSegment(i)->getMinsMaxsRGB().first.r) output.first=imageSegments->getSegment(i)->getMinsMaxsRGB().first.r;
            if (output.second<imageSegments->getSegment(i)->getMinsMaxsRGB().second.r) output.second=imageSegments->getSegment(i)->getMinsMaxsRGB().second.r;
        }
        minsMaxsRGB.first.r=output.first;
        minsMaxsRGB.second.r=output.second;
    }
    else if (mode==MODE_G)
    {
        unsigned int numberOfSegments=imageSegments->getNumberOfSegments();
        output.first=imageSegments->getSegment(0)->getMinsMaxsRGB().first.g;
        output.second=imageSegments->getSegment(0)->getMinsMaxsRGB().second.g;
        for (i=1;i<numberOfSegments;i++)
        {
            if (output.first>imageSegments->getSegment(i)->getMinsMaxsRGB().first.g) output.first=imageSegments->getSegment(i)->getMinsMaxsRGB().first.g;
            if (output.second<imageSegments->getSegment(i)->getMinsMaxsRGB().second.g) output.second=imageSegments->getSegment(i)->getMinsMaxsRGB().second.g;
        }
        minsMaxsRGB.first.g=output.first;
        minsMaxsRGB.second.g=output.second;
    }
    else if (mode==MODE_B)
    {
        unsigned int numberOfSegments=imageSegments->getNumberOfSegments();
        output.first=imageSegments->getSegment(0)->getMinsMaxsRGB().first.b;
        output.second=imageSegments->getSegment(0)->getMinsMaxsRGB().second.b;
        for (i=1;i<numberOfSegments;i++)
        {
            if (output.first>imageSegments->getSegment(i)->getMinsMaxsRGB().first.b) output.first=imageSegments->getSegment(i)->getMinsMaxsRGB().first.b;
            if (output.second<imageSegments->getSegment(i)->getMinsMaxsRGB().second.b) output.second=imageSegments->getSegment(i)->getMinsMaxsRGB().second.b;
        }
        minsMaxsRGB.first.b=output.first;
        minsMaxsRGB.second.b=output.second;
    }
    else if (mode==MODE_GRAY)
    {
        unsigned int numberOfSegments=imageSegments->getNumberOfSegments();
        output.first=imageSegments->getSegment(0)->getMinsMaxsGray().first.gray;
        output.second=imageSegments->getSegment(0)->getMinsMaxsGray().second.gray;
        for (i=1;i<numberOfSegments;i++)
        {
            if (output.first>imageSegments->getSegment(i)->getMinsMaxsGray().first.gray) output.first=imageSegments->getSegment(i)->getMinsMaxsGray().first.gray;
            if (output.second<imageSegments->getSegment(i)->getMinsMaxsGray().second.gray) output.second=imageSegments->getSegment(i)->getMinsMaxsGray().second.gray;
        }
        minsMaxsGray.first.gray=output.first;
        minsMaxsGray.second.gray=output.second;
    }
}

unsigned int Centroids::getNumberOfCentroids() { return (unsigned int)centroids.size(); }
Centroid* Centroids::getCentroid(unsigned int x) { return &centroids[x]; }
std::pair<doublePixelRGB,doublePixelRGB> Centroids::getMinsMaxsRGB() { return minsMaxsRGB; }
std::pair<doublePixelGray,doublePixelGray> Centroids::getMinsMaxsGray() { return minsMaxsGray; }

unsigned int Centroids::regenerateDeadCentroids()
{
    unsigned int i,k, output=0;
    
    std::random_device device;
    std::mt19937 generator(device());
    
    std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA];
    
    for (i=0;i<centroids.size();i+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
    {
        for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+i)<centroids.size());k++)
            if (centroids[i+k].getClosestForSegments()==0) watki[k]=std::thread(&Centroids::generateCentroid,this,i+k,generator());
        for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
            if (watki[k].joinable()) watki[k].join();
    }
    
    for (i=0;i<centroids.size();i++) if (centroids[i].getClosestForSegments()==0) output++;
    return output;
}

void Centroids::clearCentroidsClosestForSegments()
{
    unsigned int i;
    for (i=0;i<centroids.size();i++) centroids[i].clearClosestForSegments();
}

void Centroids::toggleCentroidsJustGenerated()
{
    unsigned int i;
    for (i=0;i<centroids.size();i++) centroids[i].toggleJustGenerated();
}

bool operator==(const Centroids& centroids1, const Centroids& centroids2)
{
    unsigned int i;
    if (centroids1.centroids.size()!=centroids2.centroids.size()) return false;
    if (centroids1.prefferedSize!=centroids2.prefferedSize) return false;
    if (centroids1.isRGB!=centroids2.isRGB) return false;
    
        for (i=0;i<centroids1.centroids.size();i++)
            if (!(centroids1.centroids[i]==centroids2.centroids[i])) return false;
    
    return true;
}
