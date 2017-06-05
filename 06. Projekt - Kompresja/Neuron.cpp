//
//  Neuron.cpp
//  04.Perceptron Wielowarstwowy
//
//  Created by Krystian Owoc on 17.04.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include <math.h>
#include "Neuron.hpp"
#include "Warstwa.hpp"
#include "Zarzadca.hpp"

Neuron::Neuron(Warstwa* warstwa, bool czyUzywacBias):warstwa(warstwa), czyUzywacBias(czyUzywacBias) {}
Neuron::Neuron(Warstwa* warstwa, double bias): warstwa(warstwa), bias(bias) { czyUzywacBias=true; }

void Neuron::setWarstwa(Warstwa* warstwa) { this->warstwa=warstwa; }

void Neuron::generujOdpowiedzSigmoidalnie()
{
    double a=0;
    int i;
    if (czyUzywacBias) a+=bias.waga;
    for (i=0;i<wejscia.size();i++)
    {
        a+=(wejscia[i].waga*wejscia[i].wejscie);
    }
    odpowiedz=1.0/(1.0+exp(-a));
}
void Neuron::generujOdpowiedzTangensoidalnie()
{
    double a=0;
    int i;
    if (czyUzywacBias) a+=bias.waga;
    for (i=0;i<wejscia.size();i++)
    {
        a+=(wejscia[i].waga*wejscia[i].wejscie);
    }
    odpowiedz=tanh(a);
}
void Neuron::generujOdpowiedzLiniowo()
{
    double a=0;
    int i;
    if (czyUzywacBias) a+=bias.waga;
    for (i=0;i<wejscia.size();i++)
    {
        a+=(wejscia[i].waga*wejscia[i].wejscie);
    }
    odpowiedz=a;
}
void Neuron::generujOdpowiedzPrzekazujac()
{
    if (wejscia.size()!=1) throw std::string("Ilość wejść przy neuronie przekazującym nie może być inna niż 1");
    odpowiedz=wejscia[0].wejscie;
}
void Neuron::generujBladWstecznejPropagacjiDlaSigmoidy()
{
    int i;
    std::vector<Neuron>* neuronyWyzej;
    Warstwa* nastepnaWarstwa;
    try
    {
        nastepnaWarstwa=warstwa->getNastepnaWarstwa();
    }
    catch(std::string exception) //Przypadek dla ostatniej warstwy
    {
        bladWstecznejPropagacji=odpowiedz*(1.0-odpowiedz)*(oczekiwana-odpowiedz);
        return;
    }
    
    bladWstecznejPropagacji=0;
    neuronyWyzej=warstwa->getNastepnaWarstwa()->getNeurony();
    for (i=0;i<neuronyWyzej->size();i++) //Sumowanie wag neuronow wyzej
    {
        bladWstecznejPropagacji=bladWstecznejPropagacji+(neuronyWyzej->at(i).getBladWstecznejPropagacji()*neuronyWyzej->at(i).getWagaWejscia(warstwa->getIdNeuronu(this)));
    }
    bladWstecznejPropagacji=bladWstecznejPropagacji*(odpowiedz*(1.0-odpowiedz));
}
void Neuron::generujBladWstecznejPropagacjiDlaTangensoidy()
{
    int i;
    std::vector<Neuron>* neuronyWyzej;
    Warstwa* nastepnaWarstwa;
    try
    {
        nastepnaWarstwa=warstwa->getNastepnaWarstwa();
    }
    catch(std::string exception) //Przypadek dla ostatniej warstwy
    {
        bladWstecznejPropagacji=(1.0-(odpowiedz*odpowiedz))*(oczekiwana-odpowiedz);
        return;
    }
    
    bladWstecznejPropagacji=0;
    neuronyWyzej=warstwa->getNastepnaWarstwa()->getNeurony();
    for (i=0;i<neuronyWyzej->size();i++) //Sumowanie wag neuronow wyzej
    {
        bladWstecznejPropagacji=bladWstecznejPropagacji+(neuronyWyzej->at(i).getBladWstecznejPropagacji()*neuronyWyzej->at(i).getWagaWejscia(warstwa->getIdNeuronu(this)));
    }
    bladWstecznejPropagacji=bladWstecznejPropagacji*(1.0-(odpowiedz*odpowiedz));
}
void Neuron::generujBladWstecznejPropagacjiDlaLini()
{
    int i;
    std::vector<Neuron>* neuronyWyzej;
    Warstwa* nastepnaWarstwa;
    try
    {
        nastepnaWarstwa=warstwa->getNastepnaWarstwa();
    }
    catch(std::string exception) //Przypadek dla ostatniej warstwy
    {
        bladWstecznejPropagacji=1.0*(oczekiwana-odpowiedz);
        return;
    }
    
    bladWstecznejPropagacji=0;
    neuronyWyzej=warstwa->getNastepnaWarstwa()->getNeurony();
    for (i=0;i<neuronyWyzej->size();i++) //Sumowanie wag neuronow wyzej
    {
        bladWstecznejPropagacji=bladWstecznejPropagacji+(neuronyWyzej->at(i).getBladWstecznejPropagacji()*neuronyWyzej->at(i).getWagaWejscia(warstwa->getIdNeuronu(this)));
    }
    bladWstecznejPropagacji=bladWstecznejPropagacji*1.0;
}
void Neuron::nauczWstecznaPropagacjaBledow(unsigned int trybOdpowiedzi)
{
    int i;
    if (trybOdpowiedzi==SIGMOIDALNIE) generujBladWstecznejPropagacjiDlaSigmoidy();
    else if (trybOdpowiedzi==TANGENSOIDALNIE) generujBladWstecznejPropagacjiDlaTangensoidy();
    else if (trybOdpowiedzi==LINIOWO) generujBladWstecznejPropagacjiDlaLini();
    if (czyUzywacBias)
    {
        bias.poprzedniWspolczynnikBledu=(bias.poprzedniWspolczynnikBledu*wspolczynnikMomentum)+(wspolczynnikNauki*bladWstecznejPropagacji);
        bias.waga+=bias.poprzedniWspolczynnikBledu;
    }
    for (i=0;i<wejscia.size();i++)
    {
        wejscia[i].poprzedniWspolczynnikBledu=(wejscia[i].poprzedniWspolczynnikBledu*wspolczynnikMomentum)+(wspolczynnikNauki*bladWstecznejPropagacji*wejscia[i].wejscie);
        wejscia[i].waga+=wejscia[i].poprzedniWspolczynnikBledu;
    }
}
void Neuron::przekazOdpowiedz()
{
    int i;
    std::vector<Neuron>* neuronyWyzej;
    Warstwa* nastepnaWarstwa;
    try
    {
        nastepnaWarstwa=warstwa->getNastepnaWarstwa();
    }
    catch(std::string exception) //Przypadek dla ostatniej warstwy
    {
        warstwa->getInterface()->showComunicat("Ostatnia warstwa nie ma komu przekazać odpowiedzi!");
        return;
    }
    
    neuronyWyzej=nastepnaWarstwa->getNeurony();
    for (i=0;i<neuronyWyzej->size();i++)
    {
        (*neuronyWyzej)[i].setWejscieW(warstwa->getIdNeuronu(this), odpowiedz);
    }
}

