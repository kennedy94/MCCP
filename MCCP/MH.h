#pragma once
#include "clustering.h"
#include <vector>
#include <numeric>
#include <list>
#include <algorithm>
#include <set>

struct individuo {
	vector<int> centroides;
	vector<int> clientes_centroides;
	double fitness;
	individuo(vector<int> vetor, vector<int> vetor_c, double valor) {
		centroides = vetor;
		clientes_centroides = vetor_c;
		fitness = valor;
	}
	individuo() {
		centroides = vector<int>();
		clientes_centroides = vector<int>();
		fitness = NULL;
	}
	inline bool operator<(individuo a) {
		return (fitness < a.fitness);
	}

	inline bool operator==(individuo a) {
		return (fitness == a.fitness);
	}
};

struct arco {
	int i, j, custo;
	arco(int i, int j, int custo) {
		this->i = i;
		this->j = j;
		this->custo = custo;
	}
	inline bool operator<(arco a) {
		return (this->custo < a.custo);
	}
};

class MH :
	public Clustering
{
private:

	inline void f(individuo &ind);

	bool viavel(individuo ind);

	void GA_mutacao(individuo &ind);

	void GA_mutacao_forte(individuo & ind);

	individuo swap(individuo ind, int i1, int i2);

	individuo vizinhanca_swap(individuo ind);

	individuo opt2(individuo ind, int i1, int i2);

public:

	double Gamma;

	MH(const char* filename, double alpha) :
		Clustering(filename, alpha) {
	};

	individuo HEURISTICA_GULOSA();

	void MATHEURISTICA_HIBRIDA();

	~MH();
};


