//
//  ImageSegments.cpp
//  06. Projekt - Kompresja
//
//  Created by Krystian Owoc on 08.06.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include "ImageSegments.hpp"
#include <math.h>
#include <thread>
#include <float.h> //DBL_MAX
#include <algorithm> //max

ImageSegments::ImageSegments(unsigned int prefferedSize, BMPImage image)
{
    unsigned int i,j,k;
    this->prefferedSize=prefferedSize;
    imageHeight=image.getHeight();
    imageWidth=image.getWidth();
    isRGB=image.isRGB();
    
    std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA];
    
    if ((image.getHeight()%prefferedSize!=0) || (image.getWidth()%prefferedSize!=0)) throw std::string("Nie można podzielić obrazka na segmenty o wybranym rozmiarze!");
    
    if (isRGB) pixelsRGB=image.getRGBPixels();
    else pixelsGray=image.getGrayPixels();
    
    imageSegments.reserve((imageHeight/prefferedSize)*(imageWidth/prefferedSize));
    for (i=0;i<(imageHeight/prefferedSize)*(imageWidth/prefferedSize);i++) imageSegments.push_back(ImageSegment(this));
    
    for (i=0;i<(imageHeight/prefferedSize);i++)
        for (j=0;j<(imageWidth/prefferedSize);j+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
        {
            for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+j)<(imageWidth/prefferedSize));k++)
                watki[k]=std::thread(&ImageSegments::generateSegment,this,i,j+k);
            for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
                if (watki[k].joinable()) watki[k].join();
        }
}

void ImageSegments::generateSegment(unsigned int x, unsigned int y)
{
    unsigned int i,j;
    if (isRGB)
    {
        std::vector<std::vector<pixelRGB>> tymczadowePixele(prefferedSize,std::vector<pixelRGB>(prefferedSize));
        for (i=0;i<prefferedSize;i++)
        {
            for (j=0;j<prefferedSize;j++)
                tymczadowePixele[i][j]=pixelsRGB[x*prefferedSize+i][y*prefferedSize+j];
        }
        imageSegments[x*(imageWidth/prefferedSize)+y]=ImageSegment(tymczadowePixele, this);
    }
    else
    {
        std::vector<std::vector<pixelGray>> tymczadowePixele(prefferedSize,std::vector<pixelGray>(prefferedSize));
        for (i=0;i<prefferedSize;i++)
        {
            for (j=0;j<prefferedSize;j++)
                tymczadowePixele[i][j]=pixelsGray[x*prefferedSize+i][y*prefferedSize+j];
        }
        imageSegments[x*(imageWidth/prefferedSize)+y]=ImageSegment(tymczadowePixele, this);
    }
}

void ImageSegments::updatePixelsWithSegment(unsigned int x, unsigned int y)
{
    unsigned int i,j;
    if (imageSegments[x*(imageWidth/prefferedSize)+y].getIsModified())
    {
        if (isRGB)
        {
            for (i=0;i<prefferedSize;i++)
            {
                for (j=0;j<prefferedSize;j++)
                {
                    pixelsRGB[x*prefferedSize+i][y*prefferedSize+j].r=round(imageSegments[x*(imageWidth/prefferedSize)+y].getModifiedPixelsRGB()[i][j].r);
                    pixelsRGB[x*prefferedSize+i][y*prefferedSize+j].g=round(imageSegments[x*(imageWidth/prefferedSize)+y].getModifiedPixelsRGB()[i][j].g);
                    pixelsRGB[x*prefferedSize+i][y*prefferedSize+j].b=round(imageSegments[x*(imageWidth/prefferedSize)+y].getModifiedPixelsRGB()[i][j].b);
                }
            }
        }
        else
        {
            for (i=0;i<prefferedSize;i++)
            {
                for (j=0;j<prefferedSize;j++)
                    pixelsGray[x*prefferedSize+i][y*prefferedSize+j].gray=round(imageSegments[x*(imageWidth/prefferedSize)+y].getModifiedPixelsGray()[i][j].gray);
            }
        }
    }
    else
    {
        if (isRGB)
        {
            for (i=0;i<prefferedSize;i++)
            {
                for (j=0;j<prefferedSize;j++)
                {
                    pixelsRGB[x*prefferedSize+i][y*prefferedSize+j].r=imageSegments[x*(imageWidth/prefferedSize)+y].getOrginalPixelsRGB()[i][j].r;
                    pixelsRGB[x*prefferedSize+i][y*prefferedSize+j].g=imageSegments[x*(imageWidth/prefferedSize)+y].getOrginalPixelsRGB()[i][j].g;
                    pixelsRGB[x*prefferedSize+i][y*prefferedSize+j].b=imageSegments[x*(imageWidth/prefferedSize)+y].getOrginalPixelsRGB()[i][j].b;
                }
            }
        }
        else
        {
            for (i=0;i<prefferedSize;i++)
            {
                for (j=0;j<prefferedSize;j++)
                    pixelsGray[x*prefferedSize+i][y*prefferedSize+j].gray=imageSegments[x*(imageWidth/prefferedSize)+y].getOrginalPixelsGray()[i][j].gray;
            }
        }    }
}