void Neuron::setOczekiwana(double oczekiwana) { this->oczekiwana=oczekiwana; }
void Neuron::setWspolczynnikNauki(double wspolczynnikNauki) { this->wspolczynnikNauki=wspolczynnikNauki; }
void Neuron::setWspolczynnikMomentum(double wspolczynnikMomentum) { this->wspolczynnikMomentum=wspolczynnikMomentum; }

double Neuron::getBladWstecznejPropagacji() { return bladWstecznejPropagacji; }
double Neuron::getOczekiwana() { return oczekiwana; }
double Neuron::getOdpowiedz() { return odpowiedz; }

void Neuron::pushWejscie(Wejscie wejscie) { wejscia.push_back(wejscie); }
void Neuron::eraseWejscie(int idWejscia) { wejscia.erase(wejscia.begin()+idWejscia); }
void Neuron::setWejscieW(int idWejscia, double wejscie) { wejscia[idWejscia].wejscie=wejscie; }
void Neuron::setWejscie(int idWejscia, Wejscie wejscie) { wejscia[idWejscia]=wejscie; }
double Neuron::getWagaWejscia(int idWejscia) { return wejscia[idWejscia].waga; }

std::istream& operator>>(std::istream& is, Neuron& obj)
{
    std::vector<std::stringstream> inputLines;
    std::string line;
    std::pair<unsigned int, unsigned int> linieNeuronu, linieWejsc;
    std::string tymczasoweWejscie[3];
    Wejscie tmp;
    unsigned int i,j;
    
    while (std::getline(is, line)) inputLines.push_back(std::stringstream(line));
    for (i=0;i<inputLines.size();i++)
    {
        if (inputLines[i].str().compare("\tNeuron")==0) linieNeuronu.first=i;
        if (inputLines[i].str().compare("\t\tWejscia")==0) linieWejsc.first=i;
        if (inputLines[i].str().compare("\t\t/Wejscia")==0) linieWejsc.second=i;
        if (inputLines[i].str().compare("\t/Neuron")==0) linieNeuronu.second=i;
    }
    if (((linieNeuronu.first+4)!=linieWejsc.first)||((linieNeuronu.second-5)!=linieWejsc.second)) std::string("Błąd wczytywania neuronu! Błędna ilość pól!");
    
    obj.oczekiwana=std::stold(inputLines[linieNeuronu.first+1].str());
    obj.odpowiedz=std::stold(inputLines[linieNeuronu.first+2].str());
    obj.bladWstecznejPropagacji=std::stold(inputLines[linieNeuronu.first+3].str());
    
    //WEJSCIA
    for (i=1;i<(linieWejsc.second-linieWejsc.first);i++)
    {
        j=0;
        while (std::getline(inputLines[linieWejsc.first+i], line,' '))
        {
            tymczasoweWejscie[j]=line;
            j++;
        }
        tmp.poprzedniWspolczynnikBledu=stod(tymczasoweWejscie[0]);
        tmp.waga=stold(tymczasoweWejscie[1]);
        tmp.wejscie=stold(tymczasoweWejscie[2]);
        obj.pushWejscie(tmp);
    }
    
    //BIAS
    j=0;
    while (std::getline(inputLines[linieWejsc.second+1], line,' '))
    {
        tymczasoweWejscie[j]=line;
        j++;
    }
    tmp.poprzedniWspolczynnikBledu=stod(tymczasoweWejscie[0]);
    tmp.waga=stold(tymczasoweWejscie[1]);
    tmp.wejscie=stold(tymczasoweWejscie[2]);
    obj.bias=tmp;
    //
    obj.czyUzywacBias=std::stoi(inputLines[linieWejsc.second+2].str()); //Taki myk, z tego co kojarzę, nie ma metody na konwersję string->bool, a bezsensem byłoby pisać nową, jeżeli można traktować to jako inta
    obj.wspolczynnikNauki=std::stold(inputLines[linieWejsc.second+3].str());
    obj.wspolczynnikMomentum=std::stold(inputLines[linieWejsc.second+4].str());
    return is;
}

