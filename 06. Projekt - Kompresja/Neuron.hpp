//
//  Neuron.hpp
//  04. Perceptron wielowarstwowy
//
//  Created by Krystian Owoc on 06.04.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef Neuron_hpp
#define Neuron_hpp

#include <vector>
#include <stdlib.h>
#include "Interface.hpp"
#include "GlobalDefines.hpp"
#include "Wejscie.hpp"

class Warstwa;

class Neuron //TODO Neuron na watkach
{
private:
	double oczekiwana=0;
	double odpowiedz=0;
    double bladWstecznejPropagacji=0;
	std::vector<Wejscie> wejscia;
    Wejscie bias=Wejscie(DOMYSLNA_WAGA_WEJSC);
    bool czyUzywacBias;
	Warstwa* warstwa;
	double wspolczynnikNauki=1.0; //Alfa
	double wspolczynnikMomentum=1.0; //Beta
public:
	Neuron(Warstwa* warstwa, bool czyUzywacBias=true);
    Neuron(Warstwa* warstwa, double bias);
    
    void setWarstwa(Warstwa* warstwa);
    
	void generujOdpowiedzSigmoidalnie();
    void generujOdpowiedzTangensoidalnie();
    void generujOdpowiedzLiniowo();
	void generujOdpowiedzPrzekazujac(); //Nie moze dojsc do sytuacji gdzie bedzie wiecej wejsc niz 1
    void generujBladWstecznejPropagacjiDlaSigmoidy();
    void generujBladWstecznejPropagacjiDlaTangensoidy();
    void generujBladWstecznejPropagacjiDlaLini();
	void nauczWstecznaPropagacjaBledow(unsigned int trybOdpowiedzi);
	void przekazOdpowiedz();

	void setOczekiwana(double oczekiwana);
	void setWspolczynnikNauki(double wspolczynnikNauki);
	void setWspolczynnikMomentum(double wspolczynnikMomentum);
    double getBladWstecznejPropagacji();
    double getOdpowiedz();
    double getOczekiwana();

	void pushWejscie(Wejscie wejscie);
	void eraseWejscie(int idWejscia);
	void setWejscieW(int idWejscia, double wejscie);
    void setWejscie(int idWejscia, Wejscie wejscie);
    double getWagaWejscia(int idWejscia);
    
    friend std::istream& operator>>(std::istream& is, Neuron& obj); //Zapis do klasy
    //Odczyt z klasy
    friend std::ostream& operator<<(std::ostream& os, const Neuron& obj); //cout
    friend std::string& operator<<(std::string& s, const Neuron& obj); //string
};

#endif /* Neuron_hpp */
