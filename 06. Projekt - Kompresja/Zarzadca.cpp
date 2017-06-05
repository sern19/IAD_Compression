//
//  Zarzadca.cpp
//  04.Perceptron Wielowarstwowy
//
//  Created by Krystian Owoc on 17.04.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#include <time.h>
#include <stdlib.h>
#include <math.h>

#include <sstream>
#include <fstream>

#include "Zarzadca.hpp"

Zarzadca::Zarzadca(Interface* interfejs): interfejs(interfejs) {}
Zarzadca::Zarzadca(unsigned int numberOfNeuronsInHiddenLayer, Interface* interfejs, bool czyUzywacBias, double wspolczynnikMomentum, double wspolczynnikNauki): interfejs(interfejs)
{
    int i;
    std::stringstream tytul,tmp;
    srand((unsigned int)time(NULL));
    //Tu na stale przypisujemy ilosci warstw i neuronow
    
    warstwy.clear();
    
    //Warstwa 0 - 4 neurony przekazujace
    warstwy.push_back(Warstwa(this,PRZEKAZUJAC));
    //Warstwa 1 - 2 neurony sigmoidalne
    warstwy.push_back(Warstwa(this,SIGMOIDALNIE));
    //Warstwa 2 - 4 neurony sigmoidalne
    warstwy.push_back(Warstwa(this,SIGMOIDALNIE));
    
    //Ustawiamy na kazdej warstiwe, dla ktorej ma to znaczenie wspolczynniki nauki i momentum
    for (i=0;i<warstwy.size();i++)
    {
        warstwy[i].setWspolczynnikMomentum(wspolczynnikMomentum);
        warstwy[i].setWspolczynnikNauki(wspolczynnikNauki);
    }
    
    //Neurony dodajemy od najwyzszej warstwy, zeby mogly automatycznie polaczyc swoje wyjscia z wejsciami warstwy wyzszej
    for (i=0; i<4; i++) warstwy[2].pushNeuron(Neuron(&warstwy[2],czyUzywacBias));
    for (i=0; i<numberOfNeuronsInHiddenLayer; i++) warstwy[1].pushNeuron(Neuron(&warstwy[1],czyUzywacBias));
    for (i=0; i<4; i++) warstwy[0].pushNeuron(Neuron(&warstwy[0]));
    
    tytul << TYTUL("Transformacja",wspolczynnikNauki,wspolczynnikMomentum,czyUzywacBias,warstwy[1].getNeurony()->size());
    
//    wykres.pushwykres(GnuplotWykres(tytul.str(),"Iteracja","Błąd"));
//    wykres.getwykres(0)->pushdane(GnuplotDane("", "lines", "1"));
//    if (MANUALNY_ZAKRES_Y)
//    {
//        wykres.getwykres(0)->setyRange(std::make_pair(ZAKRES_Y_DOLNY,ZAKRES_Y_GORNY));
//        tmp << "ytics " << ZAKRES_Y_DOLNY << "," << (ZAKRES_Y_DOLNY+ZAKRES_Y_GORNY)/10.0 << "," << ZAKRES_Y_GORNY;
//        wykres.getwykres(0)->pushdodatkoweArgumenty(tmp.str());
//    }
//    wykres.pushdodatkoweArgumenty("samples 3000"); //Ilosc probek do wygladzania krzywej bledu
//    wykres.pushdodatkoweArgumenty("grid");
//    wykres.getwykres(0)->setxRange(std::make_pair(0, PO_ILU_ITERACJACH_PRZESUWAC_WYKRES));

}
Warstwa* Zarzadca::getNastepnaWarstwa(Warstwa *aktualnaWarstwa)
{
    int i;
    for (i=1;i<warstwy.size();i++)
    {
        if (&warstwy[i-1]==aktualnaWarstwa) return &warstwy[i];
    }
    throw std::string("Ostatnia warstwa nie ma kolejnej warstwy lub warstwa jest spoza wektoru warstw!");
    return NULL;
}
unsigned int Zarzadca::getIdWarstwy(Warstwa* warstwa)
{
    int i;
    for (i=0;i<warstwy.size();i++)
    {
        if (&warstwy[i]==warstwa) return i;
    }
    return NULL; //Nie ma prawa tu dojsc jezeli warstwa znajduje sie w tym zarzadcy, a tylko wtedy bedzie wywolywana ta metoda
}

