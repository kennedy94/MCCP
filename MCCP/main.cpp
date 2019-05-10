#include "clustering.h"

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

	try
	{
		Clustering problema(argv[1]);
		problema.set_alpha(alpha);
		problema.montar_modelo();
		problema.resolver_linear();
	}
	catch (const std::exception&)
	{
		cerr << "Erro Linear" << endl;
	}

	try
	{
		Clustering problema(argv[1]);
		problema.set_alpha(alpha);
		problema.montar_modelo();
		problema.resolver_inteira();
	}
	catch (const std::exception&)
	{
		cerr << "Erro Inteira" << endl;
	}

	

	return 0;
}
