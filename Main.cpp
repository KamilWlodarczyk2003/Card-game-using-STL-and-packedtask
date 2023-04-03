#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

const int LICZBA_KART = 52;
// '3'-czerwo '4'-kier '5'-trefl '6'-pik
// 0-K 1-A 2-2 ... 10-10 11-J 12-Q 
struct karta {
	char kolor;
	int figura;
};

int random_liczba() {//zwraca randomow¹ liczbe z przedzia³u 0 51
	//do uzupe³niena
	int liczba = rand() % 52;
	return liczba;
}

//ustawia rozgrywke 
void reset(bool* stos, karta* baza_kart, vector<karta>* karty_gr_1, vector<karta>* karty_gr_2) {
	for (int i = 0; i < LICZBA_KART; i++) {//tworzenie talii kart
		stos[i] = true;
		baza_kart[i].figura = i % 13;
		baza_kart[i].kolor = (char)(i / 13 + 3);
	}
	int random;
	for (int i = 0; i < 7; i++) {//rozdawanie pocz¹tkowych kart
		random = random_liczba();//najpierw losuje liczbe
		while (true) {

			if (random < 51 && random > 0) {
				random++;
			}
			if (stos[random]) {//sprawdza czy na stosie jest karta pod takim numerem
				(*karty_gr_1).push_back(baza_kart[random]);//je¿eli jest to daje t¹ karte ze stosu graczowi 1
				stos[random] = false;
				break;//robi to do skutku
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

//funkcja sprawdza czy poœród kart graczy jest jakaœ czwórka i je¿eli jest to zwaraca jakiej figury
int czy_sa_4(vector<karta>* karty) {
	int ile_jakich_kart[13];//w tej tablicy indeksy odpowiadaj¹ nie zdekodowanym figurom, funkcja zliczy tutaj ile jest jakich kart
	for (int i = 0; i < 13; i++) {
		ile_jakich_kart[i] = 0;
	}
	for (vector<karta>::iterator i = (*karty).begin(); i != (*karty).end(); i++)
	{
		ile_jakich_kart[(*i).figura]++;
		if (ile_jakich_kart[(*i).figura] == 4) {//je¿eli jakaœ iloœc figur bedzie równa 4 to usuwamy je z talii i zostaj¹ stosy 
			int ktora_karta = 0;
			for (vector<karta>::iterator j = (*karty).begin(); j != (*karty).end(); j++) {
				if ((*j).figura == (*i).figura) {
					(*karty).erase((*karty).begin() + ktora_karta, (*karty).begin() + ktora_karta + 1);//usuwa je ta abominacja
				}
			}
			return (*i).figura;
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

//rozgrywka jest w tej funkcji
void gra(bool* stos, karta* baza_kart, vector<karta>* karty_gr_1, vector<karta>* karty_gr_2) {
	vector<int> stosy_gr_1, stosy_gr_2;
	int temporary;
	while (true) {
		temporary = czy_sa_4(karty_gr_1);//zmienna musi przechowaæ t¹ wartoœc bo wywo³anie funkcji czy_sa_4 usuwa od razu stosy z kart graczy
		if (temporary >= 0) {
			stosy_gr_1.push_back(temporary);
		}
		temporary = czy_sa_4(karty_gr_2);//zmienna musi przechowaæ t¹ wartoœc bo wywo³anie funkcji czy_sa_4 usuwa od razu stosy z kart graczy
		if (temporary >= 0) {
			stosy_gr_2.push_back(temporary);
		}
		cout << "twoje stosy:";
		wypisz_stosy(&stosy_gr_1);
		cout << endl;
		cout << "twoje karty:";
		wypisz_karty(karty_gr_1);
		cout << endl;
		cout << "jak¹ kartê chcesz zarz¹daæ?\n2/3/4/../10 - liczba 0 - K\n1 - A\n";
	}


}

int main()
{
	bool stos[LICZBA_KART];
	karta baza_kart[LICZBA_KART];
	vector<karta> karty_gr_1, karty_gr_2;//talie kart gracza 1 i gracza 2
	reset(stos, baza_kart, &karty_gr_1, &karty_gr_2);
	gra(stos, baza_kart, &karty_gr_1, &karty_gr_2);

}