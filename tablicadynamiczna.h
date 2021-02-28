#pragma once
#include <iostream>
#include <utility> 
#include <assert.h>

using namespace std;

template <typename typ>
class TablicaDynmiczna {
	int sizeActual;
	int sizeMax;
	typ *dane;

	void poszerzanie() {
		sizeMax *= 2;
		typ *temp = new typ[sizeMax];
		for (int i = 0; i < sizeActual; i++)
			temp[i] = move(dane[i]);
		delete[] dane;
		dane = temp;
		temp = nullptr;
	}

public:
	TablicaDynmiczna() {
		sizeMax = 1;
		sizeActual = 0;
		dane = new typ[sizeMax];
	}

	~TablicaDynmiczna() {
		sizeMax = 0;
		sizeActual = 0;
		if(dane)
			delete[] dane;
		dane = nullptr;
	}

	TablicaDynmiczna(TablicaDynmiczna &&item) {
		this->dane = move(item.dane);
		this->sizeActual = move(item.sizeActual);
		this->sizeMax = move(item.sizeMax);
		item.dane = nullptr;
	}
	TablicaDynmiczna(const TablicaDynmiczna &item) {
		for (int i = 0; i < item.sizeActual; i++) {
			this->dodaj(item.dane[i]);
		}
		this->sizeActual = item.sizeActual;
		this->sizeMax = item.sizeMax;
	}

	TablicaDynmiczna &operator=(TablicaDynmiczna &&item) {
		this->dane = move(item.dane);
		this->sizeActual = move(item.sizeActual);
		this->sizeMax = move(item.sizeMax);
		item.dane = nullptr;
		return *this;
	}
	TablicaDynmiczna &operator=(const TablicaDynmiczna &item) {
		for (int i = 0; i < item.sizeActual; i++) {
			this->dodaj(item.dane[i]);
		}
		this->sizeActual = item.sizeActual;
		this->sizeMax = item.sizeMax;
	}
	const typ &operator[](const int &index)const {
		assert(!(index < 0 && index >= sizeActual));
		return this->dane[index];
	}

	typ &operator[](int index) {
		assert(!(index < 0 && index >= sizeActual));
		return this->dane[index];
	}

	friend ostream &operator<<(ostream &outgoing, TablicaDynmiczna<typ> &item) {
		outgoing << "Elementy tablicy:" << endl;
		for (int i = 0; i < item.sizeActual; i++) {
			outgoing << "Indeks nr." << i << ", dane: ";
			outgoing << item.dane[i] << endl;
		}
		return outgoing;
	}

	friend ostream &operator<<(ostream &outgoing, TablicaDynmiczna<typ> *item) {
		outgoing << "Elementy tablicy:" << endl;
		for (int i = 0; i < item->sizeActual; i++) {
			outgoing << "Indeks nr." << i << ", dane: ";
			outgoing << item->dane[i] << endl;
		}
		return outgoing;
	}

	const int &get_size(const int &option=0) {
		if (option) sizeActual--;
		return sizeActual;
	}

	//------------------------- metody interfejsu -------------------------

	void dodaj(const typ &item) {
		if (sizeActual == sizeMax)
			poszerzanie();
		dane[sizeActual] = item;
		++sizeActual;
	}

	void podmiana(const typ &item, const int &index) {
		assert(!(index < 0 || index >= sizeActual));
		dane[index] = item;
	}

	void deleteAll() {
		delete[] dane;
		dane = nullptr;
		sizeMax = 1;
		dane = new typ[sizeMax];
		sizeActual = 0;
	}

	void shortNotation() {
		cout << "Ilosc elementow w tablicy: " << sizeActual << "; Wielosc tablicy: " << sizeMax << endl;
	}
};
