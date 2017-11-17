//
//  GlobalDefines.h
//  03.Neuron Sigmoidalny
//
//  Created by Krystian Owoc on 05.04.2017.
//  Copyright © 2017 Krystian Owoc. All rights reserved.
//

#ifndef GlobalDefines_h
#define GlobalDefines_h

#define MAKSYMALNA_ILOSC_WATKOW_DO_UZYCIA 3 //Nie dawac mniej niz 1, to zepsuje program

//Tryby liczenia sum/srednich
#define MODE_R 0
#define MODE_G 1
#define MODE_B 2
#define MODE_GRAY 3

#define CZY_NORMALIZOWAC_PIXELE true

#define CZY_WYLICZAC_PSNR true

#define CO_ILE_PROCENT_WYSWIETLAC_STATUS 5 //Tyczy sie tylko Kohonena i gazu neuronowego

#define MAKSYMALNA_LICZBA_PROB_PRZELOSOWANIA 20 //UINT_MAX jezeli chcemy wylaczyc

#define INITIAL_LAMBDA 0.5
#define INITIAL_EPS 0.5

#endif /* GlobalDefines_h */
