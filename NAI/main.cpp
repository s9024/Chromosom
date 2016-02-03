#include <iostream>
#include "chromosom.h"
#include "populacja.h"
#include <math.h>
#include <limits.h>
#include<time.h>
#include <windows.h>

using namespace std;

// prototyp funkcji
void genetyczny(float(*funk)(Chromosom *), int prawd_krzyz, int prawd_mut, string nazwa_funkcji);
float func_ocen_onemax(Chromosom * c);
float func_ocen2(Chromosom * c);
float func_ocen(Chromosom * c);
float licz_wartosc(int kod);

int main() {
	genetyczny(func_ocen, 80, 10, "y=exp(-(8*x)^2/2)+(0.1*x)^2");
	genetyczny(func_ocen2, 80, 10, "y=-100 * (x^2 - x) ^2 - (1 - x)^2");
	genetyczny(func_ocen_onemax, 80, 10, "OneMax");
	system("PAUSE");
	return 0;
}

/* Funkcja realizuj¹ca algorytm genetyczny przyjmuje na wejœcie wskaŸnik do funkcij oceniaj¹cej, 
* prawdopodibieñstwa krzy¿owania i mutowania oraz nazwê/wzór funkcji w celu poprawnego jej identyfikowania
*/
void genetyczny(float(*funk)(Chromosom *), int prawd_krzyz, int prawd_mut, string nazwa_funkcji) {
	cout << "\n***  " << nazwa_funkcji << "  ***\n";
	const int rozmiar_populacji = 100;
	int ilosc_iteracji = 100;
	cout << "Rozmiar populacji : " << rozmiar_populacji << "\n";
	//cin >> rozmiar_populacji;
	cout << "Ilosc iteracji : " << ilosc_iteracji << "\n";;
	//cin >> ilosc_iteracji;
	Sleep(2000);
	Populacja * populacja = new Populacja(rozmiar_populacji);
	int wynik_posredni = -RAND_MAX;					//-32767
	int numer_iteracji_wyniku;
	populacja->losuj_nowa_populacje(METODA_POCZATKOWA);
	//cout << "funk :" << funk << "\n";
	populacja->ocen(funk);
	for (int i = 0; i < ilosc_iteracji; i++){
		//cout << "\n Nowa Iteracja ";
		populacja->losuj_nowa_populacje(METODA_TURNIEJOWA);
		populacja->krzyzowanie(prawd_krzyz);
		populacja->mutowanie(prawd_mut);
		populacja->ocen(funk);
		// sprawdzanie w której iteracji po raz pierwszy pojawi³ siê koñcowy wynik
		//cout << "\n\n populacja->get_wynik() :" << populacja->get_wynik() << "\n";
		//cout << "wynik_posredni :" << wynik_posredni << "\n";
		if (funk(populacja->get_wynik()) != wynik_posredni){
			wynik_posredni = funk(populacja->get_wynik());
			numer_iteracji_wyniku = i;
		}
	}
	cout << "obliczone maksimum: " << (int)funk(populacja->get_wynik()) << endl;
	//cout << "Wynik osi¹gnieto po " << numer_iteracji_wyniku << " iteracjach\n";
}
/*for(int i=0 i<tab.size();i++) cout<<tab[i]<<" "		wyœwietlamy zawartoœæ vectora*/

/*
*	Wzór chromosomu (21 bitów): 0 0000 00000000 00000000
* 	czlon 0 - znak +/-
* 	cz³on 1 - pozycja przecinka
*	czlon 2	- wartoœæ licznika
* 	cz³on 3 - wartoœæ mianownika
*	Przyk³ad :
* 	1 0101  10101110110 10110
* 	przecinek - 5 poz
* 	czlon_1 = 1398
* 	cz³on_2 = 22
* 	wynik = + 1398 / 22
*/

/* Funkcja generuje wartoœæ na podstawie powy¿szego wzoru chromosomów.
* Wykorzystywana do przetwarzania chromosomów reprezentuj¹cych liczby rzeczywiste.
*/
float licz_wartosc(int kod) {
	float x;
	//cout << "\n kod: " << kod;
	int poz_przecinka = (kod >> 16) & (int)(pow(2, 4) - 1);
	float licznik = (float)((kod >> poz_przecinka) & ((int)pow(2, 16 - poz_przecinka) - 1));
	float mianownik = (float)(kod & (int)(pow(2, (poz_przecinka)) - 1));
	//cout << "\npoz_przecinka: \t" <<poz_przecinka;
	//cout << "licznik: \t" << licznik;
	//cout << "\nmianownik: \t" << mianownik;

	if (mianownik == 0){
		//return 0;
		licznik = (int)licznik % 8;
		return licznik;
	}
	else{
		if (kod & (1 << (ROZMIAR_CHROMOSOMU_BITOWO - 1))){
			return licznik / mianownik;
		}
		else{
			return -licznik / mianownik;
		}
	}
	return x;
}

float func_ocen(Chromosom * c) {
	float x = licz_wartosc(c->get_kod());
	//cout << "\n x :\t" << x;
	if (x>8 || x< -8){ // przedzia³ funkcji od -8 do 8
		//cout << "\nINT_MIN\t" << INT_MIN;
		//return INT_MIN;
		return 0;		//test
	}
	float wyn = exp(-powf((8 * x), 2) / 2) + powf((0.1*x), 2);
	//cout << "\n func_ocen wynik: \t " << wyn;
	return wyn;
}

float func_ocen2(Chromosom * c) {
	float x = licz_wartosc(c->get_kod());
	float wyn = -100.0 * powf((powf(x, 2.0) - x), 2.0) - powf((1.0 - x), 2.0);
	//cout << "\n func_ocen2 wynik: " << wyn;
	return wyn;
}

float func_ocen_onemax(Chromosom * c) {
	int x = c->get_kod();
	return (x & ((long)(pow(2, ROZMIAR_CHROMOSOMU_BITOWO)) - 1));
}
