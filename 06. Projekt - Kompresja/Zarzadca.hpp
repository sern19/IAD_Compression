//
//  Zarzadca.hpp
//  04. Perceptron wielowarstwowy
//
//  Created by Krystian Owoc on 06.04.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef Zarzadca_hpp
#define Zarzadca_hpp

#include <sstream>
#include "Warstwa.hpp"
#include "Interface.hpp"

class Zarzadca
{
private:
    std::vector<Warstwa> warstwy;
    double bladGlobalny=0;
    Interface* interfejs=NULL;
    //Zaprzyjaznienia
    friend class ZarzadcaKlasyfikacja;
    friend class ZarzadcaAproksymacja;
public:
    Zarzadca(Interface* interfejs);
    Zarzadca(unsigned int numberOfNeuronsInHiddenLayer, Interface* interfejs, bool czyUzywacBias=true, double wspolczynnikMomentum=WSPOLCZYNNIK_MOMENTUM, double wspolczynnikNauki=WSPOLCZYNNIK_NAUKI);
    Warstwa* getNastepnaWarstwa(Warstwa* aktualnaWarstwa);
    unsigned int getIdWarstwy(Warstwa* warstwa);
    
    virtual void glownaPetla(unsigned int iloscIteracji);
    
    virtual void trybTestowania();
    
    Interface* getInterface();
    
    void wczytajSiecZPliku(std::string filename);
    void zapiszSiecDoPliku(std::string filename);
};
#endif
