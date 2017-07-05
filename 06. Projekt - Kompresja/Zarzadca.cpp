//
//  Zarzadca.cpp
//  06. Projekt - Kompresja
//
//  Created by Krystian Owoc on 03.07.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include "Zarzadca.hpp"
#include <limits.h> //UINT_MAX
#include <float.h> //DBL_MAX
#include <math.h>
#include <thread>
#include <stdlib.h> //srand
#include <map> //Sortowanie przy uzyciu multimapy

Zarzadca::Zarzadca(std::string imageFilename, unsigned int numberOfCentroids, unsigned int segmentSize, Interface* interfejs): interfejs(interfejs)
{
    //Wczytywanie obrazka
    interfejs->showText("Próbuję wczytać plik o nazwie ");
    interfejs->showComunicat(imageFilename);
    try
    {
        sourceImage=BMPImage(imageFilename);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        throw std::string("Nie można kontynuować działania aplikacji!");
    }
    
    //Segmentacja obrazka
    interfejs->showText("Próbuję podzielić obrazek na segmenty o wymiarze ");
    interfejs->showText((int)segmentSize);
    interfejs->showText("x");
    interfejs->showText((int)segmentSize);
    interfejs->showComunicat(" pixeli");
    try
    {
    	imageSegments=new ImageSegments(segmentSize, sourceImage);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        throw std::string("Nie można kontynuować działania aplikacji!");
    }
    interfejs->showComunicat("Skończono segmentację obrazka");
    
    //Opcjonalna normalizacja pixeli
    if (CZY_NORMALIZOWAC_PIXELE)
    {
        interfejs->showComunicat("Normalizuję pixele w segmentach");
        imageSegments->normalizeSegments();
        interfejs->showComunicat("Skończono normalizację pixeli w segmentach");
    }
    
    //Jezeli centroidow ma byc wiecej niz segmentow, zmniejszamy te wartosc
    if (numberOfCentroids>imageSegments->getNumberOfSegments()) numberOfCentroids=imageSegments->getNumberOfSegments();
    
    //Tworzenie centroidów
    interfejs->showText("Generuję ");
    interfejs->showText((int)numberOfCentroids);
    interfejs->showComunicat(" centroidów");
    centroids=new Centroids(numberOfCentroids, segmentSize, sourceImage.isRGB(), imageSegments);
    distances.resize(imageSegments->getNumberOfSegments(),std::vector<double>(centroids->getNumberOfCentroids(),DBL_MAX));
    interfejs->showComunicat("");
    interfejs->showComunicat("Program gotowy do pracy");
    interfejs->showComunicat("");
}

Zarzadca::~Zarzadca()
{
    delete imageSegments;
    delete centroids;
}

void Zarzadca::calculateDistanceForCentroids(unsigned int centroid1Number, unsigned int centroid2Number, double* result)
{
    double output=0.0;
    unsigned int i,j;
    if (sourceImage.isRGB())
    {
        for (i=0;i<centroids->getCentroid(centroid1Number)->getPixelsRGB().size();i++)
            for (j=0;j<centroids->getCentroid(centroid1Number)->getPixelsRGB()[i].size();j++)
            {
                output+=pow((centroids->getCentroid(centroid1Number)->getPixelsRGB()[i][j].r - centroids->getCentroid(centroid2Number)->getPixelsRGB()[i][j].r),2);
                output+=pow((centroids->getCentroid(centroid1Number)->getPixelsRGB()[i][j].g - centroids->getCentroid(centroid2Number)->getPixelsRGB()[i][j].g),2);
                output+=pow((centroids->getCentroid(centroid1Number)->getPixelsRGB()[i][j].b - centroids->getCentroid(centroid2Number)->getPixelsRGB()[i][j].b),2);
            }
    }
    else
    {
        for (i=0;i<centroids->getCentroid(centroid1Number)->getPixelsGray().size();i++)
            for (j=0;j<centroids->getCentroid(centroid1Number)->getPixelsGray()[i].size();j++)
                output+=pow((centroids->getCentroid(centroid1Number)->getPixelsGray()[i][j].gray - centroids->getCentroid(centroid2Number)->getPixelsGray()[i][j].gray),2);
    }
}

