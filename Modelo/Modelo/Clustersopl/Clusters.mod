int m = ...;
int n = ...;
int p = ...;
int k = ...;

range M = 1..m;
range K = 1..k;
range N = 1..n;

float d[M][N] = ...;
int c[N][K] = ...;
int q[M][K] = ...;

dvar boolean y[N];
dvar boolean x[M][N];

minimize sum (i in M, j in N) d[i][j]*x[i][j];

subject to{

	sum (j in N) y[j]  == p;

	forall (j in N){
		sum (i in M) x[i][j] >= y[j];
	}
	forall (i in M){
		sum (j in N) x[i][j] == 1;		
	}
	forall (j in N, k in K){
		sum (i in M) q[i][k]*x[i][j] <= c[j][k];		
	}
	forall (i in M, j in N){
		x[i][j] - y[j] <= 0;	
	}
}

