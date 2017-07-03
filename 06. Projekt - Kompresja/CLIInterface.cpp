//
//  CLIInterface.cpp
//  05.Perceptron Wielowarstwowy - Klasyfikacja
//
//  Created by Krystian Owoc on 24.05.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include "CLIInterface.hpp"

#include <iostream>
#include <sstream>
#include <stdio.h>

CLIInterface::CLIInterface() { }

void CLIInterface::showComunicat(std::string text)
{
    std::cout << text << "\n";
}

void CLIInterface::showComunicat(double text)
{
    std::cout << text << "\n";
}

void CLIInterface::showComunicat(int text)
{
    std::cout << text << "\n";
}


void CLIInterface::showText(std::string text)
{
	std::cout << text;
}

void CLIInterface::showText(double text)
{
    std::cout << text;
}

void CLIInterface::showText(int text)
{
    std::cout << text;
}

void CLIInterface::waitToContinue()
{
    std::cout << "Naciśnij dowolny klawisz aby kontynuować" << std::endl;
    std::cin.get();
}
