// Autorzy: Kamil Włodarczyk i Kajetan Zawadka


#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <thread>
#include <future>
#include <random>

using namespace std;
const int LICZBA_KART = 52;
// '3'-czerwo '4'-kier '5'-trefl '6'-pik
// 0-K 1-A 2-2 ... 10-10 11-J 12-Q 




struct karta {
	char kolor;
	int figura;
};



int random_liczba() {//zwraca randomową liczbe z przedziału 0 51
	//do uzupełniena
	//srand(time(NULL));
	int liczba = rand() % 52;
	return liczba;
}

void tworzenie_talii(karta* baza_kart, bool* stos)
{
	for (int i = 0; i < LICZBA_KART; i++) {//tworzenie talii kart
		stos[i] = true;
		baza_kart[i].figura = i % 13;
		baza_kart[i].kolor = (char)(i / 13 + 3);
	}
}

void tasuj(karta* baza_kart)
{

	random_device rd;
	mt19937 gen(rd());
	shuffle(baza_kart, baza_kart + 52, gen);
}

//ustawia rozgrywke 
void reset(bool* stos, karta* baza_kart, vector<karta>* karty_gr_1, vector<karta>* karty_gr_2) {

	int random;
	for (int i = 0; i < 7; i++) {//rozdawanie początkowych kart
		random = random_liczba();										//najpierw losuje liczbe
		while (true) {

			if (random < 51 && random > 0) {
				random++;
			}
			if (stos[random]) {											//sprawdza czy na stosie jest karta pod takim numerem
				(*karty_gr_1).push_back(baza_kart[random]);				//jeżeli jest to daje tą karte ze stosu graczowi 1
				stos[random] = false;
				break;													
			}
		}
		random = random_liczba();
		while (true) {//analogicznie dla gracz 2

			if (random < 51 && random > 0) {
				random++;
			}


			if (stos[random]) {
				(*karty_gr_2).push_back(baza_kart[random]);
				stos[random] = false;
				break;
			}
		}

	}
}

//dekoduje figury z int na char np 0->K 1->A
char dekoder_figur(int x) {
	if (x < 2 || x>9) {
		switch (x) {
		case 0: {
			return 'K';
			break;
		}
		case 1: {
			return 'A';
			break;
		}
		case 11: {
			return 'J';
			break;
		}
		case 12: {
			return 'Q';
			break;
		}
		case 10: {
			cout << 1;
			return '0';
			break;
		}
		}
	}
	else {
		return (char)('0' + x);
	}

}

//funkcja sprawdza czy pośród kart graczy jest jakaś czwórka i jeżeli jest to zwaraca jakiej figury
int czy_sa_4(vector<karta>* karty) {
	int ile_jakich_kart[13];																					//w tej tablicy indeksy odpowiadają nie zdekodowanym figurom, funkcja zliczy tutaj ile jest jakich kart
	for (int i = 0; i < 13; i++) {
		ile_jakich_kart[i] = 0;
	}
	for (vector<karta>::iterator i = (*karty).begin(); i != (*karty).end(); i++)
	{
		ile_jakich_kart[(*i).figura]++;
		if (ile_jakich_kart[(*i).figura] == 4) {
			int it = 0, szukana_figura = (*i).figura;	//jeżeli jakaś ilośc figur bedzie równa 4 to usuwamy je z talii i zostają stosy 
			int skasowane_karty = 0;
			bool exit = false;
			while (true) {
				if (exit) {
					break;
				}
				int ilosc_kart = (*karty).size();
				vector<karta>::iterator j = (*karty).begin();
				it = 0;

				exit = false;
				while (true) {
					if (szukana_figura == (*j).figura) {
						(*karty).erase(j);
						skasowane_karty++;
						if (skasowane_karty == 4) {
							exit = true;
						}
						break;
					}
					it++;
					if (it >= ilosc_kart) {
						exit = true;
						break;
					}
					j++;
				}
			}

			return (*i).figura;
			break;
		}
	}
	return -1;
}

//wypisuje karty
void wypisz_karty(vector<karta>* karty) {
	for (vector<karta>::iterator i = (*karty).begin(); i != (*karty).end(); i++)
	{
		cout << " " << dekoder_figur((*i).figura) << " " << (*i).kolor << ", ";
	}
}