void ImageSegments::calculateMean(unsigned int mode, double* valueToSet, unsigned int x, unsigned int y)
{
    int output=0;
    unsigned int i;
    if (mode==MODE_R)
    {
        for (i=0;i<imageSegments.size();i++) output+=imageSegments[i].getOrginalPixelsRGB()[x][y].r;
    } else if (mode==MODE_G)
    {
        for (i=0;i<imageSegments.size();i++) output+=imageSegments[i].getOrginalPixelsRGB()[x][y].g;
    } else if (mode==MODE_B)
    {
        for (i=0;i<imageSegments.size();i++) output+=imageSegments[i].getOrginalPixelsRGB()[x][y].b;
    } else if (mode==MODE_GRAY)
    {
        for (i=0;i<imageSegments.size();i++) output+=imageSegments[i].getOrginalPixelsGray()[x][y].gray;
    }
    *valueToSet=(double)output/(double)imageSegments.size();
}

void ImageSegments::calculateStandartDeviation(unsigned int mode, double mean, unsigned int x, unsigned int y)
{
    double output=0.0;
    unsigned int i;
    if (mode==MODE_R)
    {
        for (i=0;i<imageSegments.size();i++) output+=pow((imageSegments[i].getOrginalPixelsRGB()[x][y].r-mean),2);
        standartDeviationRGB[x][y].r=sqrt(output/(double)imageSegments.size());
    } else if (mode==MODE_G)
    {
        for (i=0;i<imageSegments.size();i++) output+=pow((imageSegments[i].getOrginalPixelsRGB()[x][y].g-mean),2);
        standartDeviationRGB[x][y].g=sqrt(output/(double)imageSegments.size());
    } else if (mode==MODE_B)
    {
        for (i=0;i<imageSegments.size();i++) output+=pow((imageSegments[i].getOrginalPixelsRGB()[x][y].b-mean),2);
        standartDeviationRGB[x][y].b=sqrt(output/(double)imageSegments.size());
    } else if (mode==MODE_GRAY)
    {
        for (i=0;i<imageSegments.size();i++) output+=pow((imageSegments[i].getOrginalPixelsGray()[x][y].gray-mean),2);
        standartDeviationGray[x][y].gray=sqrt(output/(double)imageSegments.size());
    }
}

