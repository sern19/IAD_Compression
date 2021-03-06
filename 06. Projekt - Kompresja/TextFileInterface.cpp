//
//  TextFileInterface.cpp
//  06. Projekt - Kompresja
//
//  Created by Krystian Owoc on 04.07.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#define _CRT_SECURE_NO_WARNINGS 1

#include "TextFileInterface.hpp"
#include <time.h>

TextFileInterface::TextFileInterface()
{
    time_t now = time(0);
    struct tm ltm;
    #ifdef __APPLE__
        localtime_r(&now,&ltm);
    #elif defined _WIN32 || defined _WIN64
        localtime_s(&ltm, &now);
    #endif    
    outputFile.open("06. Projekt kompresja - Output.txt",std::ios_base::app);
    outputFile << "\n----Otwarto plik (" << 1900 + ltm.tm_year << "." << 1 + ltm.tm_mon << "." << ltm.tm_mday << " ";
    outputFile << 1 + ltm.tm_hour << ":" << 1 + ltm.tm_min << ":" << 1 + ltm.tm_sec << ")----" << std::endl;
}

TextFileInterface::~TextFileInterface()
{
    if (outputFile.is_open())
    {
        time_t now = time(0);
        struct tm ltm;
        #ifdef __APPLE__
                localtime_r(&now,&ltm);
        #elif defined _WIN32 || defined _WIN64
                localtime_s(&ltm, &now);
        #endif
        outputFile << "\n----Zamknięto plik (" << 1900 + ltm.tm_year << "." << 1 + ltm.tm_mon << "." << ltm.tm_mday << " ";
        outputFile << 1 + ltm.tm_hour << ":" << 1 + ltm.tm_min << ":" << 1 + ltm.tm_sec << ")----" << std::endl;
        outputFile.flush();
    }
    if (outputFile.is_open()) outputFile.close();
}

void TextFileInterface::updateFile()
{
    if (outputFile.is_open())
    {
    	outputFile.flush();
        outputFile.close();
        outputFile.open("06. Projekt kompresja - Output.txt",std::ios_base::app);
    }
}

void TextFileInterface::showComunicat(std::string text)
{
    if (outputFile.is_open()) outputFile << text << "\n";
    updateFile();
}

void TextFileInterface::showComunicat(double text)
{
    if (outputFile.is_open()) outputFile << text << "\n";
    updateFile();
}

void TextFileInterface::showComunicat(int text)
{
    if (outputFile.is_open()) outputFile << text << "\n";
    updateFile();
}


void TextFileInterface::showText(std::string text)
{
    if (outputFile.is_open()) outputFile << text;
    updateFile();
}

void TextFileInterface::showText(double text)
{
    if (outputFile.is_open()) outputFile << text;
    updateFile();
}

void TextFileInterface::showText(int text)
{
    if (outputFile.is_open()) outputFile << text;
    updateFile();
}

void TextFileInterface::waitToContinue()
{
    if (outputFile.is_open()) outputFile << "Naciśnij dowolny klawisz aby kontynuować" << std::endl;
    updateFile();
}
