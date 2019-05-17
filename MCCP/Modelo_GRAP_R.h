#pragma once
#include <iostream>
#include <fstream>
#include <ilcplex/ilocplex.h>
#include <list>
#include <vector>
#include <chrono>
#include <random>

using namespace std;

class Modelo_GRAP_R
{
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
	double densidade_arcos;
	string inst_name;
	double maior_peso, menor_peso;

	vector<vector<bool>> arcos_aceitos;

	void alocar_matrizes();
	//void desalocar_matrizes();
	void cplexvar_initiate();
	void fo();
	void restricoes();

	bool maior_que_alpha(int i, int j);
public:
	void set_alpha(double alpha) {
		this->alpha = alpha;
	}

	void Definir_Arcos_SA();

	void Definir_Arcos_GRASP();


	Modelo_GRAP_R(const char* filename);
	void montar_modelo();
	void resolver_inteira();

	void resolver_linear();

};

