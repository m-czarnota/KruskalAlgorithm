#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <time.h>
#include "reprezentanci.h"
#include "UnionFind.h"
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

int main()
{

	const int n = 3;
	for (size_t i = 1; i <= 3; i++) {
		std::vector<Punkt> wspolrzedne; //tutaj przechowywane wspó³rzêdne
		std::vector<Krawedz> krawedzie;
		std::fstream plik;
		std::string sciezka = "excercises\\g";
		sciezka += std::to_string(i);
		sciezka += ".txt";

		int ileLini = 0, ileZebrano = 0, jakieDane = -1;
		plik.open(sciezka, std::ios::in);
		if (plik.is_open()) {
			while (!plik.eof()) {
				std::string line;
				getline(plik, line);
				if (line == "") //to dlatego, ¿e na koñcu plików znajduje siê pusta linia, nie jest to koniec pliku, jednak std::stoi generuje b³¹d
					break;
				int dane1 = 0, dane2 = 0;
				if (!ileLini || ileZebrano == ileLini) { //czy ju¿ wie ile ma odczytaæ lini poni¿ej lub czy ju¿ zebra³o
					ileLini = std::stoi(line); //ile lini ma odczytaæ poni¿ej
					ileZebrano = 0; //ile zebra³o lini zerowanie

					++jakieDane; //okreœla czy ma zbieraæ wspó³rzêdne (0) czy grafy (1)
					continue; //dalej nic ma nie do odczytu w tej lini
				}

				std::string wyraz = ""; //tu zapisywane znalezione wyrazy
				for (auto& literka : line) {
					if (literka == ' ') {
						if (!jakieDane) { //do zbierania wspó³rzêdnych
							dane1 = std::stoi(wyraz);
							wyraz = "";
							continue;
						}
						else { //do zbierania krawêdzi wraz z kosztem
							if (!dane1)
								dane1 = std::stoi(wyraz);
							else if (dane1 && !dane2)
								dane2 = std::stoi(wyraz);
							wyraz = "";
							continue;
						}
					}
					wyraz += literka;
				}

				if (!jakieDane) { //do zbierania wspó³rzêdnych
					int index2 = std::stoi(wyraz);
					Punkt pkt(dane1, index2);
					wspolrzedne.emplace_back(pkt);
				}
				else { //do zbierania krawêdzi wraz z kosztem
					double dane3 = std::stod(wyraz);
					Krawedz krawedz(dane1, dane2, dane3);
					krawedzie.emplace_back(krawedz);
				}
				++ileZebrano; //ile lini ju¿ zebra³o
			}
			plik.close();
			//koniec operacji na pliku, ale dalsza czêœæ nie mo¿e siê staæ, jeœli plik nie zostanie poprawnie wczytany

			Graf nowyGraf(wspolrzedne, krawedzie);

			clock_t time1 = clock();
			nowyGraf.sortKrawedzie(Krawedz_comparator);
			clock_t time2 = clock();
			double timeOperation_sort = (time2 - time1) / (double)CLOCKS_PER_SEC;

			UnionFind unia(nowyGraf.m_krawedzie.size());
			size_t ilePrzejsc;
			std::vector<Krawedz> drzewoRozpinajace;

			//ZNAJDOWANIE BEZ KOMPRESJI ORAZ BEZ LACZENIA RANG
			time1 = clock();
			for (ilePrzejsc = 0; ilePrzejsc < nowyGraf.m_krawedzie.size(); ilePrzejsc++) {
				int zbior1 = unia.findReprezentantCollection(nowyGraf.m_krawedzie.operator[](ilePrzejsc).m_index1);
				int zbior2 = unia.findReprezentantCollection(nowyGraf.m_krawedzie.operator[](ilePrzejsc).m_index2);

				if (zbior1 != zbior2) {
					drzewoRozpinajace.push_back(nowyGraf.m_krawedzie.operator[](ilePrzejsc));
					unia.join2Collection(zbior1, zbior2);
				}
			}
			time2 = clock();
			double timeOperation_kruskal = (time2 - time1) / (double)CLOCKS_PER_SEC;
			double sumWeight = 0;
			for (auto& item : drzewoRozpinajace)
				sumWeight += item.m_waga;
			std::cout << "ZNAJDOWANIE BEZ KOMPRESJI ORAZ BEZ LACZENIA RANG" << std::endl;
			std::cout << "Ilosc krawedzi: " << drzewoRozpinajace.size() << ", suma ich wag: " << sumWeight << std::endl;
			std::cout << "Czas sortowania krawedzi: " << timeOperation_sort << " sek" << std::endl;
			std::cout << "Czas obliczen glownej petli algorytmu Kruskala: " << timeOperation_kruskal << " sek" << std::endl;
			std::cout << "Liczba wykonan operacji find: " << unia.m_ileFind << std::endl;
			std::cout << std::endl;
			drzewoRozpinajace.clear(); drzewoRozpinajace.shrink_to_fit();
			unia.deleteAll();

			//ZNAJDOWANIE Z KOMPRESJA ORAZ BEZ LACZENIA RANG
			time1 = clock();
			for (ilePrzejsc = 0; ilePrzejsc < nowyGraf.m_krawedzie.size(); ilePrzejsc++) {
				int zbior1 = unia.findReprezentantCollection_withCompress(nowyGraf.m_krawedzie.operator[](ilePrzejsc).m_index1);
				int zbior2 = unia.findReprezentantCollection_withCompress(nowyGraf.m_krawedzie.operator[](ilePrzejsc).m_index2);

				if (zbior1 != zbior2) {
					drzewoRozpinajace.push_back(nowyGraf.m_krawedzie.operator[](ilePrzejsc));
					unia.join2Collection(zbior1, zbior2);
				}
			}
			time2 = clock();
			timeOperation_kruskal = (time2 - time1) / (double)CLOCKS_PER_SEC;
			sumWeight = 0;
			for (auto& item : drzewoRozpinajace)
				sumWeight += item.m_waga;
			std::cout << "ZNAJDOWANIE Z KOMPRESJA ORAZ BEZ LACZENIA RANG" << std::endl;
			std::cout << "Ilosc krawedzi: " << drzewoRozpinajace.size() << ", suma ich wag: " << sumWeight << std::endl;
			std::cout << "Czas sortowania krawedzi: " << timeOperation_sort << " sek" << std::endl;
			std::cout << "Czas obliczen glownej petli algorytmu Kruskala: " << timeOperation_kruskal << " sek" << std::endl;
			std::cout << "Liczba wykonan operacji find: " << unia.m_ileFind << std::endl;
			std::cout << std::endl;
			drzewoRozpinajace.clear(); drzewoRozpinajace.shrink_to_fit();
			unia.deleteAll();

			//ZNAJDOWANIE BEZ KOMPRESJI ORAZ LACZENIE Z RANGI
			time1 = clock();
			for (ilePrzejsc = 0; ilePrzejsc < nowyGraf.m_krawedzie.size(); ilePrzejsc++) {
				int zbior1 = unia.findReprezentantCollection(nowyGraf.m_krawedzie.operator[](ilePrzejsc).m_index1);
				int zbior2 = unia.findReprezentantCollection(nowyGraf.m_krawedzie.operator[](ilePrzejsc).m_index2);

				if (zbior1 != zbior2) {
					drzewoRozpinajace.push_back(nowyGraf.m_krawedzie.operator[](ilePrzejsc));
					unia.join2Collection_byRank(zbior1, zbior2);
				}
			}
			time2 = clock();
			timeOperation_kruskal = (time2 - time1) / (double)CLOCKS_PER_SEC;
			sumWeight = 0;
			for (auto& item : drzewoRozpinajace)
				sumWeight += item.m_waga;
			std::cout << "ZNAJDOWANIE BEZ KOMPRESJI ORAZ LACZENIE Z RANGI" << std::endl;
			std::cout << "Ilosc krawedzi: " << drzewoRozpinajace.size() << ", suma ich wag: " << sumWeight << std::endl;
			std::cout << "Czas sortowania krawedzi: " << timeOperation_sort << " sek" << std::endl;
			std::cout << "Czas obliczen glownej petli algorytmu Kruskala: " << timeOperation_kruskal << " sek" << std::endl;
			std::cout << "Liczba wykonan operacji find: " << unia.m_ileFind << std::endl;
			std::cout << std::endl;
			drzewoRozpinajace.clear(); drzewoRozpinajace.shrink_to_fit();
			unia.deleteAll();

			//ZNAJDOWANIE Z KOMPRESJA ORAZ LACZENIE Z RANGAMI
			time1 = clock();
			for (ilePrzejsc = 0; ilePrzejsc < nowyGraf.m_krawedzie.size(); ilePrzejsc++) {
				int zbior1 = unia.findReprezentantCollection_withCompress(nowyGraf.m_krawedzie.operator[](ilePrzejsc).m_index1);
				int zbior2 = unia.findReprezentantCollection_withCompress(nowyGraf.m_krawedzie.operator[](ilePrzejsc).m_index2);

				if (zbior1 != zbior2) {
					drzewoRozpinajace.push_back(nowyGraf.m_krawedzie.operator[](ilePrzejsc));
					unia.join2Collection_byRank(zbior1, zbior2);
				}
			}
			time2 = clock();
			timeOperation_kruskal = (time2 - time1) / (double)CLOCKS_PER_SEC;
			sumWeight = 0;
			for (auto& item : drzewoRozpinajace)
				sumWeight += item.m_waga;
			std::cout << "ZNAJDOWANIE Z KOMPRESJA ORAZ LACZENIE Z RANGAMI" << std::endl;
			std::cout << "Ilosc krawedzi: " << drzewoRozpinajace.size() << ", suma ich wag: " << sumWeight << std::endl;
			std::cout << "Czas sortowania krawedzi: " << timeOperation_sort << " sek" << std::endl;
			std::cout << "Czas obliczen glownej petli algorytmu Kruskala: " << timeOperation_kruskal << " sek" << std::endl;
			std::cout << "Liczba wykonan operacji find: " << unia.m_ileFind << std::endl;
			std::cout << "-------------------" << std::endl;
			std::cout << std::endl;
			drzewoRozpinajace.clear(); drzewoRozpinajace.shrink_to_fit();
		}
		else
			std::cout << "Blad otwarcia pliku " << sciezka << std::endl;

		krawedzie.clear();
		wspolrzedne.clear();
	}

	return 0;
}