void Zarzadca::glownaPetla(unsigned int iloscIteracji)
{
//    unsigned int i,wejscie,iloscOkresowMierzenia=0;
//    double maksymalnaWartoscBledu = 0.0;
//    int j;
//    for (i=0;i<iloscIteracji;i++)
//    {
//        //Zerowanie wejsc dla funkcji: sigmoida - 0, tangens hiperboliczny - -1
//        for (j=0;j<warstwy[0].getNeurony()->size();j++)
//        {
//            warstwy[0].getNeurony()->at(j).setWejscie(0, Wejscie(0,0));
//        }
//        //Ustalanie losowego wejscia warstwy 0
//        for (j=0;j<1;j++)
//        {
//            //do
//            //{
//            wejscie=(rand()%warstwy[0].getNeurony()->size());
//            //} while (warstwy[0].getNeurony()->at(wejscie).getWagaWejscia(0)==1);
//            warstwy[0].getNeurony()->at(wejscie).setWejscieW(0,1);
//        }
//        //Po kolei generowanie odpowiedzi warstwami
//        for (j=0;j<warstwy.size();j++) warstwy[j].generujOdpowiedzi();
//        //Ustalamy oczekiwane odpowiedzi ostatniej warstwie
//        for (j=0;j<warstwy.at(warstwy.size()-1).getNeurony()->size();j++) warstwy.at(warstwy.size()-1).getNeurony()->at(j).setOczekiwana(warstwy[0].getNeurony()->at(j).getOdpowiedz());
//        
//        //Uczymy neurony zaczynajac od konca
//        for (j=(int)(warstwy.size()-1);j>0;j--) warstwy[j].uczNeurony();
//        //Blad globalny
//        bladGlobalny=0;
//        for (j=0;j<warstwy.at(warstwy.size()-1).getNeurony()->size();j++)
//        {
//            bladGlobalny+=pow((warstwy.at(warstwy.size()-1).getNeurony()->at(j).getOczekiwana()-warstwy.at(warstwy.size()-1).getNeurony()->at(j).getOdpowiedz()),2);
//        }
//        bladGlobalny/=2;
//        if (bladGlobalny>maksymalnaWartoscBledu) maksymalnaWartoscBledu=bladGlobalny;
//        if (i%OKRES_MIERZENIA_BLEDU_GLOBALNEGO==0)
//        {
//            if (CZY_PRZERWAC_PO_OSIAGNIECIU_ZADANEGO_BLEDU) {
//                
//                if (maksymalnaWartoscBledu<ZADANY_BLAD) iloscOkresowMierzenia++;
//                else iloscOkresowMierzenia=0;
//            }
//            if (i>PO_ILU_ITERACJACH_PRZESUWAC_WYKRES)
//            {
//                wykres.getwykres(0)->setxRange(std::make_pair(-PO_ILU_ITERACJACH_PRZESUWAC_WYKRES+i, i));
//                wykres.getwykres(0)->getdane(0)->erasepunktDanych(0);
//            }
//            wykres.getwykres(0)->getdane(0)->pushpunktDanych(std::make_pair(i, maksymalnaWartoscBledu));
//            if (POKAZUJ_OKNO_GNUPLOTA) wykres.rysuj();
//            maksymalnaWartoscBledu=bladGlobalny;
//        }
//        if (CZY_PRZERWAC_PO_OSIAGNIECIU_ZADANEGO_BLEDU) if (iloscOkresowMierzenia>=PRZY_ILU_OKRESACH_MIERZENIA)
//        {
//            wykres.getwykres(0)->setxRange(std::make_pair(0, i));
//            if (POKAZUJ_OKNO_GNUPLOTA) wykres.rysuj();
//            break;
//        }
//    }
}