std::ostream& operator<<(std::ostream& os, const Neuron& obj)
{
    std::stringstream output;
    unsigned int i;
    output << "Neuron" << "\n";
    output << "\t" << obj.oczekiwana << "\n";
    output << "\t" << obj.odpowiedz << "\n";
    output << "\t" << obj.bladWstecznejPropagacji << "\n";
    output << "\t" << "Wejscia" << "\n";
    for (i=0;i<obj.wejscia.size();i++)
        output << "\t" << "\t" << obj.wejscia[i].poprzedniWspolczynnikBledu << " " << obj.wejscia[i].waga << " "<< obj.wejscia[i].wejscie << "\n";
    output << "\t" << "/Wejscia" << "\n";
    output << "\t" << obj.bias.poprzedniWspolczynnikBledu << " " << obj.bias.waga << " "<< obj.bias.wejscie << "\n";
    output << "\t" << obj.czyUzywacBias << "\n";
    //Wskaznik warstwy podawany przy konstruktorze, warstwa sama sobie go doda
    output << "\t" << obj.wspolczynnikNauki << "\n";
    output << "\t" << obj.wspolczynnikMomentum << "\n";
    output << "/Neuron" << "\n";
    
    os << output.str();
    
    return os;
}
std::string& operator<<(std::string& s, const Neuron& obj)
{
    std::ostringstream out;
    out << obj;
    return s.append(out.str());
}
