#include "populacja.h"
#include <ctime>


// konstruktor tworz¹cy populacjê na podstawie podanej jej liczebnosci
Populacja::Populacja(int rozmiar)
{
	this->rozmiar_populacji = rozmiar;
	this->osobniki = new vector<Chromosom *>();
	srand(time(NULL));
	int max_rozmiar = pow(2, ROZMIAR_CHROMOSOMU_BITOWO) - 1;		//2097152
	for (int i = 0; i < rozmiar; i++){
		osobniki->push_back(new Chromosom(rand() & (max_rozmiar)));
		//osobniki->push_back((rand() & (max_rozmiar)));
		//cout << "\n osobniki :" << osobniki;
		/*
		cout << "rand() : " << rand() << "\n";
		cout << "max_rozmiar : " << max_rozmiar << "\n";
		cout << ' ' << osobniki.at(i);
		for (unsigned int i = 0; i < this->osobniki->size(); i++){
			cout << this->osobniki->at(i)->get_kod() << endl;
		}
		*/
		//cout << (osobniki->at(i)) << "\n";

		//printListContent< Chromosom * > * osobniki;
	}
	/*int a = osobniki->size();
	for (int i = 0; i < a; i++){
		cout << &osobniki[i] <<endl;
	}*/
}

// destruktor usuwaj¹cy wszystkie osobniki z pamiêci
Populacja ::~Populacja()
{
	int rozmiar = osobniki->size();
	for (int i = 0; i < rozmiar; i++){
		delete &osobniki[i];
	}
	delete osobniki;
}


// metoda zwraca string zawieraj¹cy kody wszystkich chromosomów populacji
string Populacja::toString() {
	ostringstream ss;
	for (unsigned int i = 0; i < this->osobniki->size(); i++){
		ss << this->osobniki->at(i)->get_kod() << endl;
	}
	return ss.str();
}

// metoda ocenia wszystkie chromosomy za pomoc¹ podanego wskaŸnika do funkcji oceniaj¹cej
void Populacja::ocen(float(*funk)(Chromosom *)) {
	for (unsigned int i = 0; i < osobniki->size(); i++){
		osobniki->at(i)->set_ocena((*funk)(osobniki->at(i)));
	}
}

/* Losowanie populacji na trzy dostêpne sposoby:
* METODA_RULETKI
* METODA_TURNIEJOWA - wielkoœæ turnieju okreœlona w sta³ej w pliku nag³ówkowym
* METODA_POCZATKOWA - pierwsze losowanie wartoœci chromosomów
*/
void Populacja::losuj_nowa_populacje(int metoda) {
	srand(time(NULL));
	if (metoda == METODA_RULETKI){
		float *tab = new float[this ->rozmiar_populacji];
		//float tab[this ->rozmiar_populacji];
		double suma_ocen = 0;
		for (int i = 0; i < this->rozmiar_populacji; i++){
			tab[i] = osobniki->at(i)->get_ocena();
			suma_ocen += tab[i];
		}
		for (int i = 0; i < this->rozmiar_populacji; i++){
			tab[i] /= suma_ocen;
		}
		vector< Chromosom * > * osobniki2 = new vector<Chromosom *>();
		int ilosc_wybranych_osobnikow = 0;
		int los;
		while (ilosc_wybranych_osobnikow<rozmiar_populacji){
			suma_ocen = 0;
			los = rand() % 100;
			int i = 0;
			while (suma_ocen<los){
				suma_ocen += tab[i++];
			}
			if (i>0) i -= 1;
			if ((unsigned int)i >= osobniki->size()) i = osobniki->size() - 1;

			osobniki2->push_back(new Chromosom(osobniki->at(i)->get_kod()));
			ilosc_wybranych_osobnikow++;
		}
		delete osobniki;
		osobniki = osobniki2;
	}
	else if (metoda == METODA_TURNIEJOWA){
		vector< Chromosom * > * osobniki2 = new vector<Chromosom *>();
		int ilosc_wybranych_osobnikow = 0;
		while (ilosc_wybranych_osobnikow<rozmiar_populacji){
			Chromosom  * tmp;
			Chromosom  * max = new Chromosom();
			max->set_ocena(-RAND_MAX);
			for (int i = 0; i < WIELKOSC_TURNIEJU; i++){
				tmp = osobniki->at(rand() % rozmiar_populacji);

				if (tmp->get_ocena() > max->get_ocena()){
					max->set_ocena(tmp->get_ocena());
					max->set_kod(tmp->get_kod());
				}
			}
			/*
		rozne algorytmy selekcji
		moze porownac z czyms innym
			*/
			osobniki2->push_back(new Chromosom(max->get_kod()));
			ilosc_wybranych_osobnikow++;
			delete max;
		}
		delete osobniki;
		osobniki = osobniki2;
	}
	else if (metoda == METODA_POCZATKOWA){
		int max_kod = pow(2, ROZMIAR_CHROMOSOMU_BITOWO) - 1;		//2097151 
		for (int i = 0; i < this->rozmiar_populacji; i++){
			osobniki->at(i)->set_kod(rand() & max_kod);
		}
	}
	else{
		cerr << "Nie ma takiej metody w systemie\n";
		return;
	}
}

/*
* Krzy¿owanie przebiega w nastêpuj¹cy sposób (dla ka¿dych 2 kolejnych chromosomów) :
* losowana jest szansa na krzy¿owanie (wg. podanego prawd.)
* losowane jest miejsce w którym nast¹pi bitowe krzy¿owanie
* nowe wartoœci kodów przypisywane s¹ rodzicom
*/
void Populacja::krzyzowanie(int prawd) {
	int _prawd, poz;

	for (unsigned int j = 0; j < osobniki->size(); j += 2){
		_prawd = rand() % 100;
		if (_prawd < prawd){
			poz = rand() % ROZMIAR_CHROMOSOMU_BITOWO;
			int mask_a = 0, mask_b = 0;
			for (int i = ROZMIAR_CHROMOSOMU_BITOWO; i >poz; i--){
				mask_a |= (1 << i);
			}
			for (int i = poz; i >= 0; i--){
				mask_b |= (1 << i);
			}
			int wyn1 = (osobniki->at(j)->get_kod() & mask_a) + (osobniki->at(j + 1)->get_kod() & mask_b);
			//cout << "\n wynik1: " << wyn1;
			int wyn2 = (osobniki->at(j)->get_kod() & mask_b) + (osobniki->at(j + 1)->get_kod() & mask_a);
			//cout << "\n wynik2: " << wyn2;
			osobniki->at(j)->set_kod(wyn1);
			osobniki->at(j + 1)->set_kod(wyn2);
		}
	}
}

void Populacja::mutowanie(int prawd) {
	int _prawd;
	for (unsigned int j = 0; j < osobniki->size(); j += 1){
		_prawd = rand() % 100;
		if (_prawd < prawd){
			int pos = (rand() % ROZMIAR_CHROMOSOMU_BITOWO);
			osobniki->at(j)->set_kod((osobniki->at(j)->get_kod() ^ (1 << pos)));
		}
	}
}

// metoda zwraca chromosom z najwiêkszym wynikiem funkcji przystosowania z ca³ej populacji
Chromosom * Populacja::get_wynik() {
	Chromosom * max = new Chromosom(0);
	max->set_ocena(-65536);
	for (unsigned int i = 0; i < osobniki->size(); i++){
		if (max->get_ocena() < osobniki->at(i)->get_ocena()){
			max = osobniki->at(i);
		}
	}
	return max;
}
