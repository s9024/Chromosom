#ifndef CHROMOSOM_H
#define CHROMOSOM_H

#define ROZMIAR_CHROMOSOMU_BITOWO 21
// dost�pny rozmiar chromosomu, max = rozmiar int bitowo
#include <sstream>
#include <iostream>
using namespace std;

/*
* Klasa reprezentuje Chromosom sk�adaj�cy sie z jego zapisu w systemie binarnym oraz
* oceny z funkcji przystosowania. Udost�pnia te� metody dost�pu do danych prywatnych.
*/
class Chromosom
{
	int kod;
	float ocena;
public:
	Chromosom(int kod);
	Chromosom();
	virtual ~Chromosom();
	string toString();
	void set_ocena(float o);
	void set_kod(int o);
	float get_ocena();
	int get_kod();
private:

};

#endif /* CHROMOSOM_H */ 