void ImageSegments::getMinsMaxsOfPixel(unsigned int mode, unsigned int x, unsigned int y, double* output)
{
    unsigned int i;
    std::pair<double,double> result;
    if (mode==MODE_R)
    {
        if (imageSegments[0].getIsModified())
        {
            result.first=imageSegments[0].getModifiedPixelsRGB()[x][y].r;
            result.second=imageSegments[0].getModifiedPixelsRGB()[x][y].r;
        }
        else
        {
            result.first=imageSegments[0].getOrginalPixelsRGB()[x][y].r;
            result.second=imageSegments[0].getOrginalPixelsRGB()[x][y].r;
        }
        for (i=1;i<imageSegments.size();i++)
        {
            if (imageSegments[0].getIsModified())
            {
                if (result.first>imageSegments[i].getModifiedPixelsRGB()[x][y].r) result.first=imageSegments[i].getModifiedPixelsRGB()[x][y].r;
                if (result.second<imageSegments[i].getModifiedPixelsRGB()[x][y].r) result.second=imageSegments[i].getModifiedPixelsRGB()[x][y].r;
            }
            else
            {
                if (result.first>imageSegments[i].getOrginalPixelsRGB()[x][y].r) result.first=imageSegments[i].getOrginalPixelsRGB()[x][y].r;
                if (result.second<imageSegments[i].getOrginalPixelsRGB()[x][y].r) result.second=imageSegments[i].getOrginalPixelsRGB()[x][y].r;
            }
        }
            
    } else if (mode==MODE_G)
    {
        if (imageSegments[0].getIsModified())
        {
            result.first=imageSegments[0].getModifiedPixelsRGB()[x][y].g;
            result.second=imageSegments[0].getModifiedPixelsRGB()[x][y].g;
        }
        else
        {
            result.first=imageSegments[0].getOrginalPixelsRGB()[x][y].g;
            result.second=imageSegments[0].getOrginalPixelsRGB()[x][y].g;
        }
        for (i=1;i<imageSegments.size();i++)
        {
            if (imageSegments[0].getIsModified())
            {
                if (result.first>imageSegments[i].getModifiedPixelsRGB()[x][y].g) result.first=imageSegments[i].getModifiedPixelsRGB()[x][y].g;
                if (result.second<imageSegments[i].getModifiedPixelsRGB()[x][y].g) result.second=imageSegments[i].getModifiedPixelsRGB()[x][y].g;
            }
            else
            {
                if (result.first>imageSegments[i].getOrginalPixelsRGB()[x][y].g) result.first=imageSegments[i].getOrginalPixelsRGB()[x][y].g;
                if (result.second<imageSegments[i].getOrginalPixelsRGB()[x][y].g) result.second=imageSegments[i].getOrginalPixelsRGB()[x][y].g;
            }
        }
    } else if (mode==MODE_B)
    {
        if (imageSegments[0].getIsModified())
        {
            result.first=imageSegments[0].getModifiedPixelsRGB()[x][y].b;
            result.second=imageSegments[0].getModifiedPixelsRGB()[x][y].b;
        }
        else
        {
            result.first=imageSegments[0].getOrginalPixelsRGB()[x][y].b;
            result.second=imageSegments[0].getOrginalPixelsRGB()[x][y].b;
        }
        for (i=1;i<imageSegments.size();i++)
        {
            if (imageSegments[0].getIsModified())
            {
                if (result.first>imageSegments[i].getModifiedPixelsRGB()[x][y].b) result.first=imageSegments[i].getModifiedPixelsRGB()[x][y].b;
                if (result.second<imageSegments[i].getModifiedPixelsRGB()[x][y].b) result.second=imageSegments[i].getModifiedPixelsRGB()[x][y].b;
            }
            else
            {
                if (result.first>imageSegments[i].getOrginalPixelsRGB()[x][y].b) result.first=imageSegments[i].getOrginalPixelsRGB()[x][y].b;
                if (result.second<imageSegments[i].getOrginalPixelsRGB()[x][y].b) result.second=imageSegments[i].getOrginalPixelsRGB()[x][y].b;
            }
        }
    } else if (mode==MODE_GRAY)
    {
        if (imageSegments[0].getIsModified())
        {
            result.first=imageSegments[0].getModifiedPixelsGray()[x][y].gray;
            result.second=imageSegments[0].getModifiedPixelsGray()[x][y].gray;
        }
        else
        {
            result.first=imageSegments[0].getOrginalPixelsGray()[x][y].gray;
            result.second=imageSegments[0].getOrginalPixelsGray()[x][y].gray;
        }
        for (i=1;i<imageSegments.size();i++)
        {
            if (imageSegments[0].getIsModified())
            {
                if (result.first>imageSegments[i].getModifiedPixelsGray()[x][y].gray) result.first=imageSegments[i].getModifiedPixelsGray()[x][y].gray;
                if (result.second<imageSegments[i].getModifiedPixelsGray()[x][y].gray) result.second=imageSegments[i].getModifiedPixelsGray()[x][y].gray;
            }
            else
            {
                if (result.first>imageSegments[i].getOrginalPixelsGray()[x][y].gray) result.first=imageSegments[i].getOrginalPixelsGray()[x][y].gray;
                if (result.second<imageSegments[i].getOrginalPixelsGray()[x][y].gray) result.second=imageSegments[i].getOrginalPixelsGray()[x][y].gray;
            }
        }
    }
    *output=(result.second-result.first)/2.0;
}