//wypisuje stosy
void wypisz_stosy(vector<int>* stosy) {
	for (vector<int>::iterator i = (*stosy).begin(); i != (*stosy).end(); i++)
	{
		cout << " " << dekoder_figur(*i) << ",";
	}
}

bool przekazanie_kart(int zarzadana_karta, vector<karta>* karty_biorcy, vector<karta>* karty_dawcy, bool* stos, karta* baza_kart, int ktory_gracz_jest_biorca) {
	int ktora_karta = 0;
	bool czy_biorca_ma_karte = false, czy_dawca_ma_karte = false;
	for (vector<karta>::iterator i = (*karty_biorcy).begin(); i != (*karty_biorcy).end(); i++)//				najpierw sprawdzamy czy wśród kart biorcy jest zarządana karta
	{
		if ((*i).figura == zarzadana_karta) {//to co się dzieje dalej robiłem o pólnocy po nieprzespanej nocy opisze to jeśli bedziesz potrzebować
			bool byla_liczba = false;
			int it = 0;
			czy_biorca_ma_karte = true;
			while (true) {
				int ilosc_kart_dawcy = (*karty_dawcy).size();
				if (byla_liczba) {
					break;
				}
				it = 0;
				byla_liczba = false;
				vector<karta>::iterator j = (*karty_dawcy).begin();
				while (true) {
					if ((*j).figura == zarzadana_karta) {
						(*karty_biorcy).push_back(*j);
						(*karty_dawcy).erase(j);
						czy_dawca_ma_karte = true;
						break;
					}
					it++;
					if (it >= ilosc_kart_dawcy) {
						byla_liczba = true;
						break;
					}
					j++;
				}
			}
		}
		if (czy_biorca_ma_karte) {
			break;
		}
	}

	bool na_stosie_nie_ma_kart = true;
	if (!czy_dawca_ma_karte && czy_biorca_ma_karte) {//			jeżeli dawca nie ma karty to biorca dostaje jedna karte ze stosu
		int temp;
		for (int i = 0; i < 52; i++) {
			if (stos[i]) {//									tutaj sprawdzamy czy pozostała jakakolwiek karta na stosie
				while (true) {//								jeżeli tak to losujemy ją do skutku
					temp = random_liczba() % 52;
					if (stos[temp]) {
						(*karty_biorcy).push_back(baza_kart[temp]);
						stos[temp] = false;
						na_stosie_nie_ma_kart = false;
						if (ktory_gracz_jest_biorca == 1) {
							cout << "\nPrzeciwnik nie posiada tej karty.\nOtrzymujesz wiec ze stosu karte:" << dekoder_figur(baza_kart[temp].figura) << " " << baza_kart[temp].kolor << endl;
						}
						if (ktory_gracz_jest_biorca == 2) {
							cout << "\nGracz nie posiada tej karty.\nOtrzymuje wiec ze stosu karte:" << baza_kart[temp].figura << " " << baza_kart[temp].kolor << endl;
						}

						break;
					}
				}

				break;
			}
		}

	}
	if (na_stosie_nie_ma_kart && ktory_gracz_jest_biorca == 1 && czy_biorca_ma_karte && !czy_dawca_ma_karte) {
		cout << "\nPrzeciwnik nie posiada tej karty.\nNa stosie nie ma juz zadnych kart.\n";
	}
	return czy_biorca_ma_karte;
}