void Zarzadca::calculateDistance(unsigned int segmentNumber, unsigned int centroidNumber, double* result)
{
    double output=0.0;
    unsigned int i,j;
    if (sourceImage.isRGB())
    {
        if (imageSegments->getSegment(segmentNumber)->getIsModified())
        {
            for (i=0;i<imageSegments->getSegment(segmentNumber)->getModifiedPixelsRGB().size();i++)
                for (j=0;j<imageSegments->getSegment(segmentNumber)->getModifiedPixelsRGB()[i].size();j++)
                {
                    output+=pow((centroids->getCentroid(centroidNumber)->getPixelsRGB()[i][j].r - imageSegments->getSegment(segmentNumber)->getModifiedPixelsRGB()[i][j].r),2);
                    output+=pow((centroids->getCentroid(centroidNumber)->getPixelsRGB()[i][j].g - imageSegments->getSegment(segmentNumber)->getModifiedPixelsRGB()[i][j].g),2);
                    output+=pow((centroids->getCentroid(centroidNumber)->getPixelsRGB()[i][j].b - imageSegments->getSegment(segmentNumber)->getModifiedPixelsRGB()[i][j].b),2);
                }
        }
        else
        {
            for (i=0;i<imageSegments->getSegment(segmentNumber)->getOrginalPixelsRGB().size();i++)
                for (j=0;j<imageSegments->getSegment(segmentNumber)->getOrginalPixelsRGB()[i].size();j++)
                {
                    output+=pow((centroids->getCentroid(centroidNumber)->getPixelsRGB()[i][j].r - imageSegments->getSegment(segmentNumber)->getOrginalPixelsRGB()[i][j].r),2);
                    output+=pow((centroids->getCentroid(centroidNumber)->getPixelsRGB()[i][j].g - imageSegments->getSegment(segmentNumber)->getOrginalPixelsRGB()[i][j].g),2);
                    output+=pow((centroids->getCentroid(centroidNumber)->getPixelsRGB()[i][j].b - imageSegments->getSegment(segmentNumber)->getOrginalPixelsRGB()[i][j].b),2);
                }
        }
    }
    else
    {
        if (imageSegments->getSegment(segmentNumber)->getIsModified())
        {
            for (i=0;i<imageSegments->getSegment(segmentNumber)->getModifiedPixelsGray().size();i++)
                for (j=0;j<imageSegments->getSegment(segmentNumber)->getModifiedPixelsGray()[i].size();j++)
                    output+=pow((centroids->getCentroid(centroidNumber)->getPixelsGray()[i][j].gray - imageSegments->getSegment(segmentNumber)->getModifiedPixelsGray()[i][j].gray),2);
        }
        else
        {
            for (i=0;i<imageSegments->getSegment(segmentNumber)->getOrginalPixelsGray().size();i++)
                for (j=0;j<imageSegments->getSegment(segmentNumber)->getOrginalPixelsGray()[i].size();j++)
                    output+=pow((centroids->getCentroid(centroidNumber)->getPixelsGray()[i][j].gray - imageSegments->getSegment(segmentNumber)->getOrginalPixelsGray()[i][j].gray),2);
        }
    }
    *result=sqrt(output);
}

void Zarzadca::findClosestCentroidForSegment(unsigned int x)
{
    unsigned int i;
    unsigned int closestCentroidNumber=imageSegments->getSegment(x)->getClosestCentroidNumber();
    double closestDistance;
    
    if (closestCentroidNumber==UINT_MAX)
    {
        imageSegments->getSegment(x)->setClosestCentroidNumber(0);
        centroids->getCentroid(0)->incrementClosestForSegments();
        closestCentroidNumber=0;
        calculateDistance(x, 0, &closestDistance);
    }
    else calculateDistance(x, imageSegments->getSegment(x)->getClosestCentroidNumber(), &closestDistance);
    
    //Wyliczanie odleglosci
    for (i=0;i<centroids->getNumberOfCentroids();i++)
    {
        if ((centroids->getCentroid(i)->getJustGenerated())||(distances[x][i]==DBL_MAX))
        {
            calculateDistance(x,i,&distances[x][i]);
        }
    }
    
    //Szukanie najblizszego
    for (i=0;i<centroids->getNumberOfCentroids();i++)
    	if (distances[x][i]<closestDistance)
        {
            closestDistance=distances[x][i];
            closestCentroidNumber=i;
        }
    //Operacje na segmentach i centroidach
    if (imageSegments->getSegment(x)->getClosestCentroidNumber()!=closestCentroidNumber)
    {
        centroids->getCentroid(imageSegments->getSegment(x)->getClosestCentroidNumber())->decrementClosestForSegments(); //Odjecie ze starego centroida
        imageSegments->getSegment(x)->setClosestCentroidNumber(closestCentroidNumber); //Ustawienie nowego najbliższego
        centroids->getCentroid(closestCentroidNumber)->incrementClosestForSegments(); //Dodanie w nowym centroidzie
    }
}

void Zarzadca::sortCentroidsForSegment(unsigned int x)
{
    unsigned int i;
    std::multimap<double,Centroid> tymczasowaMapa;
    std::vector<Centroid> tymczasoweCentroidy;
    
    tymczasoweCentroidy.reserve(centroids->getNumberOfCentroids());
    
    for (i=0;i<centroids->getNumberOfCentroids();i++)
        tymczasowaMapa.insert(std::make_pair(distances[x][i], *centroids->getCentroid(i)));
    for (auto it = tymczasowaMapa.begin(); it != tymczasowaMapa.end(); it++)
        tymczasoweCentroidy.push_back(it->second);
    
    centroids->setCentroids(tymczasoweCentroidy);
    centroids->toggleCentroidsJustGenerated();
}

