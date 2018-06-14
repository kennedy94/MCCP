#include "clustering.h"

int main(int argc, char *argv[])
{
	if(argc =! 2){
		cerr << "	Erro de entrada!!" << endl;
		system("pause");
		exit(0);
	}
	Clustering problema(argv[1]);
	problema.resolver_inteira();
	problema.resolver_linear();
    return 0;
}
