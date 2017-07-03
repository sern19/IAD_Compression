//
//  main.cpp
//  03.Neuron Sigmoidalny
//
//  Created by Krystian Owoc on 08.03.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include <iostream>


#include "BMPLib.hpp"
#include "ImageSegments.hpp"

int main(int argc, const char * argv[])
{
    BMPImage tmp("t2.bmp");
    ImageSegments segmenty=ImageSegments(8, tmp); // 6 i 8 bardzo optymalna liczba la predkosci az do normalizacji
    segmenty.normalizeSegments();
    std::cout << "Lol koniec";
    std::cin.get();
    //tmp.saveFile("t222.bmp");
    return 0;
}
