#pragma once
#include <iostream>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

using namespace std;

bool is_nan() {
	if (cin.rdstate() != 0)
	{
		cin.clear();
		cin.ignore((2048), '\n');
		cout << "Niepoprawny rodzaj danych, wprowadz ponownie: ";
		return true;
	}
	return false;

}

template <typename typ>
struct Wezel {

	typ dane;

	Wezel *next;
	Wezel *prev;

	Wezel(const typ &item) : dane(item) {
		next = nullptr;
		prev = nullptr;

		//dane = item;
	}
	~Wezel() {}

	friend ostream& operator<<(ostream &outgoing, const Wezel &item) {
		outgoing << item.dane << endl;
		return outgoing;
	}

	void podmiana(const typ &item) {
		this->dane = item;
	}

	const typ &returnDane() {
		return this->dane;
	}
};

template <typename data>
class Lista {

	Wezel <data> *pocz;
	Wezel <data> *koniec;
	int size;

	//----------- sortowanie elementow listy ----------- 
	void list_sortBubble_podmiana(Wezel <data> *temp1, Wezel <data> *temp2) {
		Wezel <data> *temp1_prev = temp1->prev;

		temp1->next = temp2->next;
		temp1->prev = temp2;
		temp2->next = temp1;
		temp2->prev = temp1_prev;

		if (pocz != temp1 && koniec != temp2) {
			temp2->prev->next = temp2;
			temp1->next->prev = temp1;
		}
		if (temp2->prev == nullptr) {
			temp1->next->prev = temp1;
			pocz = temp2;
		}
		if (temp1->next == nullptr) {
			temp2->prev->next = temp2;
			koniec = temp1;
		}
	}

	void list_sortBubble(const bool(*comparator)(const data &, const data &)) {
		for (int i = list_size(); i > 0; i--) {
			bool isSort = true;
			for (int j = 0; j < i; j++) {
				Wezel <data> *temp1 = returnItem(j);
				Wezel <data> *temp2 = temp1->next;
				if (temp2) {
					if (comparator(temp1->dane, temp2->dane)) {
						list_sortBubble_podmiana(temp1, temp2);
						isSort = false;
					}
				}
				temp1 = nullptr;
				temp2 = nullptr;
			}
			if (isSort)
				return;
		}
	}

	//----------- metody pomocnicze -----------
	const bool out_range(const int &index) {
		if (index < 0) {
			cout << "Podany indeks jest poza zakresem listy. Wprowadz ponownie: ";
			return false;
		}
		Wezel <data> *temp = pocz;
		int i = 0;
		while (temp->next)
			++i;
		if (index > i) {
			cout << "Podany indeks jest poza zakresem listy. Wprowadz ponownie: ";
			return false;
		}
		return true;
	}
public:
	Wezel <data> *get_pocz() { return pocz; }
	void set_pocz(Wezel <data> *item) { pocz = item; }
	void set_koniec(Wezel <data> *item) { koniec = item; }
	void set_first(Wezel <data> *item) {
		set_pocz(item);
		set_koniec(item);
	}
private:
	const int &indexReturn(int &index) {
		assert((index < -1 || index >= list_size()));
		if (index == -1) {
			cout << "Podaj nr indeksu, w ktorym wstawic nowy element: ";
			do { cin >> index; } while (is_nan() || out_range(index));
		}
		return index;
	}

	Wezel <data> *whichItem(const int &index) {
		Wezel <data> *temp = pocz;
		if (index > (list_size() / 2)) {
			temp = koniec;
			for (int i = list_size(); i > index; i--)
				temp = temp->prev;
		}
		else {
			for (int i = 0; i < index; i++)
				temp = temp->next;
		}
		return temp;
	}

	Wezel <data> *returnItem(const int &index) {
		Wezel <data> *temp = whichItem(index);
		return temp;
	}

	void delete_last() {
		delete pocz;
		pocz = nullptr;
		koniec = nullptr;
	}

public:
	Lista() {
		size = 0;
		pocz = nullptr;
		koniec = nullptr;
	}
	Lista(Lista &&item) :
		pocz(move(item.pocz)), koniec(move(item.koniec)), size(move(item.size)) {}
	Lista(const Lista &item) :
		pocz(item.pocz), koniec(item.koniec), size(item.size) {}
	Lista &operator=(Lista &&item) {
		this->pocz = move(item.pocz);
		this->koniec = move(item.koniec);
		this->size = move(item.size);
		return *this;
	}
	Lista &operator=(const Lista &item) {
		this->pocz = item.pocz;
		this->koniec = item.koniec;
		this->size = item.size;
		return *this;
	}
	~Lista() {
		deleteAll();
	}

	const int list_size() {
		Wezel <data> *temp = pocz;
		int i = 0;
		while (temp) {
			temp = temp->next;
			++i;
		}
		temp = nullptr;
		return i;
	}

	const int &get_size() {
		return size;
	}

	//----------- metody interfejsu -----------
	data *&przedstawElement(const int &index = 0) {
		assert(!(index < 0 || index >= list_size()));
		Wezel <data> *temp = pocz;
		for (int i = 0; i < index; i++)
			temp = temp->next;
		return temp->dane;
	}

