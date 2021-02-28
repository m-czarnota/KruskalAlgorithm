#pragma once
#include <iostream>
#include <utility> 
#include <assert.h>
#include <vector>
#include <string>
#include "lista.h"

using namespace std;

template <typename typ>
class TablicaMieszajaca {
public:
	struct ObjectData {
		std::string key;
		typ value;

		ObjectData(const std::string &t_key = 0, const typ &t_value = 0) :
			key(t_key), value(t_value) {}
		ObjectData(ObjectData &&item) :
			key(move(item.key)), value(move(item.value)) {}
		ObjectData(const ObjectData &item) :
			key(item.key), value(item.value) {}
		ObjectData &operator=(ObjectData &&item) {
			this->key = move(item.key);
			this->value = move(item.value);
			return *this;
		}
		ObjectData &operator=(ObjectData &item) {
			this->key = item.key;
			this->value = item.value;
			return *this;
		}
		ObjectData(ObjectData *item) :
			key(item->key), value(item->value) {}
		ObjectData &operator=(ObjectData *item) {
			this->key = item->key;
			this->value = item->value;
			return *this;
		}

		~ObjectData() {}

		friend ostream &operator<<(ostream &output, const ObjectData &item) {
			output << "(" << item.key;
			output << ", " << item.value << ")";
			return output;
		}
	};
private:
	int m_sizeActual;
	int m_sizeMax;
	const float m_fillingLimit;
	Lista<ObjectData*> *m_dane;

	void poszerzanie() {
		const int old_m_sizeMax = m_sizeMax;
		m_sizeMax *= 2;
		Lista<ObjectData*> *temp = new Lista<ObjectData*>[m_sizeMax];
		for (size_t i = 0; i < old_m_sizeMax; i++) {
			auto pointer = m_dane[i].get_pocz();
			while (pointer) {
				const unsigned long long index = wynikFunkcji(pointer->dane->key);
				//std::cout << "index=" << index << ", wielkosc listy: " << temp[index].list_size() << std::endl;
				temp[index].dodajEnd(pointer->dane);
				pointer = pointer->next;
			}
		}
		delete[] m_dane;
		m_dane = temp;
		temp = nullptr;
	}

	void listStats() {
		int lists_sizeMin = 1, lists_sizeMax = 0, lists_nonEmpty = 0, lists_sizeSum = 0;
		for (size_t i = 0; i < m_sizeMax; i++) {
			auto wskaznik = m_dane[i].get_pocz();
			if (wskaznik) {
				lists_nonEmpty++;
				int wskaznikSize = m_dane[i].list_size();
				lists_sizeSum += wskaznikSize;
				if (wskaznikSize < lists_sizeMin)
					lists_sizeMin = wskaznikSize;
				if (wskaznikSize > lists_sizeMax)
					lists_sizeMax = wskaznikSize;
			}
		}
		cout << "Statystyki list:" << endl;
		cout << "  Minimalna dlugosc listy: " << lists_sizeMin << endl;
		cout << "  Maksymalna dlugosc listy: " << lists_sizeMax << endl;
		cout << "  Ilosc niepustych list: " << lists_nonEmpty << endl;
		cout << "  Srednia dlugosc listy: " << (static_cast<float>(lists_sizeSum) / lists_nonEmpty) << endl;
	}

public:
	TablicaMieszajaca() : m_sizeActual(0), m_sizeMax(1), m_fillingLimit(0.75) {
		m_dane = new Lista<ObjectData*>[m_sizeMax];
	}

	~TablicaMieszajaca() {
		deleteAll();
		m_sizeMax = 0;
		m_sizeActual = 0;
		if (m_dane)
			delete[] m_dane;
		m_dane = nullptr;
	}

	TablicaMieszajaca(TablicaMieszajaca &&item) {
		this->m_dane = move(item.m_dane);
		this->m_sizeActual = move(item.m_sizeActual);
		this->m_sizeMax = move(item.m_sizeMax);
		item.m_dane = nullptr;
	}
	TablicaMieszajaca(const TablicaMieszajaca &item) {
		for (int i = 0; i < item.m_sizeActual; i++) {
			this->dodaj(item.m_dane[i]);
		}
		this->m_sizeActual = item.m_sizeActual;
		this->m_sizeMax = item.m_sizeMax;
	}

	TablicaMieszajaca &operator=(TablicaMieszajaca &&item) {
		this->m_dane = move(item.m_dane);
		this->m_sizeActual = move(item.m_sizeActual);
		this->m_sizeMax = move(item.m_sizeMax);
		item.m_dane = nullptr;
		return *this;
	}
	TablicaMieszajaca &operator=(const TablicaMieszajaca &item) {
		for (int i = 0; i < item.m_sizeActual; i++) {
			this->dodaj(item.m_dane[i]);
		}
		this->m_sizeActual = item.m_sizeActual;
		this->m_sizeMax = item.m_sizeMax;
		return *this;
	}
	const typ &operator[](const int &index)const {
		assert(!(index < 0 && index >= m_sizeActual));
		return this->m_dane[index];
	}