void Zarzadca::averageClosestsCentroids()
{
    unsigned int i, x, y, centroidNumber;
    if (sourceImage.isRGB())
    {
        std::vector<std::vector<std::vector<doublePixelRGB>>> meanOfPixels(centroids->getNumberOfCentroids(),std::vector<std::vector<doublePixelRGB>>(imageSegments->getPrefferedSize(),std::vector<doublePixelRGB>(imageSegments->getPrefferedSize())));
        
        //Przeszukiwanie segmentow
        for (i=0;i<imageSegments->getNumberOfSegments();i++)
        {
            centroidNumber=imageSegments->getSegment(i)->getClosestCentroidNumber();
            if (imageSegments->getSegment(i)->getIsModified())
            {
                for (x=0;x<imageSegments->getSegment(i)->getModifiedPixelsRGB().size();x++)
                    for (y=0;y<imageSegments->getSegment(i)->getModifiedPixelsRGB()[x].size();y++)
                    {
                        meanOfPixels[centroidNumber][x][y].r+=imageSegments->getSegment(i)->getModifiedPixelsRGB()[x][y].r;
                        meanOfPixels[centroidNumber][x][y].g+=imageSegments->getSegment(i)->getModifiedPixelsRGB()[x][y].g;
                        meanOfPixels[centroidNumber][x][y].b+=imageSegments->getSegment(i)->getModifiedPixelsRGB()[x][y].b;
                    }
            }
            else
            {
                for (x=0;x<imageSegments->getSegment(i)->getOrginalPixelsRGB().size();x++)
                    for (y=0;y<imageSegments->getSegment(i)->getOrginalPixelsRGB()[x].size();y++)
                    {
                        meanOfPixels[centroidNumber][x][y].r+=imageSegments->getSegment(i)->getOrginalPixelsRGB()[x][y].r;
                        meanOfPixels[centroidNumber][x][y].g+=imageSegments->getSegment(i)->getOrginalPixelsRGB()[x][y].g;
                        meanOfPixels[centroidNumber][x][y].b+=imageSegments->getSegment(i)->getOrginalPixelsRGB()[x][y].b;
                    }
            }
        }
        
        unsigned int closestForSegments;
        
        //Podmiana wartosci aktualnej za srednia
        for (i=0;i<centroids->getNumberOfCentroids();i++)
        {
            closestForSegments=centroids->getCentroid(i)->getClosestForSegments();
            if (closestForSegments!=0)
            {
                std::vector<std::vector<doublePixelRGB>> tymczasowePixele(imageSegments->getPrefferedSize(),std::vector<doublePixelRGB>(imageSegments->getPrefferedSize()));
                
                for (x=0;x<imageSegments->getPrefferedSize();x++)
                    for (y=0;y<imageSegments->getPrefferedSize();y++)
                    {
                        tymczasowePixele[x][y].r=meanOfPixels[i][x][y].r/(double)closestForSegments;
                        tymczasowePixele[x][y].g=meanOfPixels[i][x][y].g/(double)closestForSegments;
                        tymczasowePixele[x][y].b=meanOfPixels[i][x][y].b/(double)closestForSegments;
                    }
                centroids->getCentroid(i)->setPixelsRGB(tymczasowePixele);
            }
        }
    }
    else
    {
        std::vector<std::vector<std::vector<doublePixelGray>>> meanOfPixels(centroids->getNumberOfCentroids(),std::vector<std::vector<doublePixelGray>>(imageSegments->getPrefferedSize(),std::vector<doublePixelGray>(imageSegments->getPrefferedSize())));
        
        //Przeszukiwanie segmentow
        for (i=0;i<imageSegments->getNumberOfSegments();i++)
        {
            centroidNumber=imageSegments->getSegment(i)->getClosestCentroidNumber();
            if (imageSegments->getSegment(i)->getIsModified())
            {
                for (x=0;x<imageSegments->getSegment(i)->getModifiedPixelsGray().size();x++)
                    for (y=0;y<imageSegments->getSegment(i)->getModifiedPixelsGray()[x].size();y++)
                        meanOfPixels[centroidNumber][x][y].gray+=imageSegments->getSegment(i)->getModifiedPixelsGray()[x][y].gray;
            }
            else
            {
                for (x=0;x<imageSegments->getSegment(i)->getOrginalPixelsGray().size();x++)
                    for (y=0;y<imageSegments->getSegment(i)->getOrginalPixelsGray()[x].size();y++)
                        meanOfPixels[centroidNumber][x][y].gray+=imageSegments->getSegment(i)->getOrginalPixelsGray()[x][y].gray;
            }
        }
        
        unsigned int closestForSegments;
        
        //Podmiana wartosci aktualnej za srednia
        for (i=0;i<centroids->getNumberOfCentroids();i++)
        {
            closestForSegments=centroids->getCentroid(i)->getClosestForSegments();
            if (closestForSegments!=0)
            {
                std::vector<std::vector<doublePixelGray>> tymczasowePixele(imageSegments->getPrefferedSize(),std::vector<doublePixelGray>(imageSegments->getPrefferedSize()));
                
                for (x=0;x<imageSegments->getPrefferedSize();x++)
                    for (y=0;y<imageSegments->getPrefferedSize();y++)
                        tymczasowePixele[x][y].gray=meanOfPixels[i][x][y].gray/(double)closestForSegments;
                centroids->getCentroid(i)->setPixelsGray(tymczasowePixele);
            }
        }
    }
}

void Zarzadca::resetDistances()
{
    unsigned int i,j;
    for (i=0;i<distances.size();i++)
        for (j=0;j<distances[i].size();j++)
            distances[i][j]=DBL_MAX;
}

