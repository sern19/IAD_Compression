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
    //Interface* interfejs=new TextFileInterface();
    Interface* interfejs=new CLIInterface();
    Zarzadca* glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("01.bmp", 50, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }
    
    glownyZarzadca->glownaPetlaKohonen(2500,0.4);
    glownyZarzadca->zapiszDoPliku("c01.bmp");
    
    if (glownyZarzadca!=NULL) delete glownyZarzadca;
    if (interfejs!=nullptr) delete interfejs;
    return 0;
}

