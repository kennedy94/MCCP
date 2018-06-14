#include "clustering.h"

int main(int argc, char *argv[])
{
	if(argc =! 2){
		cerr << "	Erro de entrada!!" << endl;
		system("pause");
		exit(0);
	}
	Clustering problema(argv[1]);
	problema.resolver();
    return 0;
}
