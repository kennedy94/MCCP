#include "clustering.h"
#include "Modelo_GRAP_R.h"
#include "MH.h"


int main(int argc, char *argv[])
{
	if(argc != 4){
		cerr << "	Erro de entrada!!" << endl;
		exit(0);
	}
	double alpha = atof(argv[2]);
	double gamma = atof(argv[3]);
	//double alpha = 1;

	/*ofstream resultados("resultados_MCCP.txt", fstream::app);
	resultados << endl << argv[1] << "\t alpha" << alpha << "\t";
	resultados.close();*/

	/*try
	{
		Clustering problema(argv[1]);
		problema.set_alpha(alpha);
		problema.montar_modelo();
		problema.resolver_linear();
	}
	catch (const std::exception&)
	{
		cerr << "Erro Linear" << endl;
	}*/

	/*try
	{
		Clustering problema(argv[1]);
		problema.set_alpha(alpha);
		problema.montar_modelo();
		problema.resolver_inteira();
	}
	catch (const std::exception&)
	{
		cerr << "Erro Inteira" << endl;
	}*/

	/*try
	{
		Modelo_GRAP_R problema(argv[1]);
		problema.set_alpha(alpha);
		problema.Definir_Arcos_SA();
		problema.montar_modelo();
		problema.resolver_inteira();
	}
	catch (const std::exception&)
	{
		cerr << "Erro Inteira" << endl;
	}*/

	/*try
	{
		Modelo_GRAP_R problema(argv[1]);
		problema.set_alpha(alpha);
		problema.Definir_Arcos_GRASP();
		problema.montar_modelo();
		problema.resolver_inteira();
	}
	catch (const std::exception&)
	{
		cerr << "Erro Inteira" << endl;
	}*/
	
	/*{
		Clustering problema(argv[1]);
		ofstream arquivo("lower_bounds.txt", fstream::app);
		cout << argv[1] << "\t" << problema.calcular_lower_bound() << endl;
		arquivo.close();
	}*/

	


	/*ofstream resultados("resultados_MCCP.txt", fstream::app);
	resultados << endl << argv[1] << "\t alpha" << alpha << " '" << gamma << "'\t";
	resultados.close();

	try {
		MH problema(argv[1], alpha);
		problema.Gamma = gamma;
		problema.MATHEURISTICA_HIBRIDA();
	}
	catch (const std::exception&)
	{
		cerr << "Erro Inteira" << endl;
	}*/

	ofstream resultados("resultados_Heuristica.txt", fstream::app);
	resultados << endl << argv[1] << "\t alpha" << alpha << " '" << gamma << "'\t";
	
	try {
		MH problema(argv[1], alpha);
		problema.Gamma = gamma;
		resultados << problema.HEURISTICA_GULOSA().fitness;
	}
	catch (const std::exception&)
	{
		cerr << "Erro Inteira" << endl;
	}

	resultados.close();


	return 0;
}
