//
//  CLIInterface.hpp
//  05.Perceptron Wielowarstwowy - Klasyfikacja
//
//  Created by Krystian Owoc on 24.05.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef CLIInterface_hpp
#define CLIInterface_hpp

#include <stdio.h>
#include "Interface.hpp"

class CLIInterface: public Interface
{
private:
    std::string convertPolishCharacters(std::string input);
public:
    void showComunicat(std::string text);
    void showText(std::string text);
    void waitToContinue();
};

#endif /* CLIInterface_hpp */
