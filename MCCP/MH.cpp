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
			valor_fo += 100*max(capacidades_acumuladas[centroides[j]][k] - c[centroides[j]][k], 0);

	//penalizar por numero de clusters
	valor_fo += 100 * abs((int)ind.centroides.size() - p);

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
			ind.clientes_centroides[i] = clusters_fora[colocar];

	ind.centroides[retirar] = clusters_fora[colocar];
}

void MH::GA_mutacao_forte(individuo & ind) {
	vector<int> solucao = ind.centroides;

	set<int> clusters_fora;
	for (int j = 0; j < n; j++) {
		if (find(solucao.begin(), solucao.end(), j)
			== solucao.end())
			clusters_fora.insert(j);
	}

	int retirar = rand() % ind.centroides.size(),
		colocar = rand() % clusters_fora.size();

	vector<int> inserir;
	while (inserir.size() < solucao.size()){
		int colocar = rand() % clusters_fora.size();
		auto it = clusters_fora.begin();
		advance(it, colocar);
		inserir.push_back(*it);
		clusters_fora.erase(it);
	}
	
	for (int i = 0; i < m; i++){
		for (int j = 0; j < solucao.size(); j++){
			if (ind.clientes_centroides[i] == ind.centroides[j])
				ind.clientes_centroides[i] = inserir[j];
		}
	}
}

individuo MH::HEURISTICA_GULOSA() {

	vector<arco> ARCOS;

	//Ordenar arcos ordem crescente
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
			ARCOS.push_back(arco(i, j, d[i][j]));

	sort(ARCOS.begin(), ARCOS.end());
	//reverse(ARCOS.begin(), ARCOS.end());

	//pegar os p menores arcos e associar cluster como centroide
	int iterar = 0, it_a = 0;
	vector<int> centroides;
	while (iterar < p) {
		if (find(centroides.begin(), centroides.end(), ARCOS[it_a].j) == centroides.end()) {
			centroides.push_back(ARCOS[it_a].j);
			iterar++;
		}
		it_a++;
	}

	vector< vector <int> > capacidades_acumuladas(n);
	for (int j = 0; j < n; j++)
		capacidades_acumuladas[j] = vector<int>(K, 0);

	vector<arco> SOLUCAO;

	int contador = 0, j = 0;
	set<int> clientes_disponiveis;
	for (int i = 0; i < m; i++)
		clientes_disponiveis.insert(i);

	int soma_fo = 0;
	while (contador < m) {
		if (j == p)
			j = 0;


		int min = INT_MAX, min_i = -1;
		for (auto i : clientes_disponiveis) {
			bool cabe = true;
			for (int k = 0; k < K; k++) {
				if (capacidades_acumuladas[centroides[j]][k] + q[i][k] > c[centroides[j]][k]) {
					cabe = false;
					break;
				}
			}

			if (d[i][centroides[j]] < min && cabe) {
				min = d[i][centroides[j]];
				min_i = i;
			}
		}
		if (min_i != -1) {
			soma_fo += d[min_i][centroides[j]];
			SOLUCAO.push_back((arco(min_i, centroides[j], d[min_i][centroides[j]])));
			clientes_disponiveis.erase(min_i);

			for (int k = 0; k < K; k++)
				capacidades_acumuladas[centroides[j]][k] += q[min_i][k];

			contador++;

		}
		j++;
	}

	vector<int> clientes_centroides;
	for (int i = 0; i < m; i++) {
		for (int _aux = 0; _aux < m; _aux++) {
			if (SOLUCAO[_aux].i == i) {
				clientes_centroides.push_back(SOLUCAO[_aux].j);
			}
		}
	}
	set<int> distintos(clientes_centroides.begin(), clientes_centroides.end());
	return individuo(vector<int>(distintos.begin(), distintos.end()),
					clientes_centroides,
					soma_fo);
}

void MH::MATHEURISTICA_HIBRIDA() {

	individuo solucao = HEURISTICA_GULOSA();

	cout << viavel(solucao) << endl;

	vector<arco> ARCOS_HEURISTICA;

	//salvar os arcos presentes na solução da heurística
	for (int i = 0; i < m; i++)
		ARCOS_HEURISTICA.push_back(arco(i, solucao.clientes_centroides[i], d[i][solucao.clientes_centroides[i]]));

	sort(ARCOS_HEURISTICA.begin(), ARCOS_HEURISTICA.end());

	
	for (int i = 0; i < m; i++)
		ESTA_NO_MODELO[ARCOS_HEURISTICA[i].i][ARCOS_HEURISTICA[i].j] = true;

	montar_modelo();

	model.add(OBJETIVO <= solucao.fitness);
	for (int i = 0; i < Gamma*m; i++)
		model.add(x[ARCOS_HEURISTICA[i].i][ARCOS_HEURISTICA[i].j] == 1);

	
	resolver_inteira();
//	cplex.exportModel("modelo.lp");
}

individuo MH::swap(individuo ind, int i1, int i2){

	int aux = ind.clientes_centroides[i1];
	
	ind.clientes_centroides[i1] = ind.clientes_centroides[i2];
	ind.clientes_centroides[i2] = ind.clientes_centroides[i1];

	return ind;
}

individuo MH::opt2(individuo ind, int i1, int i2) {

	vector<int> newsolution = ind.clientes_centroides, 
		solution = ind.clientes_centroides;

	for (int c = 0; c <= i1 - 1; c++)
		newsolution[c] = solution[c];
	int d = 0;
	for (int c = i1; c <= i2; c++) {
		newsolution[c] = solution[i2 - d];
		d++;
	}

	for (int c = i2 + 1; c < newsolution.size(); c++)
		newsolution[c] = solution[c];
	

	return individuo(ind.centroides, newsolution, NULL);
}

individuo MH::vizinhanca_swap(individuo ind){

	
	f(ind);
	individuo melhor = ind;

	for (int i1 = 0; i1 < m; i1++){
		for (int i2 = 0; i2 < m; i2++){
			if (i1 != i2) {
				individuo aux = swap(ind, i1, i2);
				f(aux);
				if (melhor.fitness > aux.fitness) {
					melhor = aux;
				}
			}
		}
	}

	return melhor;
}

MH::~MH()
{
}
