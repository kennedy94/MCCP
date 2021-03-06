#include "Modelo_GRAP_R.h"

void Modelo_GRAP_R::alocar_matrizes() {
	c = new int*[n];
	for (int i = 0; i < n; i++)
		c[i] = new int[K];
	q = new int*[m];
	for (int i = 0; i < m; i++)
		q[i] = new int[K];
	d = new double*[m];
	for (int i = 0; i < m; i++)
		d[i] = new double[n];
}


void Modelo_GRAP_R::cplexvar_initiate() {
	y = IloBoolVarArray(env, n);

	x = IloArray<IloBoolVarArray>(env, n*m);
	for (int i = 0; i < m; i++)
		x[i] = IloBoolVarArray(env, n);

	char strnum[30];
	for (int i = 0; i < n; i++) {
		sprintf(strnum, "y(%d)", i);
		y[i].setName(strnum);
	}

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			sprintf(strnum, "x(%d,%d)", i, j);
			x[i][j].setName(strnum);
		}
	}
}
void Modelo_GRAP_R::fo() {
	IloInt i, j;
	IloExpr costSum(env);
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			if (arcos_aceitos[i][j] == 1)
				costSum += d[i][j] * x[i][j];
	model.add(IloMinimize(env, costSum)).setName("FO");
	costSum.end();
}
void Modelo_GRAP_R::restricoes() {
	IloInt i, j, w;
	IloExpr expr(env);
	for (j = 0; j < n; j++)
		expr += y[j];
	model.add(expr == p);
	expr.clear();
	for (j = 0; j < n; j++) {
		for (i = 0; i < m; i++)
			if (arcos_aceitos[i][j] == 1)
				expr += x[i][j];
		model.add(expr >= y[j]);
		expr.clear();
	}
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++)
			if (arcos_aceitos[i][j] == 1)
				expr += x[i][j];
		model.add(expr == 1);
		expr.clear();
	}
	for (j = 0; j < n; j++) {
		for (w = 0; w < K; w++) {
			for (i = 0; i < m; i++)
				if (arcos_aceitos[i][j] == 1)
					expr += q[i][w] * x[i][j];
			model.add(expr <= c[j][w]);
			expr.clear();
		}
	}
	expr.end();
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			if (arcos_aceitos[i][j] == 1)
				model.add(x[i][j] - y[j] <= 0);

}

bool Modelo_GRAP_R::maior_que_alpha(int i, int j) {
	if (d[i][j] > maior_peso*alpha)
		return true;
	else
		return false;
}

void Modelo_GRAP_R::Definir_Arcos_SA()
{
	double	TEMPERATURA = (maior_peso - maior_peso*alpha),
			taxa_reducao = 0.9,
			referencia_d = maior_peso*alpha,
			DELTA;

	arcos_aceitos = vector<vector<bool>>(m);
	for (int i = 0; i < m; i++)
		arcos_aceitos[i] = vector<bool>(n);

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);

	uniform_real_distribution<double> distribution(0.0, 1.0);

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			DELTA = d[i][j] - referencia_d;
			if (distribution(generator) < exp(-DELTA / TEMPERATURA)) {
				arcos_aceitos[i][j] = 1;
			}
			else
			{
				arcos_aceitos[i][j] = 0;
			}
		}
	}
}

