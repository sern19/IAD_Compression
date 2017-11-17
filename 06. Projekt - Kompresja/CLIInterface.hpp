//
//  CLIInterface.hpp
//  05.Perceptron Wielowarstwowy - Klasyfikacja
//
//  Created by Krystian Owoc on 24.05.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef CLIInterface_hpp
#define CLIInterface_hpp

#include "Interface.hpp"

class CLIInterface: public Interface
{
public:
    CLIInterface();
    void showComunicat(std::string text);
    void showComunicat(double text);
    void showComunicat(int text);
    void showText(std::string text);
    void showText(double text);
    void showText(int text);
    void waitToContinue();
};

#endif /* CLIInterface_hpp */
