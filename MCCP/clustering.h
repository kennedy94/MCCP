#pragma once
#include <iostream>
#include <fstream>
#include <ilcplex/ilocplex.h>
#include <list>
#include <vector>


using namespace std;

class Clustering{
protected:
	ofstream resultados;
	IloNum soltime;
	double alpha;
	int m, n, p, K, **c, **q;
	double **d;
	IloArray<IloBoolVarArray> x;
	IloBoolVarArray y;
	IloEnv env;
	IloModel model;
	IloCplex cplex;
	IloExpr OBJETIVO;
	string inst_name;
	double maior_peso;
	vector< vector<int>> ESTA_NO_MODELO;

	void alocar_matrizes();
	//void desalocar_matrizes();
	void cplexvar_initiate();
	void fo();
	void restricoes();

	bool maior_que_alpha(int i, int j);
public:
	Clustering(const char* filename, double alpha);
	void montar_modelo();
	void resolver_inteira();

	void resolver_linear();

	double calcular_lower_bound();

};