	void dodajBegin(const data &dane) {
		Wezel <data> *item = new Wezel <data>(dane);
		if (item) {
			if (!get_pocz()) {
				set_first(item);
			}
			else {
				pocz->prev = item;
				item->next = pocz;
				set_pocz(item);
			}
			item = nullptr;
		}
		++size;
	}

	void dodajEnd(const data &dane) {
		Wezel <data> *item = new Wezel <data>(dane);
		if (item) {
			if (!get_pocz()) {
				set_first(item);
			}
			else {
				koniec->next = item;
				item->prev = koniec;
				set_koniec(item);
			}
			item = nullptr;
		}
		++size;
	}

	void dodajCenter(const data &dane, int &index = -1) {
		if (list_empty()) {
			Wezel <data> *item = new Wezel <data>(dane);
			set_first(item);
		}
		else {
			index = indexReturn(index);
			Wezel <data> *temp = whichItem(index);
			if (index == 0)
				dodajBegin(dane);
			else if (index == list_size())
				dodajEnd(dane);
			else {
				Wezel <data> *item = new Wezel <data>(dane);
				temp->prev->next = item;
				item->next = temp;
				temp->prev = item;
			}
			temp = nullptr;
		}
		++size;
	}

	void dodaj_withOrder(const data &dane, const bool(*comparator)(const data &, const data &)) {
		dodajEnd(dane);
		list_sortBubble(comparator);
	}

	void dodaj(const data &dane, const bool(*comparator)(const data &, const data &)) {
		Wezel <data> *item = new Wezel <data>(dane);
		if (item) {
			if (!get_pocz()) {
				set_first(item);
			}
			else {
				koniec->next = item;
				item->prev = koniec;
				set_koniec(item);
				list_sortBubble(comparator);
			}
			item = nullptr;
		}
		++size;
	}

	void deleteBegin() {
		if (list_empty())
			return;
		if (list_size() > 1) {
			Wezel <data> *item = pocz;
			pocz = item->next;
			pocz->prev = nullptr;
			delete item;
			item = nullptr;
			--size;
		}
		else if (list_size() == 1)
			delete_last();
	}

	void deleteEnd() {
		if (list_empty())
			return;
		if (list_size() > 1) {
			Wezel <data> *item = koniec;
			koniec = item->prev;
			koniec->next = nullptr;
			delete item;
			item = nullptr;
			--size;
		}
		else if (list_size() == 1)
			delete_last();
	}

	void deleteCenter(const int &index = -1) {
		if (list_empty())
			return;
		if (list_size() > 1) {
			index = indexReturn(index);
			if (index == 0)
				deleteBegin();
			else if (index == (list_size() - 1))
				deleteEnd();
			else {
				Wezel <data> *temp = whichItem(index);
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
				delete temp;
				temp = nullptr;
				--size;
			}
		}
		else if (list_size() == 1)
			delete_last();
	}

	void deleteAll() {
		if (!list_empty()) {
			Wezel <data> *temp;
			while (pocz->next)
				deleteAll_help(temp);
			deleteAll_help(temp);
			temp = nullptr;
			pocz = nullptr;
			koniec = nullptr;
			--size;
		}
	}

	void deleteAll_help(Wezel<data> *&item) {
		item = pocz;
		pocz = pocz->next;
		delete item;
		--size;
	}

	bool list_empty(const int &option = 1) {
		if (!pocz) {
			if (option != 1)
				cout << "Lista jest obecnie pusta." << endl;
			return true;
		}
		return false;
	}

	void changeData(const data &dane, int index = -1) {
		index = indexReturn(index);
		Wezel <data> *temp = whichItem(index);
		temp->podmiana(dane);
		temp = nullptr;
	}

	Wezel <data> *findElement(const data &dane, const bool(*comparator)(const data &, const data &)) {
		Wezel <data> *temp = pocz;
		Wezel <data> *pierwszy = nullptr;
		while (temp) {
			if (comparator(dane, temp->dane)) {
				pierwszy = temp;
				break;
			}
			temp = temp->next;
		}
		temp = nullptr;
		return pierwszy;
	}

	const bool findElement_delete(const data &item, const bool(*comparator)(const data &, const data &)) {
		Wezel <data> *temp = pocz;
		bool deleted = false;
		auto czysciciel = [&]() {
			deleted = true;
			if (temp == pocz) {
				deleteBegin();
				temp = pocz;
			}
			else if (temp == koniec) {
				deleteEnd();
				temp = koniec;
			}
			else {
				temp->next->prev = temp->prev;
				temp->prev->next = temp->next;
				delete temp;
				temp = temp->next;
				--size;
			}
		};
		while (temp) {
			if (comparator(item, temp->dane)) {
				czysciciel();
				break;
			}
			temp = temp->next;
		}
		temp = nullptr;
		return deleted;
	}

	void shortNotation() {
		cout << endl;
		cout << "Ilosc elementow przechowywanych w liscie: " << list_size();
		Wezel <data> *temp = pocz;
		int size = 0;
		while (temp) {
			/*size+=sizeof(temp->dane->pole1);
			size+=sizeof(temp->dane->pole2);*/
			size += sizeof(temp->dane);
			size += sizeof(temp);
			temp = temp->next;
		}
		size += sizeof(this);
		cout << "; Ilosc zajmowanej pamieci przez liste: " << size << " B" << endl;
	}
};