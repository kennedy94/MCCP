#include "clustering.h"

void Clustering::alocar_matrizes() {
	c = vector<vector<int>>(n);
	for (int i = 0; i < n; i++)
		c[i] = vector<int>(K);
	q = vector<vector<int>>(m);
	for (int i = 0; i < m; i++)
		q[i] = vector<int>(K);
	d = vector<vector<double>>(m);
	for (int i = 0; i < m; i++)
		d[i] = vector<double>(n);
}


void Clustering::cplexvar_initiate() {
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
void Clustering::fo() {
	IloInt i, j;
	OBJETIVO = IloExpr(env);


	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			if(ESTA_NO_MODELO[i][j])
				OBJETIVO += d[i][j] * x[i][j];
	model.add(IloMinimize(env, OBJETIVO)).setName("FO");

}
void Clustering::restricoes() {
	IloInt i, j, w;
	IloExpr expr(env);
	for (j = 0; j < n; j++)
		expr += y[j];
	model.add(expr == p);
	expr.clear();
	for (j = 0; j < n; j++) {
		for (i = 0; i < m; i++)
			if (ESTA_NO_MODELO[i][j])
				expr += x[i][j];
		model.add(expr >= y[j]);
		expr.clear();
	}
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++)
			if (ESTA_NO_MODELO[i][j])
				expr += x[i][j];
		model.add(expr == 1);
		expr.clear();
	}
	for (j = 0; j < n; j++) {
		for (w = 0; w < K; w++) {
			for (i = 0; i < m; i++)
				if (ESTA_NO_MODELO[i][j])
					expr += q[i][w] * x[i][j];
			model.add(expr <= c[j][w]);
			expr.clear();
		}
	}
	expr.end();
	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			if (ESTA_NO_MODELO[i][j])
				model.add(x[i][j] - y[j] <= 0);

}

bool Clustering::maior_que_alpha(int i, int j) {
	if (d[i][j] > maior_peso*alpha)
		return true;
	else
		return false;
}

Clustering::Clustering(const char* filename, double alpha) {
	this->alpha = alpha;

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

	ESTA_NO_MODELO = vector<vector<int>>(m);
	for (int i = 0; i < m; i++)
		ESTA_NO_MODELO[i] = vector<int>(n, false);

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (!maior_que_alpha(i, j))
				ESTA_NO_MODELO[i][j] = true;
		}
	}

}

void Clustering::montar_modelo() {
	model = IloModel(env);
	cplexvar_initiate();
	fo();
	restricoes();

}



void Clustering::resolver_inteira() {
	//cplex.setParam(IloCplex::TiLim, 3600);
	try {
		//desalocar_matrizes();

		cplex = IloCplex(model);

		cplex.setParam(IloCplex::TiLim, 600);
		//cplex.setParam(IloCplex::Param::Emphasis::Numerical, 1);
		cplex.setParam(IloCplex::NodeFileInd, 1);

		soltime = cplex.getCplexTime();
		if (!cplex.solve()) {
			cout << cplex.getStatus() << endl;
			env.error() << "Otimizacao do LP mal-sucedida." << endl;
			return;
		}
		soltime = cplex.getCplexTime() - soltime;
		
		resultados.open("resultados_MCCP.txt", fstream::app);
		resultados << "\t" << cplex.getObjValue() << "\t" << cplex.getNnodes() << "\t" << cplex.getMIPRelativeGap() <<
			"\t" << soltime;
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


void Clustering::resolver_linear() {
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

	cplex.setParam(IloCplex::TiLim, 600);
	cplex.setParam(IloCplex::NodeFileInd, 1);
	//cplex.setParam(IloCplex::TiLim, 3600);
	//cplex.setParam(IloCplex::Param::Emphasis::Numerical, 1);
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

double Clustering::calcular_lower_bound() {
	double lower_bound = 0.0;

	
	for (int i = 0; i < m; i++)
	{
		double min = 9999999999;
		for (int j = 0; j < n; j++)
		{
			if (d[i][j] < min)
				min = d[i][j];
		}
		lower_bound += min;
	}

	return lower_bound;
}

