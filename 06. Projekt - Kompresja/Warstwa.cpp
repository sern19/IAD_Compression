//
//  Warstwa.cpp
//  04.Perceptron Wielowarstwowy
//
//  Created by Krystian Owoc on 17.04.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include "Warstwa.hpp"
#include "Zarzadca.hpp"
#include "Neuron.hpp"
#include <stdlib.h>
#include <string>

Warstwa::Warstwa(Zarzadca* zarzadca, unsigned char trybOdpowiedzi, unsigned char trybNauki): zarzadca(zarzadca), trybNauki(trybNauki), trybOdpowiedzi(trybOdpowiedzi) {}

void Warstwa::setZarzadca(Zarzadca* zarzadca) { this->zarzadca=zarzadca; }

void Warstwa::generujOdpowiedzi()
{
    int i;
    if (trybOdpowiedzi==PRZEKAZUJAC)
    {
        for (i=0;i<neurony.size();i++)
        {
            try
            {
                neurony[i].generujOdpowiedzPrzekazujac();
            }
            catch (std::string exception)
            {
                zarzadca->getInterface()->showComunicat((
                                                         std::stringstream() << "Warstwa[" << zarzadca->getIdWarstwy(this) << "] Neuron[" << i << "]: " << exception
                                                         ).str());
                return;
            }
        }
    }
    else if (trybOdpowiedzi==SIGMOIDALNIE)
    {
        for (i=0;i<neurony.size();i++) neurony[i].generujOdpowiedzSigmoidalnie();
    }
    else if (trybOdpowiedzi==TANGENSOIDALNIE)
    {
        for (i=0;i<neurony.size();i++) neurony[i].generujOdpowiedzTangensoidalnie();
    }
    else if (trybOdpowiedzi==LINIOWO)
    {
        for (i=0;i<neurony.size();i++) neurony[i].generujOdpowiedzLiniowo();
    }

    try
    {
        if (zarzadca->getNastepnaWarstwa(this)!=NULL)
        {
            for (i=0;i<neurony.size();i++) neurony[i].przekazOdpowiedz();
        }
    }
    catch(std::string exception) {}
}
void Warstwa::uczNeurony()
{
    int i;
    if (trybNauki==WSTECZNA_PROPAGACJA_BLEDOW)
    {
        for (i=0;i<neurony.size();i++) neurony[i].nauczWstecznaPropagacjaBledow(trybOdpowiedzi);
    }
}

unsigned int Warstwa::getIdNeuronu(Neuron* neuron)
{
    int i;
    for (i=0;i<neurony.size();i++)
    {
        if (&neurony[i]==neuron) return i;
    }
    return NULL; //Nie ma prawa tu dojsc jezeli neuron znajduje sie w tej warstwie, a tylko wtedy bedzie wywolywana ta metoda
}
Warstwa* Warstwa::getNastepnaWarstwa()
{
    Warstwa* output=NULL;
    try
    {
        output=zarzadca->getNastepnaWarstwa(this);
    }
    catch(std::string exception)
    {
        throw exception;
    }
    return output;
}
std::vector<Neuron>* Warstwa::getNeurony() { return &neurony; }
Interface* Warstwa::getInterface() { return zarzadca->getInterface(); }

void Warstwa::setTrybNauki(unsigned char trybNauki) { this->trybNauki=trybNauki; }
void Warstwa::setTrybOdpowiedzi(unsigned char trybOdpowiedzi) { this->trybOdpowiedzi=trybOdpowiedzi; }

void Warstwa::setWspolczynnikNauki(double wspolczynnikNauki) { this->wspolczynnikNauki=wspolczynnikNauki; }
void Warstwa::setWspolczynnikMomentum(double wspolczynnikMomentum) { this->wspolczynnikMomentum=wspolczynnikMomentum; }