//rozgrywka jest w tej funkcji
void gra(bool* stos, karta* baza_kart, vector<karta>* karty_gr_1, vector<karta>* karty_gr_2) {
	vector<int> stosy_gr_1, stosy_gr_2;
	int temporary, zadana_karta;
	while (true) {
		temporary = czy_sa_4(karty_gr_1);								//zmienna musi przechować tą wartośc bo wywołanie funkcji czy_sa_4 usuwa od razu stosy z kart graczy
		if (temporary >= 0) {
			stosy_gr_1.push_back(temporary);
		}
		temporary = czy_sa_4(karty_gr_2);								//zmienna musi przechować tą wartośc bo wywołanie funkcji czy_sa_4 usuwa od razu stosy z kart graczy
		if (temporary >= 0) {
			stosy_gr_2.push_back(temporary);
		}
		if (!(*karty_gr_1).size() || !(*karty_gr_2).size()) {
			system("CLS");
			if (stosy_gr_1.size() > stosy_gr_2.size())
			{
				cout << "KONIEC GRY" << endl << "WYGRALES" << endl;
			}
			if (stosy_gr_1.size() == stosy_gr_2.size())
			{
				cout << "KONIEC GRY" << endl << "REMIS" << endl;
			}
			if (stosy_gr_1.size() < stosy_gr_2.size())
			{
				cout << "KONIEC GRY" << endl << "Przegrales" << endl;
			}
			cout << "twoje punkty: " << stosy_gr_1.size() << endl << "punkty BOTa: " << stosy_gr_2.size() << endl;
			break;
		}
		temporary = czy_sa_4(karty_gr_1);								//zmienna musi przechować tą wartośc bo wywołanie funkcji czy_sa_4 usuwa od razu stosy z kart graczy
		if (temporary >= 0) {
			stosy_gr_1.push_back(temporary);
		}
		temporary = czy_sa_4(karty_gr_2);								//zmienna musi przechować tą wartośc bo wywołanie funkcji czy_sa_4 usuwa od razu stosy z kart graczy
		if (temporary >= 0) {
			stosy_gr_2.push_back(temporary);
		}
		cout << "twoje stosy:";
		wypisz_stosy(&stosy_gr_1);
		cout << endl;
		cout << "twoje karty:";
		wypisz_karty(karty_gr_1);
		cout << endl << endl;
		cout << "Bota stosy:";
		wypisz_stosy(&stosy_gr_2);
		cout << endl;
		cout << "karty przeciwnika:";
		wypisz_karty(karty_gr_2);
		cout << endl << endl;
		cout << "jaka karte chcesz zarzadac?\n2/3/4/../10 - liczba\n0 - K\n1 - A\n11 - J\n12 - Q\n\n";
		while (true) {
			cin >> zadana_karta;
			if (zadana_karta == 2137) {//	taki hak żeby wymusić wykrycie stosu w czy_sa_4
				(*karty_gr_1).push_back(baza_kart[7]);
				(*karty_gr_1).push_back(baza_kart[7]);
				(*karty_gr_1).push_back(baza_kart[7]);
				(*karty_gr_1).push_back(baza_kart[7]);
				break;
			}
			else {
				if (przekazanie_kart(zadana_karta, karty_gr_1, karty_gr_2, stos, baza_kart, 1)) {
					break;
				}

			}

			cout << "mozesz zarzadac tylko karty ze swojej puli\n";
		}

		temporary = czy_sa_4(karty_gr_1);								//zmienna musi przechować tą wartośc bo wywołanie funkcji czy_sa_4 usuwa od razu stosy z kart graczy
		if (temporary >= 0) {
			stosy_gr_1.push_back(temporary);
		}
		temporary = czy_sa_4(karty_gr_2);								//zmienna musi przechować tą wartośc bo wywołanie funkcji czy_sa_4 usuwa od razu stosy z kart graczy
		if (temporary >= 0) {
			stosy_gr_2.push_back(temporary);
		}

		int dlugosc_talii_2 = (*karty_gr_2).size();
		srand(time(0));
		int random = rand() % dlugosc_talii_2;
		if (dlugosc_talii_2 > 0)
		{
			int zadana_karta = (*karty_gr_2)[random].figura;
			przekazanie_kart(zadana_karta, karty_gr_2, karty_gr_1, stos, baza_kart, 2);
		}

	}


}

int main()
{

	bool stos[LICZBA_KART];
	karta baza_kart[LICZBA_KART];
	vector<karta> karty_gr_1, karty_gr_2;									//talie kart gracza 1 i gracza 2
	tworzenie_talii(baza_kart, stos);

	packaged_task<void(karta*)> task(tasuj);
	thread th(move(task), baza_kart);
	th.join();

	reset(stos, baza_kart, &karty_gr_1, &karty_gr_2);
	gra(stos, baza_kart, &karty_gr_1, &karty_gr_2);

}
