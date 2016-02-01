#ifndef POPULACJA_H
#define POPULACJA_H

#define METODA_TURNIEJOWA 1
#define METODA_RULETKI 2
#define METODA_POCZATKOWA 3
#define WIELKOSC_TURNIEJU 3

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include "chromosom.h"
using namespace std;

/*
*	Klasa definujê populacjê z³o¿ona
*  z osobników (Chromosomów) oraz
*  zapewnia metody obs³uguj¹ce algorytm
*  genetyczny (losowanie, krzy¿owanie,
*  mutowanie)
*/

class Populacja
{
	vector< Chromosom * > * osobniki;
	int rozmiar_populacji;
public:
	Populacja(int rozmiar);
	virtual ~Populacja();
	string toString();
	void ocen(float(*funk)(Chromosom*));
	void losuj_nowa_populacje(int metoda);
	void krzyzowanie(int prawd);
	void mutowanie(int prawd);
	Chromosom * get_wynik();
};

#endif /* POPULACJA_H */ 