void Warstwa::pushNeuron(Neuron neuron)
{
    neurony.push_back(neuron);
    if (trybOdpowiedzi==PRZEKAZUJAC)
    {
        neurony.at(neurony.size()-1).pushWejscie(Wejscie(0,0));
    }
    else
    {
        neurony.at(neurony.size()-1).setWspolczynnikNauki(wspolczynnikNauki);
        neurony.at(neurony.size()-1).setWspolczynnikMomentum(wspolczynnikMomentum);
    }
    
    int i;
    std::vector<Neuron>* neuronyWyzej;
    Warstwa* nastepnaWarstwa;
    try
    {
        nastepnaWarstwa=getNastepnaWarstwa();
    }
    catch(std::string exception)
    {
        return;
    }
    
    if (nastepnaWarstwa!=NULL)
    {
        neuronyWyzej=nastepnaWarstwa->getNeurony();
        for (i=0;i<neuronyWyzej->size();i++)
        {
            (*neuronyWyzej)[i].pushWejscie(Wejscie(DOMYSLNA_WAGA_WEJSC,0));
        }
    }
}
void Warstwa::eraseNeuron(int idNeuronu)
{
    neurony.erase(neurony.begin()+idNeuronu);
    
    int i;
    std::vector<Neuron>* neuronyWyzej;
    Warstwa* nastepnaWarstwa;
    try
    {
        nastepnaWarstwa=getNastepnaWarstwa();;
    }
    catch(std::string exception)
    {
        return;
    }

    if (nastepnaWarstwa!=NULL)
    {
        neuronyWyzej=nastepnaWarstwa->getNeurony();
        for (i=0;i<neuronyWyzej->size();i++)
        {
            (*neuronyWyzej)[i].eraseWejscie(idNeuronu);
        }
    }
}
void Warstwa::setNeuron(int idNeuronu, Neuron neuron) { neurony[idNeuronu]=neuron; }

std::istream& operator>>(std::istream& is, Warstwa& obj)
{
    std::vector<std::stringstream> inputLines;
    std::vector<std::pair<unsigned int, unsigned int>> linieNeuronow;
    std::pair<unsigned int, unsigned int> linieWarstwy;
    std::string line;
    std::istringstream wejscieDoNeuronu;
    std::stringstream tmp;
    Neuron tymczasowyNeuron(&obj);
    unsigned int i,j=0;
    
    while (std::getline(is, line)) inputLines.push_back(std::stringstream(line));
    for (i=0;i<inputLines.size();i++)
    {
        if (inputLines[i].str().compare("Warstwa")==0) linieWarstwy.first=i;
        if (inputLines[i].str().compare("\tNeuron")==0)
        {
            linieNeuronow.push_back(std::make_pair(i, 0));
        }
        if (inputLines[i].str().compare("\t/Neuron")==0)
        {
            linieNeuronow[j].second=i;
            j++;
        }
        if (inputLines[i].str().compare("/Warstwa")==0) linieWarstwy.second=i;
    }
    if (((linieWarstwy.first+1)!=linieNeuronow[0].first)||((linieNeuronow.at(linieNeuronow.size()-1).second+5)!=linieWarstwy.second)) std::string("Błąd wczytywania neuronu! Błędna ilość pól!");
    
    //Neurony
    for (i=0;i<linieNeuronow.size();i++)
    {
        tymczasowyNeuron=Neuron(&obj);
        tmp=std::stringstream();
        for (j=linieNeuronow[i].first;j<=linieNeuronow[i].second;j++) tmp << inputLines[j].str() << "\n";
        wejscieDoNeuronu=std::istringstream(tmp.str());
        try
        {
            wejscieDoNeuronu >> tymczasowyNeuron;
        } catch (std::string exception)
        {
            obj.zarzadca->getInterface()->showComunicat(exception);
            return is;
        }
        obj.neurony.push_back(tymczasowyNeuron);
    }
    obj.trybNauki=std::stoi(inputLines.at(linieNeuronow.at(linieNeuronow.size()-1).second+1).str());
    obj.trybOdpowiedzi=std::stoi(inputLines.at(linieNeuronow.at(linieNeuronow.size()-1).second+2).str());
    obj.wspolczynnikNauki=std::stold(inputLines.at(linieNeuronow.at(linieNeuronow.size()-1).second+3).str());
    obj.wspolczynnikMomentum=std::stold(inputLines.at(linieNeuronow.at(linieNeuronow.size()-1).second+4).str());
    return is;
}

std::ostream& operator<<(std::ostream& os, const Warstwa& obj)
{
    std::stringstream output, tmp;
    std::string line;
    unsigned int i;
    output << "Warstwa" << "\n";
    for (i=0;i<obj.neurony.size();i++) tmp << obj.neurony[i];
    while (std::getline(tmp, line))
    {
        output << "\t" << line << "\n";
    }
    //Wskaznik zarzadcy podawany przy konstruktorze, zarzadca sam sobie go doda
    output << "\t" << obj.trybNauki << "\n";
    output << "\t" << obj.trybOdpowiedzi << "\n";
    output << "\t" << obj.wspolczynnikNauki << "\n";
    output << "\t" << obj.wspolczynnikMomentum << "\n";
    output << "/Warstwa" << "\n";
    
    os << output.str();
    
    return os;
}
std::string& operator<<(std::string& s, const Warstwa& obj)
{
    std::ostringstream out;
    out << obj;
    return s.append(out.str());
}
