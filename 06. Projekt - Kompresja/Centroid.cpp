//
//  Centroid.cpp
//  06. Projekt - Kompresja
//
//  Created by Krystian Owoc on 03.07.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include "Centroids.hpp"
#include "Centroid.hpp"

Centroid::Centroid(std::vector<std::vector<doublePixelRGB>> pixels, Centroids* centroids): pixelsRGB(pixels), centroids(centroids) {}
Centroid::Centroid(std::vector<std::vector<doublePixelGray>> pixels, Centroids* centroids): pixelsGray(pixels), centroids(centroids) {}
Centroid::Centroid(Centroids* centroids): centroids(centroids) {}

std::vector<std::vector<doublePixelRGB>> Centroid::getPixelsRGB() { return pixelsRGB; }
std::vector<std::vector<doublePixelGray>> Centroid::getPixelsGray() { return pixelsGray; }
void Centroid::setPixelsRGB(std::vector<std::vector<doublePixelRGB>> pixels) { pixelsRGB=pixels; }
void Centroid::setPixelsGray(std::vector<std::vector<doublePixelGray>> pixels) { pixelsGray=pixels; }
unsigned int Centroid::getClosestForSegments() { return isClosestForSegments; }

void Centroid::incrementClosestForSegments() { isClosestForSegments++; }
void Centroid::decrementClosestForSegments() { if (isClosestForSegments!=0) isClosestForSegments--; }

bool operator==(const Centroid& centroid1, const Centroid& centroid2)
{
    unsigned int x,y;
    
    if (centroid1.pixelsRGB.size()!=centroid2.pixelsRGB.size()) return false;
    for (x=0;x<centroid1.pixelsRGB.size();x++) if (centroid1.pixelsRGB[x].size()!=centroid2.pixelsRGB[x].size()) return false;
    if (centroid1.pixelsGray.size()!=centroid2.pixelsGray.size()) return false;
    for (x=0;x<centroid1.pixelsGray.size();x++) if (centroid1.pixelsGray[x].size()!=centroid2.pixelsGray[x].size()) return false;
    
    for (x=0;x<centroid1.pixelsRGB.size();x++)
        for (y=0;y<centroid1.pixelsRGB[x].size();y++)
        	{
                if (centroid1.pixelsRGB[x][y].r!=centroid2.pixelsRGB[x][y].r) return false;
                else if (centroid1.pixelsRGB[x][y].g!=centroid2.pixelsRGB[x][y].g) return false;
                else if (centroid1.pixelsRGB[x][y].b!=centroid2.pixelsRGB[x][y].b) return false;
            }
    for (x=0;x<centroid1.pixelsGray.size();x++)
        for (y=0;y<centroid1.pixelsGray[x].size();y++)
            if (centroid1.pixelsGray[x][y].gray!=centroid2.pixelsGray[x][y].gray) return false;
    
    return true;
}
