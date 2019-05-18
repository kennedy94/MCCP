#pragma once
#include "clustering.h"
#include <vector>
#include <numeric>
#include <list>
#include <algorithm>

struct individuo {
	vector<int> cromossomo;
	double fitness;
	individuo(vector<int> vetor, double valor) {
		cromossomo = vetor;
		fitness = valor;
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

	int f(vector<int> solucao);
	bool viavel(vector<bool> DVAR_Y, vector<vector<bool>> DVAR_X);

	void GA_mutacao(vector<int> & solucao);

	vector<int> GA_crossover(vector<int> pai, vector<int> mae);


public:
	MH(const char* filename): Clustering(filename) {
	};

	void testes_unitarios() {
		vector<int> Y_zao = {1, 7, 12, 17, 24, 29, 30, 31, 33, 36, 37, 45};
		reverse(Y_zao.begin(), Y_zao.end());

		cout << f(Y_zao) << endl;
		
		getchar();


		/*srand(time(NULL));
		vector<individuo> Populacao = gerar_populacao_inicial(100);
		sort(Populacao.begin(), Populacao.end());
		
		for (int iter = 0; iter < 10000; iter++){
			int pai = rand() % Populacao.size(),
				mae = pai;
			while (pai == mae)
				mae = rand() % Populacao.size();

			vector <int> filho_vet = GA_crossover(Populacao[pai].cromossomo,
				Populacao[mae].cromossomo);
			
			if (rand() < 0.01)
				GA_mutacao(filho_vet);

			individuo filho(filho_vet, f(filho_vet));

			if (find(Populacao.begin(), Populacao.end(), filho) == Populacao.end()) {
				Populacao.push_back(filho);
				sort(Populacao.begin(), Populacao.end());
				Populacao = vector<individuo>(Populacao.begin(), Populacao.begin() + 100);
			}

		}

		cout << "melhor" <<  Populacao[0].fitness << endl;*/

	}

	void cuckoo_search();

	

	vector<individuo> gerar_populacao_inicial(int popu_size);

	~MH();
};