ImageSegment* ImageSegments::getSegment(unsigned int x) { return &imageSegments[x]; }
ImageSegment* ImageSegments::getSegment(unsigned int x, unsigned int y) { return &imageSegments[x*(imageWidth/prefferedSize)+y]; }
std::vector<std::vector<pixelRGB>> ImageSegments::getPixelsRGB() { return pixelsRGB; }
std::vector<std::vector<pixelGray>> ImageSegments::getPixelsGray() { return pixelsGray; }

double ImageSegments::getMinsMaxsOfPixels()
{
    double output;
    unsigned int i,j,k;
    if (isRGB)
    {
        std::vector<double> tymczasowePary(3*prefferedSize*prefferedSize,DBL_MAX);
        std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA][3];
        for (i=0;i<prefferedSize;i++)
            for (j=0;j<prefferedSize;j+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
            {
                for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+j)<prefferedSize);k++)
                {
                    watki[k][0]=std::thread(&ImageSegments::getMinsMaxsOfPixel,this,MODE_R,i,j+k,&tymczasowePary[i*prefferedSize+j+k]);
                    watki[k][1]=std::thread(&ImageSegments::getMinsMaxsOfPixel,this,MODE_G,i,j+k,&tymczasowePary[i*prefferedSize+j+k+1*(prefferedSize*prefferedSize)]);
                    watki[k][2]=std::thread(&ImageSegments::getMinsMaxsOfPixel,this,MODE_B,i,j+k,&tymczasowePary[i*prefferedSize+j+k+2*(prefferedSize*prefferedSize)]);
                }
                for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
                {
                    if (watki[k][0].joinable()) watki[k][0].join();
                    if (watki[k][1].joinable()) watki[k][1].join();
                    if (watki[k][2].joinable()) watki[k][2].join();
                }
            }
        output=*std::max(tymczasowePary.begin(), tymczasowePary.end());
    }
    else
    {
        std::vector<double> tymczasowePary(prefferedSize,DBL_MAX);
        std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA];
        for (i=0;i<prefferedSize;i++)
            for (j=0;j<prefferedSize;j+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
            {
                for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+j)<prefferedSize);k++)
                    watki[k]=std::thread(&ImageSegments::getMinsMaxsOfPixel,this,MODE_GRAY,i,j+k,&tymczasowePary[i+j+k]);
                for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
                    if (watki[k].joinable()) watki[k].join();
            }
        output=*std::max(tymczasowePary.begin(), tymczasowePary.end());
    }
    return output;
}

double ImageSegments::getDeviationValue(unsigned int mode, unsigned int x, unsigned int y)
{
    if (mode==MODE_R) return standartDeviationRGB[x][y].r;
    else if (mode==MODE_G) return standartDeviationRGB[x][y].g;
    else if (mode==MODE_B) return standartDeviationRGB[x][y].b;
    return standartDeviationGray[x][y].gray;
}

bool ImageSegments::getIsRGB() { return isRGB; }

unsigned int ImageSegments::getNumberOfSegments() { return (unsigned int)imageSegments.size(); }
unsigned int ImageSegments::getPrefferedSize() { return prefferedSize; }

void ImageSegments::setSegment(unsigned int x, unsigned int y, ImageSegment segment) { imageSegments[x*(imageWidth/prefferedSize)+y]=segment; }

void ImageSegments::clearClosestsCentroids()
{
    unsigned int i;
    for (i=0;i<imageSegments.size();i++) imageSegments[i].setClosestCentroidNumber(UINT_MAX);
}