void Zarzadca::searchForClosestCentroidsLoop()
{
    unsigned int i,k;
    
    std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA];
    
    for (i=0;i<imageSegments->getNumberOfSegments();i+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
    {
        for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+i)<imageSegments->getNumberOfSegments());k++)
            watki[k]=std::thread(&Zarzadca::findClosestCentroidForSegment,this,i+k);
        for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
            if (watki[k].joinable()) watki[k].join();
    }
    centroids->toggleCentroidsJustGenerated();
}

void Zarzadca::calculatePixelForNeuralGas(unsigned int mode, unsigned int numberOfSegment, unsigned int numberOfCentroid, unsigned int x, unsigned int y, double eps, double lambda, double* result)
{
    if (mode==MODE_R)
    {
		if (imageSegments->getSegment(numberOfSegment)->getIsModified())
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].r+eps*exp(((-1.0)*(double)numberOfCentroid)/lambda)*(imageSegments->getSegment(numberOfSegment)->getModifiedPixelsRGB()[x][y].r-centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].r);
        else
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].r+eps*exp(((-1.0)*(double)numberOfCentroid)/lambda)*(imageSegments->getSegment(numberOfSegment)->getOrginalPixelsRGB()[x][y].r-centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].r);
    }
    else if (mode==MODE_G)
    {
        if (imageSegments->getSegment(numberOfSegment)->getIsModified())
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].g+eps*exp(((-1.0)*(double)numberOfCentroid)/lambda)*(imageSegments->getSegment(numberOfSegment)->getModifiedPixelsRGB()[x][y].g-centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].g);
        else
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].g+eps*exp(((-1.0)*(double)numberOfCentroid)/lambda)*(imageSegments->getSegment(numberOfSegment)->getOrginalPixelsRGB()[x][y].g-centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].g);
    }
    else if (mode==MODE_B)
    {
        if (imageSegments->getSegment(numberOfSegment)->getIsModified())
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].b+eps*exp(((-1.0)*(double)numberOfCentroid)/lambda)*(imageSegments->getSegment(numberOfSegment)->getModifiedPixelsRGB()[x][y].b-centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].b);
        else
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].b+eps*exp(((-1.0)*(double)numberOfCentroid)/lambda)*(imageSegments->getSegment(numberOfSegment)->getOrginalPixelsRGB()[x][y].b-centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].b);
    }
    else if (mode==MODE_GRAY)
    {
        if (imageSegments->getSegment(numberOfSegment)->getIsModified())
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsGray()[x][y].gray+eps*exp(((-1.0)*(double)numberOfCentroid)/lambda)*(imageSegments->getSegment(numberOfSegment)->getModifiedPixelsGray()[x][y].gray-centroids->getCentroid(numberOfCentroid)->getPixelsGray()[x][y].gray);
        else
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsGray()[x][y].gray+eps*exp(((-1.0)*(double)numberOfCentroid)/lambda)*(imageSegments->getSegment(numberOfSegment)->getOrginalPixelsGray()[x][y].gray-centroids->getCentroid(numberOfCentroid)->getPixelsGray()[x][y].gray);
    }
}

void Zarzadca::calculatePixelForKohonen(unsigned int mode, unsigned int numberOfSegment, unsigned int numberOfCentroid, unsigned int x, unsigned int y, double eps, double lambda, double squareDistance, double* result)
{
    if (mode==MODE_R)
    {
        if (imageSegments->getSegment(numberOfSegment)->getIsModified())
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].r+eps*exp(((-1.0)*(double)squareDistance)/(2.0*pow(lambda,2)))*(imageSegments->getSegment(numberOfSegment)->getModifiedPixelsRGB()[x][y].r-centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].r);
        else
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].r+eps*exp(((-1.0)*(double)squareDistance)/(2.0*pow(lambda,2)))*(imageSegments->getSegment(numberOfSegment)->getOrginalPixelsRGB()[x][y].r-centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].r);
    }
    else if (mode==MODE_G)
    {
        if (imageSegments->getSegment(numberOfSegment)->getIsModified())
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].g+eps*exp(((-1.0)*(double)squareDistance)/(2.0*pow(lambda,2)))*(imageSegments->getSegment(numberOfSegment)->getModifiedPixelsRGB()[x][y].g-centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].g);
        else
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].g+eps*exp(((-1.0)*(double)squareDistance)/(2.0*pow(lambda,2)))*(imageSegments->getSegment(numberOfSegment)->getOrginalPixelsRGB()[x][y].g-centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].g);
    }
    else if (mode==MODE_B)
    {
        if (imageSegments->getSegment(numberOfSegment)->getIsModified())
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].b+eps*exp(((-1.0)*(double)squareDistance)/(2.0*pow(lambda,2)))*(imageSegments->getSegment(numberOfSegment)->getModifiedPixelsRGB()[x][y].b-centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].b);
        else
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].b+eps*exp(((-1.0)*(double)squareDistance)/(2.0*pow(lambda,2)))*(imageSegments->getSegment(numberOfSegment)->getOrginalPixelsRGB()[x][y].b-centroids->getCentroid(numberOfCentroid)->getPixelsRGB()[x][y].b);
    }
    else if (mode==MODE_GRAY)
    {
        if (imageSegments->getSegment(numberOfSegment)->getIsModified())
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsGray()[x][y].gray+eps*exp(((-1.0)*(double)squareDistance)/(2.0*pow(lambda,2)))*(imageSegments->getSegment(numberOfSegment)->getModifiedPixelsGray()[x][y].gray-centroids->getCentroid(numberOfCentroid)->getPixelsGray()[x][y].gray);
        else
            *result=centroids->getCentroid(numberOfCentroid)->getPixelsGray()[x][y].gray+eps*exp(((-1.0)*(double)squareDistance)/(2.0*pow(lambda,2)))*(imageSegments->getSegment(numberOfSegment)->getOrginalPixelsGray()[x][y].gray-centroids->getCentroid(numberOfCentroid)->getPixelsGray()[x][y].gray);
    }
}

