//
//  Warstwa.hpp
//  04. Perceptron wielowarstwowy
//
//  Created by Krystian Owoc on 06.04.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef Warstwa_hpp
#define Warstwa_hpp

#include "Neuron.hpp"
#include "GlobalDefines.hpp"


class Zarzadca;

class Warstwa
{
private:
	std::vector<Neuron> neurony;
	Zarzadca* zarzadca;
	unsigned int trybNauki;
	unsigned int trybOdpowiedzi;
    double wspolczynnikNauki=DOMYSLNY_WSPOLCZYNNIK_NAUKI; //Alfa
    double wspolczynnikMomentum=DOMYSLNY_WSPOLCZYNNIK_MOMENTUM; //Beta
public:
	Warstwa(Zarzadca* zarzadca, unsigned char trybOdpowiedzi=SIGMOIDALNIE, unsigned char trybNauki=WSTECZNA_PROPAGACJA_BLEDOW);
    
    void setZarzadca(Zarzadca* zarzadca);
    
	void generujOdpowiedzi();
	void uczNeurony();

	unsigned int getIdNeuronu(Neuron* neuron);
    Warstwa* getNastepnaWarstwa();
    std::vector<Neuron>* getNeurony();
    Interface* getInterface();
    
	void setTrybNauki(unsigned char trybNauki);
	void setTrybOdpowiedzi(unsigned char trybOdpowiedzi);
    
    //Te dwie wartosci powinny byc ustawiane po utworzeniu obiektu
    void setWspolczynnikNauki(double wspolczynnikNauki);
    void setWspolczynnikMomentum(double wspolczynnikMomentum);
    //
    
    void pushNeuron(Neuron neuron);
    void eraseNeuron(int idNeuronu);
    void setNeuron(int idNeuronu, Neuron neuron);
    
    friend std::istream& operator>>(std::istream& is, Warstwa& obj); //Zapis do klasy
    //Odczyt z klasy
    friend std::ostream& operator<<(std::ostream& os, const Warstwa& obj); //cout
    friend std::string& operator<<(std::string& s, const Warstwa& obj); //string
};

#endif