void ImageSegments::normalizeSegments()
{
    unsigned int i,j,k;
    if (isRGB)
    {
        std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA][3];
        
        std::vector<std::vector<doublePixelRGB>> srednie;
        
        //Rozmiar vectoru z odcyleniami standartowymi
        standartDeviationRGB.resize(prefferedSize);
        for (i=0;i<prefferedSize;i++) standartDeviationRGB[i].resize(prefferedSize);
        
        //Vector ze srednimi o rozmiarze jednego segmentu
        srednie.resize(prefferedSize);
        for (i=0;i<prefferedSize;i++) srednie[i].resize(prefferedSize);
        
        //Obliczanie srednich w wielu watkach
        for (i=0;i<prefferedSize;i++)
            for (j=0;j<prefferedSize;j+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
            {
                for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+j)<prefferedSize);k++)
                {
                    watki[k][0]=std::thread(&ImageSegments::calculateMean,this,MODE_R,&srednie[i][j+k].r,i,j+k);
                    watki[k][1]=std::thread(&ImageSegments::calculateMean,this,MODE_G,&srednie[i][j+k].g,i,j+k);
                    watki[k][2]=std::thread(&ImageSegments::calculateMean,this,MODE_B,&srednie[i][j+k].b,i,j+k);
                }
                for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
                {
                	if (watki[k][0].joinable()) watki[k][0].join();
                    if (watki[k][1].joinable()) watki[k][1].join();
                    if (watki[k][2].joinable()) watki[k][2].join();
                }
            }
        //Obliczanie odchylenia standartowego w wielu watkach
        for (i=0;i<prefferedSize;i++)
            for (j=0;j<prefferedSize;j+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
            {
                for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+j)<prefferedSize);k++)
                {
                    watki[k][0]=std::thread(&ImageSegments::calculateStandartDeviation,this,MODE_R,srednie[i][j+k].r,i,j+k);
                    watki[k][1]=std::thread(&ImageSegments::calculateStandartDeviation,this,MODE_G,srednie[i][j+k].g,i,j+k);
                    watki[k][2]=std::thread(&ImageSegments::calculateStandartDeviation,this,MODE_B,srednie[i][j+k].b,i,j+k);
                }
                for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
                {
                    if (watki[k][0].joinable()) watki[k][0].join();
                    if (watki[k][1].joinable()) watki[k][1].join();
                    if (watki[k][2].joinable()) watki[k][2].join();
                }
            }
    }
    else
    {
        std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA];
        
        std::vector<std::vector<doublePixelGray>> srednie;
        
        //Rozmiar vectoru z odcyleniami standartowymi
        standartDeviationGray.resize(prefferedSize);
        for (i=0;i<prefferedSize;i++) standartDeviationGray[i].resize(prefferedSize);
        
        //Vector ze srednimi o rozmiarze jednego segmentu
        srednie.resize(prefferedSize);
        for (i=0;i<prefferedSize;i++) srednie[i].resize(prefferedSize);
        
        //Obliczanie srednich w wielu watkach
        for (i=0;i<prefferedSize;i++)
            for (j=0;j<prefferedSize;j+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
            {
                for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+j)<prefferedSize);k++)
                    watki[k]=std::thread(&ImageSegments::calculateMean,this,MODE_GRAY,&srednie[i][j+k].gray,i,j+k);
                for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
                    if (watki[k].joinable()) watki[k].join();
            }
        //Obliczanie odchylenia standartowego w wielu watkach
        for (i=0;i<prefferedSize;i++)
            for (j=0;j<prefferedSize;j+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
            {
                for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+j)<prefferedSize);k++)
                    watki[k]=std::thread(&ImageSegments::calculateStandartDeviation,this,MODE_GRAY,srednie[i][j+k].gray,i,j+k);
                for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
                    if (watki[k].joinable()) watki[k].join();
            }
    }
    //Normalizacja w segmentach
    std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA];
    for (i=0;i<imageSegments.size();i+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
    {
        for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+i)<imageSegments.size());k++)
            watki[k]=std::thread(&ImageSegment::nomralizePixels,&imageSegments[i+k]);
        for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
            if (watki[k].joinable()) watki[k].join();
    }
}

void ImageSegments::denormalizeSegments()
{
    if (!imageSegments[0].getIsModified())
    {
        throw std::string("Nie można zdenormalizować pixeli bez wcześniejszego ich znormalizowania!");
    }
    else
    {
        unsigned int i,k;
        //Denormalizacja w segmentach
        std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA];
        for (i=0;i<imageSegments.size();i+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
        {
            for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+i)<imageSegments.size());k++)
                watki[k]=std::thread(&ImageSegment::denomralizePixels,&imageSegments[i+k]);
            for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
                if (watki[k].joinable()) watki[k].join();
        }
    }
}

void ImageSegments::generateFinalPixels()
{
    unsigned int i,j,k;
    std::thread watki[MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA];
    
    for (i=0;i<(imageHeight/prefferedSize);i++)
        for (j=0;j<(imageWidth/prefferedSize);j+=MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)
        {
            for (k=0;(k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA)&&((k+j)<(imageWidth/prefferedSize));k++)
                watki[k]=std::thread(&ImageSegments::updatePixelsWithSegment,this,i,j+k);
            for (k=0;k<MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA;k++)
                if (watki[k].joinable()) watki[k].join();
        }
}
