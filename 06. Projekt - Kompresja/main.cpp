//
//  main.cpp
//  03.Neuron Sigmoidalny
//
//  Created by Krystian Owoc on 08.03.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include <iostream>


#include "Zarzadca.hpp"
#include "CLIInterface.hpp"
#include "TextFileInterface.hpp"

int main(int argc, const char * argv[])
{
    Interface* interfejs=new TextFileInterface();
    Zarzadca* glownyZarzadca=nullptr;
    try
    {
        glownyZarzadca=new Zarzadca("08.bmp", 512, 4, interfejs); // 6 i 8 pixeli to optymalna wartość dla prędkości wykonania segmentacji, 4 najszybsze w przypadku nauki
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }
    glownyZarzadca->glownaPetlaKmeans(4,25);
    glownyZarzadca->zapiszDoPliku("c08.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    if (interfejs!=nullptr) delete interfejs;
    return 0;
}