	friend ostream &operator<<(ostream &outgoing, TablicaMieszajaca<typ> &item) {
		outgoing << "---------------Tablica mieszajaca:---------------" << endl;
		outgoing << "  Obecny rozmiar: " << item.get_size() << endl;
		outgoing << "  Maksymalny rozmiar: " << item.get_sizeMax() << endl;

		/*outgoing << "  Elementy tablicy mieszajacej:" << endl;
		outgoing << "  {" << endl;
		int k = 0;
		for (int i = 0; i < item.m_sizeMax && k < 10; i++) {
			auto pointer = item.m_dane[i].get_pocz();
			bool istnieje = false;
			if (pointer)
				istnieje = true;

			while (pointer) {
				ObjectData object = pointer->returnDane();
				outgoing << "    " << i << ": " << object << "; ";
				pointer = pointer->next;
			}

			if (istnieje) {
				outgoing << endl;
				k++;
			}
		}
		outgoing << "  }" << endl;*/

		item.listStats();
		outgoing << "---------------Koniec---------------" << endl;
		return outgoing;
	}

	friend ostream &operator<<(ostream &outgoing, TablicaMieszajaca<typ> *item) {
		outgoing << "---------------Tablica mieszajaca:---------------" << endl;
		outgoing << "  Obecny rozmiar: " << item->get_size() << endl;
		outgoing << "  Maksymalny rozmiar: " << item->get_sizeMax() << endl;

		/*outgoing << "  Elementy tablicy mieszajacej:" << endl;
		outgoing << "  {" << endl;
		int k = 0;
		for (int i = 0; i < item->m_sizeMax && k<10; i++) {
			auto pointer = item->m_dane[i].get_pocz();
			bool istnieje = false;
			if (pointer)
				istnieje = true;

			while (pointer) {
				ObjectData object = pointer->returnDane();
				outgoing << "    " << i << ": " << object << "; ";
				pointer = pointer->next;
			}

			if (istnieje) {
				outgoing << endl;
				k++;
			}
		}
		outgoing << "  }" << endl;*/

		item->listStats();
		outgoing << "---------------Koniec---------------" << endl;
		return outgoing;
	}

	const int &get_size() {
		return m_sizeActual;
	}

	const int &get_sizeMax() {
		return m_sizeMax;
	}

	//------------------------- metody interfejsu -------------------------

	void dodaj(const std::string &item1, const typ &item2) {
		if ((m_sizeActual / static_cast<float>(m_sizeMax)) > m_fillingLimit)
			poszerzanie();
		const unsigned long long index = wynikFunkcji(item1);
		ObjectData *object = new ObjectData(item1, item2);

		auto pointer = m_dane[index].get_pocz();
		bool znaleziony = false;
		while (pointer) {
			if (pointer->dane->key.compare(item1) == 0) {
				delete pointer->dane;
				pointer->dane = object;
				znaleziony = true;
				break;
			}
			pointer = pointer->next;
		}

		if (!znaleziony) {
			m_dane[index].dodajEnd(object);
			++m_sizeActual;
		}
	}

	void deleteAll() {
		for (size_t i = 0; i < m_sizeMax; i++) {
			auto pointer = m_dane[i].get_pocz();
			while (pointer) {
				delete pointer->dane;
				pointer = pointer->next;
			}
		}
		delete[] m_dane;
		m_dane = nullptr;
		m_sizeMax = 1;
		m_dane = new Lista<ObjectData*>[m_sizeMax];
		m_sizeActual = 0;
	}

	const unsigned long long wynikFunkcji(const std::string &klucz) {
		const int baza = 31, klucz_dlugosc = klucz.length();
		int k = klucz_dlugosc - 1;
		unsigned long long suma = 0;
		for (int i = 0; i < klucz_dlugosc; i++, k--) {
			suma += klucz[i] * pow(31, k);
		}
		const unsigned long long wynik = suma % m_sizeMax;
		return wynik;
	}

	ObjectData *wyszukaj(const std::string &klucz) {
		const unsigned long long index = wynikFunkcji(klucz);
		auto pointer = m_dane[index].get_pocz();
		if (pointer) {
			ObjectData *object = pointer->dane;
			return object;
		}
		return nullptr;
	}

	const bool usun(const std::string &klucz) {
		const unsigned long long index = wynikFunkcji(klucz);
		auto pointer = m_dane[index].get_pocz();
		while (pointer) {
			if (pointer->dane->key == klucz) {
				auto pointer_prev = pointer->prev;
				auto pointer_next = pointer->next;
				if (pointer_prev)
					pointer_prev->next = pointer_next;
				if (pointer_next)
					pointer_next->prev = pointer_prev;
				delete pointer->dane; delete pointer; pointer = nullptr;
				pointer_prev = nullptr; pointer_next = nullptr;
				return true;
			}
			pointer = pointer->next;
		}
		return false;
	}
};