void Modelo_GRAP_R::Definir_Arcos_GRASP()
{
	arcos_aceitos = vector<vector<bool>>(m);
	for (int i = 0; i < m; i++)
		arcos_aceitos[i] = vector<bool>(n);

	

	int qtde_arcos = 0;

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (d[i][j] <= alpha*maior_peso){// + distribution(generator)*(maior_peso - alpha*maior_peso)){
				arcos_aceitos[i][j] = 1;
				qtde_arcos++;
			}
			else{
				arcos_aceitos[i][j] = 0;
			}
		}
	}
	
	for (int j = 0; j < n; j++)
	{
		bool tem_arco = false;
		int i_min, j_min, menor_arco = INT_MAX;
		for (int i = 0; i < m; i++)
		{
			if (arcos_aceitos[i][j] = 1) {
				tem_arco = true;
				break;
			}
			if (menor_arco > arcos_aceitos[i][j]) {
				menor_arco = arcos_aceitos[i][j];
				i_min = i;
				j_min = j;
			}

		}
		if (!tem_arco) {
			arcos_aceitos[i_min][j_min] = 1;
			qtde_arcos++;
		}

	}

	for (int i = 0; i < m; i++)
	{
		bool tem_arco = false;
		int i_min, j_min, menor_arco = INT_MAX;
		for (int j = 0; j < n; j++)
		{
			if (arcos_aceitos[i][j] = 1) {
				tem_arco = true;
				break;
			}
			if (menor_arco > arcos_aceitos[i][j]) {
				menor_arco = arcos_aceitos[i][j];
				i_min = i;
				j_min = j;
			}

		}
		if (!tem_arco) {
			arcos_aceitos[i_min][j_min] = 1;
			qtde_arcos++;
		}

	}

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);

	uniform_real_distribution<double> distribution(0.0, 1.0);
	int maiores_aceitos = 0;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (arcos_aceitos[i][j] == 0 &&
				d[i][j] <= alpha*maior_peso + distribution(generator)*(maior_peso - alpha*maior_peso)
				&& (double)maiores_aceitos/(m*n) <= alpha/2) {
				arcos_aceitos[i][j] = 1;
				qtde_arcos++;
				maiores_aceitos++;
			}
		}
	}


	densidade_arcos = (double)qtde_arcos/(m*n);
	resultados.open("resultados_MCCP.txt", fstream::app);
	resultados << "\t" << densidade_arcos;
	resultados.close();
}


Modelo_GRAP_R::Modelo_GRAP_R(const char* filename) {
	inst_name = filename;
	ifstream instancia(inst_name, ifstream::in);
	if (instancia.fail()) {
		cerr << "	Instancia nao encontrada" << endl;
		exit(1);
	}
	instancia >> m >> n >> p >> K;
	alocar_matrizes();

	for (int i = 0; i < m; i++)
		for (int j = 0; j < K; j++)
			instancia >> q[i][j];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < K; j++)
			instancia >> c[i][j];
	maior_peso = 0;
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++) {
			instancia >> d[i][j];
			if (d[i][j] > maior_peso)
				maior_peso = d[i][j];
		}

	instancia.close();
}

void Modelo_GRAP_R::montar_modelo() {
	model = IloModel(env);
	cplexvar_initiate();
	fo();
	restricoes();

}


void Modelo_GRAP_R::resolver_inteira() {
	//cplex.setParam(IloCplex::TiLim, 3600);
	try {
		//desalocar_matrizes();

		cplex = IloCplex(model);

		cplex.setParam(IloCplex::TiLim, 600);
		//cplex.setParam(IloCplex::Param::Emphasis::Numerical, 1);
		cplex.setParam(IloCplex::NodeFileInd, 1);

		soltime = cplex.getCplexTime();
		if (!cplex.solve()) {
			env.error() << "Otimizacao do LP mal-sucedida." << endl;
			resultados.open("resultados_MCCP.txt", fstream::app);
			resultados << "\t" << cplex.getStatus();
			resultados.close();
			return;
		}
		soltime = cplex.getCplexTime() - soltime;

		resultados.open("resultados_MCCP.txt", fstream::app);
		resultados << "\t" << cplex.getObjValue() << "\t" << cplex.getNnodes() << "\t" << cplex.getMIPRelativeGap() <<
			"\t" << soltime << "\t" << cplex.getStatus();
		resultados.close();

	}
	catch (IloException& e) {


		cerr << "Erro: " << e.getMessage() << endl;
		cout << "\nErro na inteira" << endl;
		return;
	}
	catch (...) {
		cerr << "Outra excecao" << endl;
	}
}


void Modelo_GRAP_R::resolver_linear() {
	//linear
	IloModel relax(env);
	relax.add(model);
	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++) {
			try {
				relax.add(IloConversion(env, x[i][j], ILOFLOAT));
			}
			catch (...) {
				continue;
			}
		}
	for (int j = 0; j < n; j++)
		relax.add(IloConversion(env, y[j], ILOFLOAT));
	cplex = IloCplex(relax);

	cplex.setParam(IloCplex::TiLim, 3600);
	cplex.setParam(IloCplex::Param::Emphasis::Numerical, 1);
	soltime = cplex.getCplexTime();
	if (!cplex.solve()) {
		env.error() << "Otimizacao do LP mal-sucedida." << endl;
		return;
	}
	soltime = cplex.getCplexTime() - soltime;


	resultados.open("resultados_MCCP.txt", fstream::app);
	resultados << "\t Linear \t" << cplex.getObjValue() << "\t" <<
		"\t" << soltime;
	resultados.close();
}
