#pragma once
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

void liczydlo(int *tablica, const int &size, const int &przedzial) {
	int *liczydelko1 = new int[przedzial];
	for (int i = 0; i < przedzial; i++)
		liczydelko1[i] = 0;

	int mini = 0, maxi = 0;
	for (int i = 0; i < size; i++) {
		if (tablica[i] < mini)
			mini = tablica[i];
		else if (tablica[i] > maxi)
			maxi = tablica[i];
	}

	for (int i = 0; i < size; i++)
		liczydelko1[tablica[i] - mini]++;
	for (int i = mini + 1; i <= maxi; i++)
		liczydelko1[i - mini] += liczydelko1[i - mini - 1];

	int *liczydelko2 = new int[size];
	for (int i = size - 1; i >= 0; i--)
		liczydelko2[(liczydelko1[tablica[i] - mini]--) - 1] = tablica[i];
	for (int i = 0; i < size; i++)
		tablica[i] = liczydelko2[i];

	delete[] liczydelko1; delete[] liczydelko2;
}

void kubelek(int *tablica, const int &size, const int &przedzial) {
	int maxi = tablica[0];
	for (int i = 0; i < size; i++) {
		if (tablica[i] > maxi)
			maxi = tablica[i];
	}

	const int kubelek_ile = 10;
	float kubelek_wielkosc = przedzial / kubelek_ile;

	vector<int> KFC[kubelek_ile];

	for (int i = 0; i < size; i++) {
		const double gdzie = tablica[i] / maxi;
		double k = 0;
		for (auto &item : KFC) {
			if ((k <= gdzie) && (gdzie < (k + kubelek_wielkosc)))
				item.emplace_back(tablica[i]);
			k += kubelek_wielkosc;
			if (k > przedzial)
				break;
		}
	}
	for (auto &item : KFC)
		sort(item.begin(), item.end());
	size_t j = 0;
	for (auto &item : KFC) {
		for (size_t i = 0; i < item.size(); i++)
			tablica[j++] = move(item[i]);
		item.clear();
	}
	KFC->clear();
}

template <typename typ>
void kubelek(typ *tablica, const int &size, const double &przedzial, const double (*klucz)(const typ &), const bool (*comparator)(const typ &, const typ &)) {
	typ maxi=tablica[0];
	for (int i = 0; i < size; i++) {
		if (!comparator(tablica[i], maxi))
			maxi = tablica[i];
	}

	const int kubelek_ile = 10;
	float kubelek_wielkosc = przedzial/kubelek_ile;

	vector<typ> KFC[kubelek_ile];

	for (int i = 0; i < size; i++) {
		const double gdzie = klucz(tablica[i]) / klucz(maxi);
		double k = 0;
		for (auto &item : KFC) {
			if ((k <= gdzie) && (gdzie < (k + kubelek_wielkosc)))
				item.emplace_back(tablica[i]);
			k += kubelek_wielkosc;
			if (k > przedzial)
				break;
		}
	}

	for (auto &item : KFC)
		sort(item.begin(), item.end(), comparator);
	size_t j = 0;
	for (auto &item : KFC) {
		for (size_t i = 0; i < item.size(); i++)
			tablica[j++] = move(item[i]);
		item.clear();
	}
	KFC->clear();
}
