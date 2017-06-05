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

std::string CLIInterface::convertPolishCharacters(std::string input)
{
    int i;
    std::stringstream output;
    for(i=0;i<input.size();i++)
            if (strncmp(&input[i],"ą",1)==0) output << "Ą";
            else if (strncmp(&input[i],"ć",1)==0) output << "†";
            else if (strncmp(&input[i],"ę",1)==0) output << "©";
            else if (strncmp(&input[i],"ł",1)==0) output << "";
            else if (strncmp(&input[i],"ń",1)==0) output << "ä";
			else if (strncmp(&input[i],"ó",1)==0) output << "˘";
            else if (strncmp(&input[i],"ś",1)==0) output << "";
            else if (strncmp(&input[i],"ż",1)==0) output << "ľ";
            else if (strncmp(&input[i],"ź",1)==0) output << "«";
            else if (strncmp(&input[i],"Ą",1)==0) output << "¤";
            else if (strncmp(&input[i],"Ć",1)==0) output << "Ź";
            else if (strncmp(&input[i],"Ę",1)==0) output << "¨";
            else if (strncmp(&input[i],"Ł",1)==0) output << "ť";
            else if (strncmp(&input[i],"Ń",1)==0) output << "ă";
            else if (strncmp(&input[i],"Ó",1)==0) output << "ŕ";
            else if (strncmp(&input[i],"Ś",1)==0) output << "—";
            else if (strncmp(&input[i],"Ż",1)==0) output << "˝";
            else if (strncmp(&input[i],"Ź",1)==0) output << "Ť";
            else output << input[i];
    return output.str();
}

void CLIInterface::showComunicat(std::string text)
{
#if defined _WIN32 || defined _WIN64
    text=convertPolishCharacters(text);
#endif
    std::cout << text << "\n";
}

void CLIInterface::showText(std::string text)
{
#if defined _WIN32 || defined _WIN64
    text=convertPolishCharacters(text);
#endif
    std::cout << text;
}

void CLIInterface::waitToContinue()
{
    std::cout << "Naciśnij dowolny klawisz aby kontynuować\n";
    std::cin.get();
}
