#include "clustering.h"
#include "Modelo_GRAP_R.h"
#include "MH.h"


int main(int argc, char *argv[])
{
	if(argc != 3){
		cerr << "	Erro de entrada!!" << endl;
		exit(0);
	}
	double alpha = atof(argv[2]);
	//double alpha = 1;

	ofstream resultados("resultados_MCCP.txt", fstream::app);
	resultados << endl << argv[1] << "\t alpha" << alpha << "\t";
	resultados.close();

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

	
	try {
		MH problema(argv[1]);
		//problema.set_alpha(alpha);
		//problema.montar_modelo();
		//problema.resolver_inteira();
		//problema.ILS();
		problema.HEURISTICA_GULOSA();
	}
	catch (const std::exception&)
	{
		cerr << "Erro Inteira" << endl;
	}



	return 0;
}
