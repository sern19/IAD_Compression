#ifndef Wejscie_hpp
#define Wejscie_hpp

struct Wejscie
{
	double waga;
	double wejscie;
    double poprzedniWspolczynnikBledu=0;
    Wejscie() {};
    Wejscie(double waga): waga(waga) {}
    Wejscie(double waga, double wejscie): waga(waga), wejscie(wejscie) {}
};

struct inputDataKlasyfikacja
{
    double input[4];
    int klasa;
};

struct inputDataAproksymacja
{
    double input;
    double output;
};


#endif
