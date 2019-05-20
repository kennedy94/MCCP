#include "MH.h"




inline void MH::f(individuo &ind)
{
	ind.fitness = 0;
	vector<int> centroides = ind.centroides,
				cent_clientes = ind.clientes_centroides;
	double valor_fo = 0.0;

	vector<vector<int>> capacidades_acumuladas;
	capacidades_acumuladas = vector<vector<int>>(n);
	for (int j = 0; j < n; j++)
		capacidades_acumuladas[j] = vector<int>(K, 0);

	for (int i = 0; i < m; i++) {
		valor_fo += d[i][cent_clientes[i]];

		for (int k = 0; k < K; k++)
			capacidades_acumuladas[cent_clientes[i]][k] += q[i][k];
	}
	

	//penalizar inviabilidade por capacidade
	for (int j = 0; j < ind.centroides.size(); j++)
		for (int k = 0; k < K; k++)		
			valor_fo += 1*max(capacidades_acumuladas[centroides[j]][k] - c[centroides[j]][k], 0);

	//penalizar por numero de clusters
	valor_fo += 1 * abs((int)ind.centroides.size() - p);

	ind.fitness = valor_fo;
}

bool MH::viavel(individuo ind)
{
	vector<bool> DVAR_Y(n);
	vector<vector<bool>> DVAR_X(m);
	for (int i = 0; i < m; i++)
		DVAR_X[i] = vector<bool>(n);



	for (int j = 0; j < ind.centroides.size(); j++)
		DVAR_Y[ind.centroides[j]] = true;
	for (int i = 0; i < m; i++)
		DVAR_X[i][ind.clientes_centroides[i]] = true;


	int i, j, w;
	int soma = 0;
	for (j = 0; j < n; j++)
		soma += DVAR_Y[j];
	if(soma != p) return false;
	soma = 0;

	for (j = 0; j < n; j++) {
		for (i = 0; i < m; i++)
			soma += DVAR_X[i][j];
		if (soma < DVAR_Y[j]) return false;
		soma = 0;
	}

	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++)
			soma += DVAR_X[i][j];
		if(soma != 1) return false;
		soma = 0;
	}
	for (j = 0; j < n; j++) {
		for (w = 0; w < K; w++) {
			for (i = 0; i < m; i++)
				soma += q[i][w] * DVAR_X[i][j];
			if(soma > c[j][w]) return false;
			soma = 0;
		}
	}

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			if (DVAR_X[i][j] - DVAR_Y[j] > 0) return false;

	return true;
}

void MH::GA_mutacao(individuo & ind){

	vector<int> solucao = ind.centroides;

	vector<int> clusters_fora;
	for (int j = 0; j < n; j++){
		if (find(solucao.begin(), solucao.end(), j)
			== solucao.end())
			clusters_fora.push_back(j);
	}

	int retirar = rand() % ind.centroides.size(),
		colocar = rand() % clusters_fora.size();


	for (int i = 0; i < m; i++)
		if (ind.clientes_centroides[i] == solucao[retirar])
			ind.clientes_centroides[i] == clusters_fora[colocar];

	ind.centroides[retirar] = clusters_fora[colocar];
}

individuo MH::GA_crossover_alternado(individuo pai, individuo mae) {
	vector<int> filho_clientes(m);

	for (int i = 0; i < m; i++)
		filho_clientes[i] = (i < m/2) ? pai.clientes_centroides[i] : mae.clientes_centroides[i];

	set<int> filho_cent(filho_clientes.begin(), filho_clientes.end());
	
	return individuo(vector<int>(filho_cent.begin(), filho_cent.end()), filho_clientes, NULL);
}


vector<individuo> MH::gerar_populacao_inicial(int popu_size)
{
	vector<individuo> Popu;

	for (int j = 0; j < popu_size; j++) {

		individuo ind = gerar_solu_viavel();
		f(ind);
		Popu.push_back(ind);
	}

	return Popu;
}

individuo MH::gerar_solu_viavel(){

	vector<vector<int>> capacidades_acumuladas;
	capacidades_acumuladas = vector<vector<int>>(n);
	for (int j = 0; j < n; j++)
		capacidades_acumuladas[j] = vector<int>(K, 0);

	vector<bool> cliente_alocado(m, false);

	vector<int> myvector;
	for (int j = 0; j < n; j++) myvector.push_back(j);

	random_shuffle(myvector.begin(), myvector.end());
	vector<int> subset(myvector.begin(), myvector.begin() + p);

	vector<int> clientes(m);

	for (int i = 0; i < m; i++) {
		int min = INT_MAX,
			cluster = -1;

		for (int j = 0; j < p; j++) {
			double soma = 0;
			bool ainda_cabe = true;
			for (int k = 0; k < K; k++) {
				if (capacidades_acumuladas[subset[j]][k] + q[i][k] > c[subset[j]][k])
					ainda_cabe = false;
			}

			if (d[i][subset[j]] < min && ainda_cabe) {
				min = d[i][j];
				cluster = subset[j];
			}
		}

		for (int k = 0; k < K; k++)
			capacidades_acumuladas[cluster][k] += q[i][k];

		clientes[i] = cluster;
	}

	return individuo(subset, clientes, NULL);
}

void MH::crossover(vector<individuo> Populacao){
	int tam_popu = Populacao.size();
	for (int i = 0; i < tam_popu; i++){
		for (int j = 0; j < tam_popu; j++) {
			if ((double)rand() / RAND_MAX < 0.3 && i != j) {
				individuo  filho = GA_crossover_alternado(Populacao[i], Populacao[j]);
				if ((double)rand() / RAND_MAX < 0.05)
					GA_mutacao(filho);

				f(filho);
				Populacao.push_back(filho);
			}
		}

	}

	sort(Populacao.begin(), Populacao.end());
	Populacao = vector<individuo>(Populacao.begin(), Populacao.begin() + tam_popu);
}



MH::~MH()
{
}