void Zarzadca::trybTestowania()
{
    int i,j;
    for (i=0;i<warstwy[0].getNeurony()->size();i++)
    {
        for (j=0;j<warstwy[0].getNeurony()->size();j++) //Zerowanie wejsc
        {
            warstwy[0].getNeurony()->at(j).setWejscie(0, Wejscie(0,0));
        }
        warstwy[0].getNeurony()->at(i).setWejscieW(0, 1);
        for (j=0;j<warstwy.size();j++) warstwy[j].generujOdpowiedzi();
        interfejs->showComunicat((
                                  std::stringstream() << "Podaję wejście na neuron numer " << i
                                  ).str());
        interfejs->showComunicat("Odpowiedz neuronow to: ");
        for (j=0;j<warstwy.at(warstwy.size()-1).getNeurony()->size();j++) interfejs->showText((
                                                                                                    std::stringstream() << warstwy.at(warstwy.size()-1).getNeurony()->at(j).getOdpowiedz() << " "
                                                                                                    ).str());
    }
    interfejs->waitToContinue();
}

Interface* Zarzadca::getInterface() { return interfejs; }

void Zarzadca::wczytajSiecZPliku(std::string filename)
{
    std::istringstream input;
    std::vector<std::stringstream> inputLines;
    std::ifstream inputFile(filename);
    std::string line;
    std::vector<std::pair<unsigned int, unsigned int>> linieWarstw;
    std::stringstream tmp;
    std::istringstream wejscieDoWarstwy;
    Warstwa tymczasowaWarstwa(this);
    int i,j=0;
    
    if (inputFile.is_open())
    {
        
        warstwy.clear();
        
        while (std::getline(inputFile, line)) inputLines.push_back(std::stringstream(line));
        for (i=0;i<inputLines.size();i++)
        {
            if (inputLines[i].str().compare("Warstwa")==0)
            {
                linieWarstw.push_back(std::make_pair(i, 0));
            }
            if (inputLines[i].str().compare("/Warstwa")==0)
            {
                linieWarstw[j].second=i;
                j++;
            }
        }
        for (i=0;i<linieWarstw.size();i++)
        {
            tymczasowaWarstwa=Warstwa(this);
            tmp.str("");
            for (j=linieWarstw[i].first;j<=linieWarstw[i].second;j++) tmp << inputLines[j].str() << "\n";
            wejscieDoWarstwy=std::istringstream(tmp.str());
            try
            {
                wejscieDoWarstwy >> tymczasowaWarstwa;
            } catch (std::string exception)
            {
                interfejs->showComunicat(exception);
                warstwy.clear();
                inputFile.close();
                return;
            }
            warstwy.push_back(tymczasowaWarstwa);
        }
        for (i=0;i<warstwy.size();i++) //Fix do problemu z wskażnikami po utworzeniu sieci
        {
            warstwy[i].setZarzadca(this);
            for (j=0;j<warstwy[i].getNeurony()->size();j++) warstwy[i].getNeurony()->at(j).setWarstwa(&warstwy[i]);
        }
        inputFile.close();
    }
    else interfejs->showComunicat("Nie można otworzyć pliku!");
}

void Zarzadca::zapiszSiecDoPliku(std::string filename)
{
    std::stringstream output;
    std::ofstream outputFile(filename);
    int i;
    
    if (outputFile.is_open())
    {
        for (i=0;i<warstwy.size();i++) output << warstwy[i];
        outputFile << output.str();
        outputFile.close();
    }
    else interfejs->showComunicat("Nie można otworzyć pliku!");
}
