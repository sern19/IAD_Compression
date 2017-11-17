//
//  TextFileInterface.hpp
//  06. Projekt - Kompresja
//
//  Created by Krystian Owoc on 04.07.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef TextFileInterface_hpp
#define TextFileInterface_hpp

#include <stdio.h>
#include <fstream>

#include "Interface.hpp"

class TextFileInterface: public Interface
{
private:
    std::ofstream outputFile;
    void updateFile();
public:
    TextFileInterface();
    ~TextFileInterface();
    void showComunicat(std::string text);
    void showComunicat(double text);
    void showComunicat(int text);
    void showText(std::string text);
    void showText(double text);
    void showText(int text);
    void waitToContinue();
};

#endif /* TextFileInterface_hpp */
