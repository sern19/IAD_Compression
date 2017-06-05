//
//  Interface.hpp
//  05.Perceptron Wielowarstwowy - Klasyfikacja
//
//  Created by Krystian Owoc on 24.05.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef Interface_hpp
#define Interface_hpp

#include <stdio.h>
#include <string>

class Interface
{
public:
    virtual ~Interface() {}
    virtual void showComunicat(std::string text)=0;
    virtual void showText(std::string text)=0;
    virtual void waitToContinue()=0;
};

#endif /* Interface_hpp */
