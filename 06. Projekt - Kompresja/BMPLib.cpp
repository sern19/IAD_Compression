//
//  BMPLib.cpp
//  05. Projekt - Kompresja
//
//  Created by Krystian Owoc on 24.05.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include "BMPLib.hpp"
#include <fstream>

BMPImage::BMPImage()
{
    pixelsRGB.reserve(0);
    pixelsGray.reserve(0);
}
BMPImage::BMPImage(std::string filename): filename(filename)
{
    pixelsRGB.reserve(0);
    pixelsGray.reserve(0);
    loadFile();
}

void BMPImage::openFile(std::string filename)
{
    this->filename=filename;
    loadFile();
}
void BMPImage::saveFile(std::string filename)
{
    int i,j;
    uint8_t tmp = 0, zero = 0;
    std::ofstream outputFile;
    outputFile.open(filename, std::ios_base::out | std::ios_base::binary);
    if (outputFile.is_open())
    {
        //FileHeader
        outputFile.write((char*)&fileHeader.bfType, sizeof(fileHeader.bfType));
        outputFile.write((char*)&fileHeader.bfSize, sizeof(fileHeader.bfSize));
        outputFile.write((char*)&fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
        outputFile.write((char*)&fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
        outputFile.write((char*)&fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));
        //InfoHeader
        outputFile.write((char*)&infoHeader.biSize, sizeof(infoHeader.biSize));
        outputFile.write((char*)&infoHeader.biWidth, sizeof(infoHeader.biWidth));
        outputFile.write((char*)&infoHeader.biHeight, sizeof(infoHeader.biHeight));
        outputFile.write((char*)&infoHeader.biPlanes, sizeof(infoHeader.biPlanes));
        outputFile.write((char*)&infoHeader.biBitCount, sizeof(infoHeader.biBitCount));
        outputFile.write((char*)&infoHeader.biCompression, sizeof(infoHeader.biCompression));
        outputFile.write((char*)&infoHeader.biSizeImage, sizeof(infoHeader.biSizeImage));
        outputFile.write((char*)&infoHeader.biXPelsPerMeter, sizeof(infoHeader.biXPelsPerMeter));
        outputFile.write((char*)&infoHeader.biYPelsPerMeter, sizeof(infoHeader.biYPelsPerMeter));
        outputFile.write((char*)&infoHeader.biClrUsed, sizeof(infoHeader.biClrUsed));
        outputFile.write((char*)&infoHeader.biClrImportant, sizeof(infoHeader.biClrImportant));
        if (infoHeader.biBitCount==8)
        {
            //Reconsruct color index
            for (i=0;i<256;i++)
            {
                outputFile.write((char*)&tmp, sizeof(tmp));
                outputFile.write((char*)&tmp, sizeof(tmp));
                outputFile.write((char*)&tmp, sizeof(tmp));
                outputFile.write((char*)&zero, sizeof(zero));
                tmp++;
            }
            if (infoHeader.biHeight>=0)
            {
                for (i=0;i<infoHeader.biHeight;i++)
                    for (j=0;j<stride;j++) //Stride,
                    {
                        if (j<pixelsGray.at(i).size()) tmp=pixelsGray[i][j].gray;
                        else tmp=0;
                        outputFile.write((char*)&tmp,sizeof(tmp));
                    }
            }
            else
            {
                for (i=abs(infoHeader.biHeight)-1;i>=0;i--)
                    for (j=0;j<stride;j++) //Stride,
                    {
                        if (j<pixelsGray.at(i).size()) tmp=pixelsGray[i][j].gray;
                        else tmp=0;
                        outputFile.write((char*)&tmp,sizeof(tmp));
                    }
            }
        } else if (infoHeader.biBitCount==24)
        {
            if (infoHeader.biHeight>=0)
            {
                for (i=0;i<infoHeader.biHeight;i++)
                    for (j=0;j<stride;j++) //Stride,
                    {
                        if (j<pixelsRGB.at(i).size()) tmp=pixelsRGB[i][j].b;
                        else tmp=0;
                        outputFile.write((char*)&tmp,sizeof(tmp));
                        if (j<pixelsRGB.at(i).size()) tmp=pixelsRGB[i][j].g;
                        else tmp=0;
                        outputFile.write((char*)&tmp,sizeof(tmp));
                        if (j<pixelsRGB.at(i).size()) tmp=pixelsRGB[i][j].r;
                        else tmp=0;
                        outputFile.write((char*)&tmp,sizeof(tmp));
                    }
            }
            else
            {
                for (i=abs(infoHeader.biHeight)-1;i>=0;i--)
                    for (j=0;j<stride;j++) //Stride,
                    {
                        if (j<pixelsRGB.at(i).size()) tmp=pixelsRGB[i][j].b;
                        else tmp=0;
                        outputFile.write((char*)&tmp,sizeof(tmp));
                        if (j<pixelsRGB.at(i).size()) tmp=pixelsRGB[i][j].g;
                        else tmp=0;
                        outputFile.write((char*)&tmp,sizeof(tmp));
                        if (j<pixelsRGB.at(i).size()) tmp=pixelsRGB[i][j].r;
                        else tmp=0;
                        outputFile.write((char*)&tmp,sizeof(tmp));
                    }
            }
        }
        outputFile.close();
    }
    else
    {
        throw std::string("Nie można utworzyć pliku!");
    }
}
void BMPImage::reloadFile() { loadFile(); }

void BMPImage::loadFile()
{
    int i,j;
    uint8_t tmp = 0;
    std::ifstream inputFile;
    inputFile.open(filename, std::ios_base::in | std::ios_base::binary);
    if (inputFile.is_open())
    {
        inputFile.seekg(0,std::ios_base::end);
        int fileSize = (int)inputFile.tellg();
        inputFile.seekg(0);
        //FileHeader
        inputFile.read((char*)&fileHeader.bfType, sizeof(fileHeader.bfType));
        inputFile.read((char*)&fileHeader.bfSize, sizeof(fileHeader.bfSize));
        inputFile.read((char*)&fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
        inputFile.read((char*)&fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
        inputFile.read((char*)&fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));
        //Check if is BMP
        if ((fileHeader.bfType!=19778)||(fileHeader.bfReserved1!=0)||(fileHeader.bfReserved2!=0)||(fileSize!=fileHeader.bfSize))
        {
            inputFile.close();
            throw std::string("To nie jest prawidłowy plik bitmapy!");
        }
        //InfoHeader
        inputFile.read((char*)&infoHeader.biSize, sizeof(infoHeader.biSize));
        inputFile.read((char*)&infoHeader.biWidth, sizeof(infoHeader.biWidth));
        inputFile.read((char*)&infoHeader.biHeight, sizeof(infoHeader.biHeight));
        inputFile.read((char*)&infoHeader.biPlanes, sizeof(infoHeader.biPlanes));
        inputFile.read((char*)&infoHeader.biBitCount, sizeof(infoHeader.biBitCount));
        inputFile.read((char*)&infoHeader.biCompression, sizeof(infoHeader.biCompression));
        inputFile.read((char*)&infoHeader.biSizeImage, sizeof(infoHeader.biSizeImage));
        inputFile.read((char*)&infoHeader.biXPelsPerMeter, sizeof(infoHeader.biXPelsPerMeter));
        inputFile.read((char*)&infoHeader.biYPelsPerMeter, sizeof(infoHeader.biYPelsPerMeter));
        inputFile.read((char*)&infoHeader.biClrUsed, sizeof(infoHeader.biClrUsed));
        inputFile.read((char*)&infoHeader.biClrImportant, sizeof(infoHeader.biClrImportant));
        //Check if it's supported by program
        if ((infoHeader.biCompression!=0)||((infoHeader.biBitCount!=8)&&(infoHeader.biBitCount!=24)))
        {
            inputFile.close();
            throw std::string("Jedynie nieskompresowane obrazy 8-bitowe bądź 24-bitowe są obsługiwane!");
        }
        //Calculate stride
        stride = infoHeader.biWidth*infoHeader.biBitCount;  // bits per row
        stride += 31;            // round up to next 32-bit boundary
        stride /= 32;            // DWORDs per row
        stride *= 4;             // bytes per row
        stride /= (infoHeader.biBitCount/8);
        if (infoHeader.biBitCount==8)
        {
            pixelRGB tymczasowy;
            //Check if it's greyscale
            for (i=0;i<256;i++)
            {
                inputFile.read((char*)&tmp,sizeof(tmp));
                tymczasowy.b=tmp;
                inputFile.read((char*)&tmp,sizeof(tmp));
                tymczasowy.g=tmp;
                inputFile.read((char*)&tmp,sizeof(tmp));
                tymczasowy.r=tmp;
                inputFile.read((char*)&tmp,sizeof(tmp));
                if((tymczasowy.r!=tymczasowy.g)||(tymczasowy.r!=tymczasowy.b))
                {
                    inputFile.close();
                    throw std::string("Indeksowane kolorowe obrazy 8-bitowe nie są obsługiwane!");
                }
            }
            pixelsGray=std::vector<std::vector<pixelGray>>(abs(infoHeader.biHeight),std::vector<pixelGray>(abs(infoHeader.biWidth)));
            inputFile.seekg(fileHeader.bfOffBits); //Przejscie do poczatku obrazka
            if (infoHeader.biHeight>=0)
            {
                for (i=0;i<infoHeader.biHeight;i++)
                    for (j=0;j<stride;j++) //Stride,
                    {
                        inputFile.read((char*)&tmp,sizeof(tmp));
                        if (j<pixelsGray.at(i).size()) pixelsGray[i][j].gray=tmp;
                    }
            }
            else
            {
                for (i=abs(infoHeader.biHeight)-1;i>=0;i--)
                    for (j=0;j<stride;j++) //Stride,
                    {
                        inputFile.read((char*)&tmp,sizeof(tmp));
                        if (j<pixelsGray.at(i).size()) pixelsGray[i][j].gray=tmp;
                    }
            }
        }
        else
        {
            pixelsRGB=std::vector<std::vector<pixelRGB>>(abs(infoHeader.biHeight),std::vector<pixelRGB>(abs(infoHeader.biWidth)));
            inputFile.seekg(fileHeader.bfOffBits); //Przejscie do poczatku obrazka
            if (infoHeader.biHeight>=0)
            {
                for (i=0;i<infoHeader.biHeight;i++)
                    for (j=0;j<stride;j++) //Stride,
                    {
                        inputFile.read((char*)&tmp,sizeof(tmp));
                        if (j<pixelsRGB.at(i).size()) pixelsRGB[i][j].b=tmp;
                        inputFile.read((char*)&tmp,sizeof(tmp));
                        if (j<pixelsRGB.at(i).size()) pixelsRGB[i][j].g=tmp;
                        inputFile.read((char*)&tmp,sizeof(tmp));
                        if (j<pixelsRGB.at(i).size()) pixelsRGB[i][j].r=tmp;
                    }
            }
            else
            {
                for (i=abs(infoHeader.biHeight)-1;i>=0;i--)
                    for (j=0;j<stride;j++) //Stride,
                    {
                        inputFile.read((char*)&tmp,sizeof(tmp));
                        if (j<pixelsRGB.at(i).size()) pixelsRGB[i][j].b=tmp;
                        inputFile.read((char*)&tmp,sizeof(tmp));
                        if (j<pixelsRGB.at(i).size()) pixelsRGB[i][j].g=tmp;
                        inputFile.read((char*)&tmp,sizeof(tmp));
                        if (j<pixelsRGB.at(i).size()) pixelsRGB[i][j].r=tmp;
                    }
            }
        }
    	inputFile.close();
    }
    else
    {
        throw std::string("Nie można otworzyć pliku!");
    }
}
bool BMPImage::isRGB()
{
    if (infoHeader.biBitCount==24) return true;
    else return false;
}
void BMPImage::setRGBPixel(int x, int y, pixelRGB pixel) { pixelsRGB[x][y]=pixel; }
void BMPImage::setGrayPixel(int x, int y, pixelGray pixel) { pixelsGray[x][y]=pixel; }
unsigned int BMPImage::getHeight() { return abs(infoHeader.biHeight); }
unsigned int BMPImage::getWidth() { return abs(infoHeader.biWidth); }
std::vector<std::vector<pixelRGB>> BMPImage::getRGBPixels() { return pixelsRGB; }
std::vector<std::vector<pixelGray>> BMPImage::getGrayPixels() { return pixelsGray; }
void BMPImage::setRGBPixels(std::vector<std::vector<pixelRGB>> pixels) { pixelsRGB=pixels; }
void BMPImage::setGrayPixels(std::vector<std::vector<pixelGray>> pixels) { pixelsGray=pixels; }
