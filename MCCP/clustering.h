#include <iostream>
#include <fstream>
#include <ilcplex/ilocplex.h>
#include <list>

using namespace std;

class Clustering{
protected:
	ofstream resultados;
	IloNum soltime;
	double alpha;
	int m, n, p, k, **c, **q;
	double **d;
	IloArray<IloBoolVarArray> x;
	IloBoolVarArray y;
	IloEnv env;
	IloModel model;
	IloCplex cplex;
	string inst_name;
	double maior_peso;

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


	Clustering(const char* filename);
	void montar_modelo();
	void resolver_inteira();

	void resolver_linear();

	double calcular_lower_bound();

};