std::vector<std::vector<doublePixelRGB>> Zarzadca::calculatePixelsRGBForNeuralGas(unsigned int numberOfSegment, unsigned int numberOfCentroid, double eps, double lambda)
{
    std::vector<std::vector<doublePixelRGB>> output(imageSegments->getPrefferedSize(),std::vector<doublePixelRGB>(imageSegments->getPrefferedSize()));
    
    unsigned int i,j,k;
    
    std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA][3];
    
    for (i=0;i<imageSegments->getPrefferedSize();i++)
        for (j=0;j<imageSegments->getPrefferedSize();j+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
        {
            for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+j)<imageSegments->getPrefferedSize());k++)
            {
                watki[k][0]=std::thread(&Zarzadca::calculatePixelForNeuralGas,this,MODE_R,numberOfSegment,numberOfCentroid,i,j+k,eps,lambda,&output[i][k+j].r);
                watki[k][1]=std::thread(&Zarzadca::calculatePixelForNeuralGas,this,MODE_G,numberOfSegment,numberOfCentroid,i,j+k,eps,lambda,&output[i][k+j].g);
                watki[k][2]=std::thread(&Zarzadca::calculatePixelForNeuralGas,this,MODE_B,numberOfSegment,numberOfCentroid,i,j+k,eps,lambda,&output[i][k+j].b);
            }
            for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
            {
                if (watki[k][0].joinable()) watki[k][0].join();
                if (watki[k][1].joinable()) watki[k][1].join();
                if (watki[k][2].joinable()) watki[k][2].join();
            }
        }
    return output;
}

std::vector<std::vector<doublePixelGray>> Zarzadca::calculatePixelsGrayForNeuralGas(unsigned int numberOfSegment, unsigned int numberOfCentroid, double eps, double lambda)
{
    std::vector<std::vector<doublePixelGray>> output(imageSegments->getPrefferedSize(),std::vector<doublePixelGray>(imageSegments->getPrefferedSize()));
    
    unsigned int i,j,k;
    
    std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA];
    
    for (i=0;i<imageSegments->getPrefferedSize();i++)
        for (j=0;j<imageSegments->getPrefferedSize();j+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
        {
            for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+j)<imageSegments->getPrefferedSize());k++)
                watki[k]=std::thread(&Zarzadca::calculatePixelForNeuralGas,this,MODE_GRAY,numberOfSegment,numberOfCentroid,i,j+k,eps,lambda,&output[i][k+j].gray);
            for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
                if (watki[k].joinable()) watki[k].join();
        }
    return output;
}

std::vector<std::vector<doublePixelRGB>> Zarzadca::calculatePixelsRGBForKohonen(unsigned int numberOfSegment, unsigned int numberOfCentroid, double eps, double lambda)
{
    std::vector<std::vector<doublePixelRGB>> output(imageSegments->getPrefferedSize(),std::vector<doublePixelRGB>(imageSegments->getPrefferedSize()));
    
    unsigned int i,j,k;
    double squareDistance;
    
    std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA][3];
    
    calculateDistanceForCentroids(imageSegments->getSegment(numberOfSegment)->getClosestCentroidNumber(), numberOfCentroid, &squareDistance);
    squareDistance*=squareDistance;

    for (i=0;i<imageSegments->getPrefferedSize();i++)
        for (j=0;j<imageSegments->getPrefferedSize();j+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
        {
            for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+j)<imageSegments->getPrefferedSize());k++)
            {
                watki[k][0]=std::thread(&Zarzadca::calculatePixelForKohonen,this,MODE_R,numberOfSegment,numberOfCentroid,i,j+k,eps,lambda,squareDistance,&output[i][k+j].r);
                watki[k][1]=std::thread(&Zarzadca::calculatePixelForKohonen,this,MODE_G,numberOfSegment,numberOfCentroid,i,j+k,eps,lambda,squareDistance,&output[i][k+j].g);
                watki[k][2]=std::thread(&Zarzadca::calculatePixelForKohonen,this,MODE_B,numberOfSegment,numberOfCentroid,i,j+k,eps,lambda,squareDistance,&output[i][k+j].b);
            }
            for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
            {
                if (watki[k][0].joinable()) watki[k][0].join();
                if (watki[k][1].joinable()) watki[k][1].join();
                if (watki[k][2].joinable()) watki[k][2].join();
            }
        }
    return output;
}
std::vector<std::vector<doublePixelGray>> Zarzadca::calculatePixelsGrayForKohonen(unsigned int numberOfSegment, unsigned int numberOfCentroid, double eps, double lambda)
{
    std::vector<std::vector<doublePixelGray>> output(imageSegments->getPrefferedSize(),std::vector<doublePixelGray>(imageSegments->getPrefferedSize()));
    
    unsigned int i,j,k;
    double squareDistance;
    
    std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA];
    
    calculateDistanceForCentroids(imageSegments->getSegment(numberOfSegment)->getClosestCentroidNumber(), numberOfCentroid, &squareDistance);
    squareDistance*=squareDistance;
    
    for (i=0;i<imageSegments->getPrefferedSize();i++)
        for (j=0;j<imageSegments->getPrefferedSize();j+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
        {
            for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+j)<imageSegments->getPrefferedSize());k++)
                watki[k]=std::thread(&Zarzadca::calculatePixelForKohonen,this,MODE_GRAY,numberOfSegment,numberOfCentroid,i,j+k,eps,lambda,squareDistance,&output[i][k+j].gray);
            for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
                if (watki[k].joinable()) watki[k].join();
        }
    return output;
}

