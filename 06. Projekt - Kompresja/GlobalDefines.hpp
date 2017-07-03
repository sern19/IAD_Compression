//
//  GlobalDefines.h
//  03.Neuron Sigmoidalny
//
//  Created by Krystian Owoc on 05.04.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef GlobalDefines_h
#define GlobalDefines_h

#define MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA 8 //Nie dawac mniej niz 1, to zepsuje program

//Tryby liczenia sum/srednich
#define MODE_R 0
#define MODE_G 1
#define MODE_B 2
#define MODE_GRAY 3





//Jakies stare
#define SCIEZKA_GNUPLOT_MAC "/usr/local/bin/gnuplot"
#define SCIEZKA_GNUPLOT_WIN "C://gnuplot//bin//gnuplot"

#define TERMINAL_MAC "wxt"
#define TERMINAL_WIN "wxt"

#define CZY_UZYWAC_WYJSCIA_PRZEZ_PLIK false //Przyspiesza zapis przy terminalu PNG, wxt ma problemy przy tym ustawieniu, nieznacznie przyspiesza aqua

#define WSTECZNA_PROPAGACJA_BLEDOW 0

#define PRZEKAZUJAC 0
#define SIGMOIDALNIE 1
#define TANGENSOIDALNIE 2
#define LINIOWO 3

#define TYTUL(wariant,x,y,z,neurony) wariant << ", Współczynnik nauki:" << x << ", Współczynnik momentum:" << y << ", Flaga Biasu:" << z << "\\nIlość neuronów w warstwie ukrytej:" << neurony

#define DOMYSLNA_WAGA_WEJSC ((double)rand()/(RAND_MAX)-0.5)
#define DOMYSLNY_WSPOLCZYNNIK_NAUKI (double)rand()/(RAND_MAX)
#define DOMYSLNY_WSPOLCZYNNIK_MOMENTUM (double)rand()/(RAND_MAX)

#define WSPOLCZYNNIK_NAUKI 0.00005
#define WSPOLCZYNNIK_MOMENTUM 0.6

#define ILOSC_ITERACJI 500000000

#define MANUALNY_ZAKRES_Y false //Przy true moze zdarzyc sie sytuacja, gdy blad wyjdzie poza zakres
#define ZAKRES_Y_DOLNY 0
#define ZAKRES_Y_GORNY 1

#define POKAZUJ_OKNO_GNUPLOTA false

#define CZY_PRZERWAC_PO_OSIAGNIECIU_ZADANEGO_BLEDU true
#define ZADANY_BLAD 0.005
#define PRZY_ILU_OKRESACH_MIERZENIA 5

#define PO_ILU_ITERACJACH_PRZESUWAC_WYKRES ILOSC_ITERACJI //Odkomentować to, jezeli nie chcemy, aby nasz wykres się przesowal
//#define PO_ILU_ITERACJACH_PRZESUWAC_WYKRES 75000 //A to zakomentowac

#define OKRES_MIERZENIA_BLEDU_GLOBALNEGO 50000 //5000000 //W iteracjach

#endif /* GlobalDefines_h */
