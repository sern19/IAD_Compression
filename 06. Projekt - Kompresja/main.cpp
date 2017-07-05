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
    //Interface* interfejs=new CLIInterface();
    Zarzadca* glownyZarzadca=nullptr;
    try
    {
        glownyZarzadca=new Zarzadca("01.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c01.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("02.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c02.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("03.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c03.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("04.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c04.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("05.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c05.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("06.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c06.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("07.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c07.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("08.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c08.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("09.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c09.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("10.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c10.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("11.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c11.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("12.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c12.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("13.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c13.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("14.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c14.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("15.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c15.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("16.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("16.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("17.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c17.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("18.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c18.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("19.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c19.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    try
    {
        glownyZarzadca=new Zarzadca("20.bmp", 75, 4, interfejs);
    }
    catch (std::string comunicat)
    {
        interfejs->showComunicat(comunicat);
        return 1;
    }glownyZarzadca->glownaPetlaNeuralGas(25000);
    glownyZarzadca->zapiszDoPliku("c20.bmp");
    if (glownyZarzadca!=nullptr) delete glownyZarzadca;
    if (interfejs!=nullptr) delete interfejs;
    return 0;
}

//glownyZarzadca->glownaPetlaKmeans(1,10);
//glownyZarzadca->glownaPetlaNeuralGas(25000); //50 tys starcza na noc
//glownyZarzadca->glownaPetlaKohonen(2500,0.4); //50 tys starcza na noc