double Zarzadca::calculateQuantizationError()
{
    double suma=0.0, result;
    unsigned int i;
    for (i=0;i<imageSegments->getNumberOfSegments();i++)
    {
        calculateDistance(i, imageSegments->getSegment(i)->getClosestCentroidNumber(), &result);
        suma+=result;
    }
    return suma;
}

double Zarzadca::calculatePSNR(BMPImage source, BMPImage result)
{
    double output=0.0;
    unsigned int i,j;
    if ((source.isRGB()!=result.isRGB()) || (source.getHeight()!=result.getHeight()) || (source.getWidth()!=result.getWidth())) return DBL_MAX;
    if (source.isRGB())
    {
        for (i=0;i<source.getHeight();i++)
            for (j=0;j<source.getWidth();j++)
            {
                output+=pow((source.getRGBPixels()[i][j].r-result.getRGBPixels()[i][j].r),2);
                output+=pow((source.getRGBPixels()[i][j].g-result.getRGBPixels()[i][j].g),2);
                output+=pow((source.getRGBPixels()[i][j].b-result.getRGBPixels()[i][j].b),2);
            }
        output/=3*source.getHeight()*source.getWidth();
    }
    else
    {
        for (i=0;i<source.getHeight();i++)
            for (j=0;j<source.getWidth();j++)
                output+=pow((source.getGrayPixels()[i][j].gray-result.getGrayPixels()[i][j].gray),2);
        output/=source.getHeight()*source.getWidth();
    }
    return 10*log10(pow(255,2)/output);
}

void Zarzadca::glownaPetlaKmeans(unsigned int numberOfRetries, unsigned int numberOfIterations)
{
    unsigned int mainLoop,i;
    double currentQuantizationError, minimumQuantizationError=DBL_MAX;
    
    interfejs->showComunicat("Rozpoczynam pętlę k-średnich");
    
    if (hasRunAtLeastOnce)
    {
        unsigned int numberOfCentroids=centroids->getNumberOfCentroids();
        interfejs->showText("\tProgram był już uruchomiony conajmniej raz, generuję ");
        interfejs->showText((int)numberOfCentroids);
        interfejs->showComunicat(" nowych centroidów");
        if (centroids!=nullptr) delete centroids;
        centroids=new Centroids(numberOfCentroids,imageSegments->getPrefferedSize(),sourceImage.isRGB(),imageSegments);
        imageSegments->clearClosestsCentroids();
    }
    Centroids previosuCentroids=*centroids;
    Centroids bestCentroids=*centroids;
    
    
    for (mainLoop=0;mainLoop<numberOfRetries;mainLoop++)
    {
        interfejs->showText("\t\t");
        interfejs->showText((int)mainLoop+1);
        interfejs->showText(": ");
        interfejs->showText(0);
        interfejs->showComunicat("%");
        for (i=0;i<numberOfIterations;i++)
        {

            //Szukanie najblizszych centroidow
            searchForClosestCentroidsLoop();
            
            //Przemieszczanie martwych centroidow
			centroids->regenerateDeadCentroids();
            
            //Zapis aktualnych centroidow
            previosuCentroids=*centroids;
            
            //Usrednianie centroidow dla najblizszych segmentow
            averageClosestsCentroids();
            
            //Sprawdzanie czy poprzednie centroidy sa takie same
            if (previosuCentroids==*centroids)
            {
                interfejs->showText("\t\t");
                interfejs->showText((int)mainLoop+1);
                interfejs->showText(": ");
                interfejs->showText(100);
                interfejs->showComunicat("%");
                break;
            }
            interfejs->showText("\t\t");
            interfejs->showText((int)mainLoop+1);
            interfejs->showText(": ");
            interfejs->showText((double)(i+1)/(double)numberOfIterations*100.0);
            interfejs->showComunicat("%");
        }
        currentQuantizationError=calculateQuantizationError();
        if (currentQuantizationError<minimumQuantizationError)
        {
            interfejs->showText("\tZachowuję najlepsze centroidy o błędzie kwantyzacji ");
            interfejs->showComunicat(currentQuantizationError);
            //Przypisanie najlepszego
            bestCentroids=*centroids;
            minimumQuantizationError=currentQuantizationError;
        }
        
        //Generowanie nowych centroidow
        unsigned int numberOfCentroids=centroids->getNumberOfCentroids();
        if (centroids!=nullptr) delete centroids;
        centroids=new Centroids(numberOfCentroids,imageSegments->getPrefferedSize(),sourceImage.isRGB(),imageSegments);
        imageSegments->clearClosestsCentroids();
        
        interfejs->showText("\tSkończono próbę numer ");
        interfejs->showText((int)mainLoop+1);
        interfejs->showText("\\");
        interfejs->showComunicat((int)numberOfRetries);
    }
    
    *centroids=bestCentroids;
    resetDistances();
    imageSegments->clearClosestsCentroids();
    
    //Szukanie najblizszych centroidow
    searchForClosestCentroidsLoop();
    
    interfejs->showComunicat("Skończono pętlę k-średnich");
    interfejs->showComunicat("");
    hasRunAtLeastOnce=true;
}

