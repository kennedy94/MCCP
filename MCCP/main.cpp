#include "clustering.h"

int main(int argc, char *argv[])
{
	if(argc > 3){
		cerr << "	Erro de entrada!!" << endl;
		exit(0);
	}
	double alpha = atof(argv[2]);
	cout << "Alpha: " << alpha << endl;
	Clustering problema(argv[1]);
	problema.set_alpha(alpha);
	problema.resolver_inteira();
	problema.resolver_linear();
    return 0;
}
