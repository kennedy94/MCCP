#include "MH.h"




int MH::f(vector<int> solucao)
{
	double valor_fo = 0.0;

	//vector<bool> DVAR_Y(n, false);
	//vector<vector<bool>> DVAR_X(m);
	//for (int i = 0; i < m; i++)
	//	DVAR_X[i] = vector<bool>(n, false);



	vector<vector<int>> capacidades_acumuladas;
	capacidades_acumuladas = vector<vector<int>>(n);
	for (int j = 0; j < n; j++)
		capacidades_acumuladas[j] = vector<int>(K, 0);

	for (int i = 0; i < m; i++) {
		int min = INT_MAX,
			cluster = -1;

		for (int j = 0; j < p; j++) {
			double soma = 0;
			bool ainda_cabe = true;
			for (int k = 0; k < K; k++) {
				if (capacidades_acumuladas[solucao[j]][k] + q[i][k] > c[solucao[j]][k])
					ainda_cabe = false;
			}
		
			if (d[i][solucao[j]] < min && ainda_cabe) {
				min = d[i][j];
				cluster = solucao[j];
			}
		}

		if (cluster == -1) {
			cerr << "Inviabilidade" << endl;
			system("pause");
			exit(0);
		}
		for (int k = 0; k < K; k++)
			capacidades_acumuladas[cluster][k] += q[i][k];

		//DVAR_X[i][cluster] = true;
		//DVAR_Y[cluster] = true;

		valor_fo += min;
	}
	//cout << viavel(DVAR_Y, DVAR_X) << endl;
	return valor_fo;
}

bool MH::viavel(vector<bool> DVAR_Y, vector<vector<bool>> DVAR_X)
{

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

void MH::GA_mutacao(vector<int> & solucao){
	vector<int> clusters_fora;
	for (int j = 0; j < n; j++){
		if (find(solucao.begin(), solucao.end(), j)
			== solucao.end())
			clusters_fora.push_back(j);
	}

	int retirar = rand() % p,
		colocar = rand() % (n - p);

	solucao[retirar] = clusters_fora[colocar];
	sort(solucao.begin(), solucao.end());
}

vector<int> MH::GA_crossover(vector<int> pai, vector<int> mae) {
	vector<int> filho,
				so_no_pai,
				so_na_mae,
				nos_dois;

	for (int j = 0; j < p; j++)	{
		if (find(mae.begin(), mae.end(), pai[j]) == mae.end())
			so_no_pai.push_back(pai[j]);
		else
			nos_dois.push_back(pai[j]);
		
		if (find(pai.begin(), pai.end(), mae[j]) == pai.end())
			so_na_mae.push_back(mae[j]);

	}

	filho = vector<int>(nos_dois.begin(), nos_dois.end());

	int contador = 0;
	for (int j1 = 0; j1 < p - nos_dois.size(); j1++) {
		if (j1 % 2)
			filho.push_back(so_no_pai[contador]);
		else {
			filho.push_back(so_na_mae[contador]);
			contador++;
		}
	}

	sort(filho.begin(), filho.end());
	return filho;
}


vector<individuo> MH::gerar_populacao_inicial(int popu_size)
{
	vector<individuo> Popu;

	vector<int> myvector;
	for (int j = 0; j < n; j++) myvector.push_back(j);
	

	for (int i = 0; i < m*n; i++){
		random_shuffle(myvector.begin(), myvector.end());
		vector<int> subset(myvector.begin(), myvector.begin() + p);
		sort(subset.begin(), subset.end());

		Popu.push_back(individuo(subset, f(subset)));
	}
	sort(Popu.begin(), Popu.end());
	Popu = vector<individuo>(Popu.begin(), Popu.begin() + popu_size);
	
	return Popu;
}

MH::~MH()
{
}