void Zarzadca::glownaPetlaNeuralGas(unsigned int numberOfIterations, double initialLambda, double initialEps)
{
    unsigned int mainLoop,i,randomSegmentNumber;
    double lambda,eps,iterationConst=(double)numberOfIterations/log(initialLambda);
    
    
    unsigned int numberOfDeadCentroids=0, previousNumberOfDeadCentroids=UINT_MAX;
    
    interfejs->showComunicat("Rozpoczynam pętlę gazu neuronowego");
    
    if (hasRunAtLeastOnce)
    {
        unsigned int numberOfCentroids=centroids->getNumberOfCentroids();
        interfejs->showText("\tProgram był już uruchomiony conajmniej raz, generuję ");
        interfejs->showText((int)numberOfCentroids);
        interfejs->showComunicat(" nowych centroidów");
        if (centroids!=nullptr) delete centroids;
        centroids=new Centroids(numberOfCentroids,imageSegments->getPrefferedSize(),sourceImage.isRGB(),imageSegments);
        imageSegments->clearClosestsCentroids();
    }
    
    //Poszukiwania najblizszego centroidu i przelosowywanie az do momentu gdy nie będzie martwych centroidow
    interfejs->showComunicat("\tSzukam i ponownie generuję martwe centroidy");
    interfejs->showComunicat("\t\t0%");
    do
    {
        //Szukanie najblizszych centroidow
        searchForClosestCentroidsLoop();
        
        numberOfDeadCentroids=centroids->regenerateDeadCentroids();
        if (previousNumberOfDeadCentroids>numberOfDeadCentroids)
        {
            interfejs->showText("\t\t");
            interfejs->showText(((double)centroids->getNumberOfCentroids()-numberOfDeadCentroids)/(double)centroids->getNumberOfCentroids()*100.0);
            interfejs->showComunicat("%");
            previousNumberOfDeadCentroids=numberOfDeadCentroids;
        }
    } while (numberOfDeadCentroids!=0);
    
    //Srand
    srand((unsigned int)time(NULL));
    
    //Glowna petla
    interfejs->showComunicat("\tRozpoczynam główną pętlę");
    for (mainLoop=0;mainLoop<numberOfIterations;mainLoop++)
    {
        lambda=initialLambda*exp(((-1.0)*(double)mainLoop)/iterationConst);
        eps=initialEps*exp(((-1.0)*(double)mainLoop)/(double)numberOfIterations);
        
        //Losowanie segmentu
        randomSegmentNumber=rand()%imageSegments->getNumberOfSegments();
        
        //Sortowanie wg wylosowanego segmentu
        sortCentroidsForSegment(randomSegmentNumber);
        
        //Zmiana wartosci w centroidach
        for (i=0;i<centroids->getNumberOfCentroids();i++)
        {
            if (sourceImage.isRGB()) centroids->getCentroid(i)->setPixelsRGB(calculatePixelsRGBForNeuralGas(randomSegmentNumber, i, eps, lambda));
        	else centroids->getCentroid(i)->setPixelsGray(calculatePixelsGrayForNeuralGas(randomSegmentNumber, i, eps, lambda));
        }
        
        centroids->clearCentroidsClosestForSegments();
        imageSegments->clearClosestsCentroids();
    }
    
    //Szukanie najblizszych centroidow
    searchForClosestCentroidsLoop();
    
    
    interfejs->showComunicat("Skończono pętlę gazu neuronowego");
    interfejs->showComunicat("");
    hasRunAtLeastOnce=true;
}

