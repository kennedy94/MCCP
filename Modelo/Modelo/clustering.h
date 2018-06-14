#include <iostream>
#include <fstream>
#include <ilcplex/ilocplex.h>
#include <list>

using namespace std;

class Clustering{
protected:
	ofstream resultados;
	IloNum soltime;
	int m, n, p, k, **c, **q;
	double **d;
	IloArray<IloBoolVarArray> x;
	IloBoolVarArray y;
	IloEnv env;
	IloModel model;
	IloCplex cplex;
	string inst_name;
	void alocar_matrizes() {
		c = new int*[n];
		for (int i = 0; i < n; i++)
			c[i] = new int[k];
		q = new int*[m];
		for (int i = 0; i < m; i++)
			q[i] = new int[k];
		d = new double*[m];
		for (int i = 0; i < m; i++)
			d[i] = new double[n];
	}
	void desalocar_matrizes() {
		for (int i = 0; i < n; ++i) {
			delete[] c[i];
		}
		delete[] c;
		for (int i = 0; i < m; ++i) {
			delete[] q[i];
		}
		delete[] q;
		for (int i = 0; i < m; ++i) {
			delete[] d[i];
		}
		delete[] d;
	}
	void cplexvar_initiate() {
		y = IloBoolVarArray(env, n);

		x = IloArray<IloBoolVarArray>(env, n*m);
		for (int i = 0; i < m; i++)
			x[i] = IloBoolVarArray(env, n);

		char strnum[30];
		for (int i = 0; i < n; i++){
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
	void fo() {
		IloInt i, j;
		IloExpr costSum(env);
		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++)
				costSum += d[i][j] * x[i][j];
		model.add(IloMinimize(env, costSum)).setName("FO");
		costSum.end();
	}
	void restricoes() {
		IloInt i, j, w;
		IloExpr expr(env);
		for (j = 0; j < n; j++)
			expr += y[j];
		model.add(expr == p);
		expr.clear();
		for (j = 0; j < n; j++) {
			for (i = 0; i < m; i++)
				expr += x[i][j];
			model.add(expr >= y[j]);
			expr.clear();
		}
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++)
				expr += x[i][j];
			model.add(expr == 1);
			expr.clear();
		}
		for (j = 0; j < n; j++) {
			for (w = 0; w < k; w++) {
				for (i = 0; i < m; i++)
					expr += q[i][w] * x[i][j];
				model.add(expr <= c[j][w]);
				expr.clear();
			}
		}
		expr.end();
		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++)
				model.add(x[i][j] - y[j] <= 0);
	}
public:
	Clustering(const char* filename) {
		inst_name = filename;
		ifstream instancia(inst_name, ifstream::in);
		if (instancia.fail()) {
			cerr << "	Instancia nao encontrada" << endl;
			system("pause");
			exit(1);
		}
		instancia >> m >> n >> p >> k;
		alocar_matrizes();

		for (int i = 0; i < m; i++)
			for (int j = 0; j < k; j++)
				instancia >> q[i][j];
		for (int i = 0; i < n; i++)
			for (int j = 0; j < k; j++)
				instancia >> c[i][j];
		for (int i = 0; i < m; i++)
			for (int j = 0; j < n; j++){
				instancia >> d[i][j];
				//cout << d[i][j] << " ";
				//getchar();
			}
		instancia.close();
	}

	void resolver() {

		try {
			model = IloModel(env);
			cplexvar_initiate();
			fo();
			restricoes();
			desalocar_matrizes();

			for (int i = 0; i < m; i++)
                for (int j = 0; j < n; j++)
                    if(d[i][j] > 30)
                        x[i][j].removeFromAll();

			cplex = IloCplex(model);

			cplex.setParam(IloCplex::TiLim, 3600);

            soltime = cplex.getCplexTime();

            if (!cplex.solve()) {
                env.error() << "Otimizacao do LP mal-sucedida." << endl;
                throw(-1);
            }

            soltime = cplex.getCplexTime() - soltime;

            resultados.open("resultados.txt", fstream::app);
            resultados << endl << inst_name << "\t"<< cplex.getBestObjValue() << "\t" << cplex.getNnodes() << "\t" << cplex.getNiterations() << "\t"<< cplex.getMIPRelativeGap() <<
                "\t"<< soltime << "\t";



		}
		catch (IloException& e) {
			cerr << "Erro: " << e.getMessage() << endl;
			cout << "\nErro na inteira" << endl;
			return;
		}
		catch (...) {
			cerr << "Outra excecao" << endl;
			cout << "\noutra excecao" << endl;
		}


		//linear
        IloModel relax(env);
        relax.add(model);
		for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++){
                try{
                    relax.add(IloConversion(env, x[i][j], ILOFLOAT));
                }catch (...) {
                    cerr << "Outra excecao" << endl;
                    cout << "\noutra excecao" << endl;
                }
		}


        relax.add(IloConversion(env, y, ILOFLOAT));
        cplex = IloCplex(relax);
		soltime = cplex.getCplexTime();

		if (!cplex.solve()) {
			env.error() << "Otimizacao do LP mal-sucedida." << endl;
			throw(-1);
		}


		soltime = cplex.getCplexTime() - soltime;
		resultados << cplex.getObjValue() << "	" <<
			"	"<< soltime << endl;
		resultados.close();
	}
};
