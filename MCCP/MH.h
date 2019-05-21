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

class MH :
	public Clustering
{
private:

	inline void f(individuo &ind);

	bool viavel(individuo ind);

	void GA_mutacao(individuo &ind);

	void GA_mutacao_forte(individuo & ind);

	individuo GA_crossover_alternado(individuo pai, individuo mae);

	individuo gerar_solu_viavel();

	void crossover(vector<individuo> Populacao);

public:
	MH(const char* filename): Clustering(filename) {
	};

	void testes_unitarios() {
		srand(time(NULL));
		vector<individuo> Populacao = gerar_populacao_inicial(50);
		sort(Populacao.begin(), Populacao.end());

		cout << "melhor" << Populacao[0].fitness << endl;
		cout << viavel(Populacao[0]) << endl;
		cout << endl;
		
		for (int iter = 0; iter < 1000; iter++){
			crossover(Populacao);
			
			cout << Populacao[0].fitness << " viavel? " << viavel(Populacao[0]) << endl;
		}

		cout << "melhor" <<  Populacao[0].fitness << endl;
		cout << viavel(Populacao[0]) << endl;
		cout << endl;

	}

	void cuckoo_search();

	void ILS();

	individuo swap(individuo ind, int i1, int i2);
	individuo vizinhanca_swap(individuo ind);
	individuo opt2(individuo ind, int i1, int i2);
	vector<individuo> gerar_populacao_inicial(int popu_size);

	~MH();
};