void Zarzadca::glownaPetlaKohonen(unsigned int numberOfIterations, double initialEps)
{
    unsigned int mainLoop,i,randomSegmentNumber;
    double initialLambda=imageSegments->getMinsMaxsOfPixels()*0.16;
    double lambda,eps,iterationConst=(double)numberOfIterations/log(initialLambda);
    
    unsigned int numberOfDeadCentroids=0, previousNumberOfDeadCentroids=UINT_MAX;
    
    interfejs->showComunicat("Rozpoczynam pętlę Kohonena");
    
    if (hasRunAtLeastOnce)
    {
        unsigned int numberOfCentroids=centroids->getNumberOfCentroids();
        interfejs->showText("\tProgram był już uruchomiony conajmniej raz, generuję ");
        interfejs->showText((int)numberOfCentroids);
        interfejs->showComunicat(" nowych centroidów");
        if (centroids!=nullptr) delete centroids;
        centroids=new Centroids(numberOfCentroids,imageSegments->getPrefferedSize(),sourceImage.isRGB(),imageSegments);
        imageSegments->clearClosestsCentroids();
    }
    
    //Poszukiwania najblizszego centroidu i przelosowywanie az do momentu gdy nie będzie martwych centroidow
    interfejs->showComunicat("\tSzukam i ponownie generuję martwe centroidy");
    interfejs->showComunicat("\t\t0%");
    do
    {
        //Szukanie najblizszych centroidow
        searchForClosestCentroidsLoop();
        
        numberOfDeadCentroids=centroids->regenerateDeadCentroids();
        if (previousNumberOfDeadCentroids>numberOfDeadCentroids)
        {
            interfejs->showText("\t\t");
            interfejs->showText(((double)centroids->getNumberOfCentroids()-numberOfDeadCentroids)/(double)centroids->getNumberOfCentroids()*100.0);
            interfejs->showComunicat("%");
            previousNumberOfDeadCentroids=numberOfDeadCentroids;
        }
    } while (numberOfDeadCentroids!=0);
    
    //Srand
    srand((unsigned int)time(NULL));
    
    //Glowna petla
    interfejs->showComunicat("\tRozpoczynam główną pętlę");
    for (mainLoop=0;mainLoop<numberOfIterations;mainLoop++)
    {
        lambda=initialLambda*exp(((-1.0)*(double)mainLoop)/iterationConst);
        eps=initialEps*exp(((-1.0)*(double)mainLoop)/(double)numberOfIterations);
        
        //Losowanie segmentu
        randomSegmentNumber=rand()%imageSegments->getNumberOfSegments();
        
        //Sortowanie wg wylosowanego segmentu
        searchForClosestCentroidsLoop();
        
        //Zmiana wartosci w centroidach
        for (i=0;i<centroids->getNumberOfCentroids();i++)
        {
            if (lambda>distances[randomSegmentNumber][i])
            {
                if (sourceImage.isRGB()) centroids->getCentroid(i)->setPixelsRGB(calculatePixelsRGBForKohonen(randomSegmentNumber, i, eps, lambda));
                else centroids->getCentroid(i)->setPixelsGray(calculatePixelsGrayForKohonen(randomSegmentNumber, i, eps, lambda));
            }
        }
        
        centroids->clearCentroidsClosestForSegments();
        imageSegments->clearClosestsCentroids();
    }
    
    //Szukanie najblizszych centroidow
    searchForClosestCentroidsLoop();
    
    
    interfejs->showComunicat("Skończono pętlę Kohonena");
    interfejs->showComunicat("");
    hasRunAtLeastOnce=true;
}

void Zarzadca::zapiszDoPliku(std::string imageFilename)
{
    unsigned i;
    
    interfejs->showComunicat("Przepisuję wartości z najbliższych centroidów do segmentów");
    //Przepisywanie wartosci z najblizszych centroidow do segmentow
    for (i=0;i<imageSegments->getNumberOfSegments();i++)
    {
        if (sourceImage.isRGB()) imageSegments->getSegment(i)->setModifiedPixelsRGB(centroids->getCentroid(imageSegments->getSegment(i)->getClosestCentroidNumber())->getPixelsRGB());
        else imageSegments->getSegment(i)->setModifiedPixelsGray(centroids->getCentroid(imageSegments->getSegment(i)->getClosestCentroidNumber())->getPixelsGray());
    }
    interfejs->showComunicat("Przepisano wartości z najbliższych centroidów do segmentów");
    
    if (CZY_NORMALIZOWAC_PIXELE)
    {
        interfejs->showComunicat("Próbuję zdenormalizować pixele w segmentach");
        try
        {
            imageSegments->denormalizeSegments();
        }
        catch (std::string comunicat)
        {
            interfejs->showComunicat(comunicat);
            throw std::string("Nie można kontynuować działania aplikacji!");
        }
        interfejs->showComunicat("Zdenormalizowano pixele w segmentach");
    }
    
    interfejs->showComunicat("Odtwarzam pixele z segmentów do całego obrazka");
    imageSegments->generateFinalPixels();
    
    BMPImage imageToSave=sourceImage;
    if (imageToSave.isRGB()) imageToSave.setRGBPixels(imageSegments->getPixelsRGB());
    else imageToSave.setGrayPixels(imageSegments->getPixelsGray());
    interfejs->showComunicat("Skończono odtwarzać pixele do obrazka");
    
    if (CZY_WYLICZAC_PSNR)
    {
        interfejs->showText("Obliczone PSNR: ");
        interfejs->showComunicat(calculatePSNR(sourceImage, imageToSave));
    }
    
    interfejs->showText("Próbuję zapisać obrazek do pliku o nazwie ");
    interfejs->showComunicat(imageFilename);
    try
    {
    	imageToSave.saveFile(imageFilename);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        throw std::string("Nie można kontynuować działania aplikacji!");
    }
    interfejs->showComunicat("Zapisano obrazek do pliku");
}
