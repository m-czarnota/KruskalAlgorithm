#pragma once
#include <iostream>
#include <algorithm>

struct Punkt {
	int m_x, m_y;

	Punkt(const int& x, const int& y) : m_x(x), m_y(y) {}
	~Punkt() {}
};

struct Krawedz {
	int m_index1, m_index2;
	double m_waga;

	Krawedz(const int& index1, const int& index2, const double& waga) :
		m_index1(index1), m_index2(index2), m_waga(waga) {}
	~Krawedz() {}
};
const bool Krawedz_comparator(const Krawedz& item1, const Krawedz& item2) {
	return item1.m_waga < item2.m_waga;
}

struct Graf {
	std::vector<Punkt> m_wspolrzedne;
	std::vector<Krawedz> m_krawedzie;

	Graf(const std::vector<Punkt>& wspolrzedne, const std::vector<Krawedz>& krawedzie)
		: m_wspolrzedne(wspolrzedne), m_krawedzie(krawedzie) {}
	~Graf() {
		//delete[] m_wspolrzedne, m_krawedzie; //mo¿e w przysz³oœci powodowaæ problemy
	}

	void sortKrawedzie(const bool(*comparator)(const Krawedz& item1, const Krawedz& item2)) {
		std::sort(m_krawedzie.begin(), m_krawedzie.end(), comparator);
	}
};