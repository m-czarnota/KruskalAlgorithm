#pragma once
#include <iostream>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

struct UnionFind {
private:
	int m_size;

public:
	int* m_rodzic, * m_ranga, m_ileFind;

	UnionFind(const int& n) :
		m_rodzic(new int[n]), m_ranga(new int[n]), m_ileFind(0), m_size(n)
	{
		for (size_t i = 0; i < m_size; i++) {
			m_rodzic[i] = i;
			m_ranga[i] = 0;
		}
	}
	~UnionFind() {
		delete[] m_rodzic, m_ranga;
	}

	void deleteAll() {
		delete[] m_rodzic, m_ranga;
		m_ileFind = 0;
		m_rodzic = new int[m_size];
		m_ranga = new int[m_size];
		for (size_t i = 0; i < m_size; i++) {
			m_rodzic[i] = i;
			m_ranga[i] = 0;
		}
	}

	void join2Collection(const int& index1, const int& index2) {
		m_rodzic[index1] = index2;
	}

	void join2Collection_byRank(const int& index1, const int& index2) {
		if (m_ranga[index1] < m_ranga[index2])
			m_rodzic[index1] = index2;
		else if (m_ranga[index2] < m_ranga[index1])
			m_rodzic[index2] = index1;
		else {
			m_rodzic[index1] = index2;
			m_ranga[index2]++;
		}
	}

	const int findReprezentantCollection(const int& index) {
		m_ileFind++;
		if (index == m_rodzic[index])
			return index;
		return findReprezentantCollection(m_rodzic[index]);
	}

	const int findReprezentantCollection_withCompress(const int& index) {
		m_ileFind++;
		if (index == m_rodzic[index])
			return index;
		m_rodzic[index] = findReprezentantCollection_withCompress(m_rodzic[index]);
		return m_rodzic[index];
	}
};