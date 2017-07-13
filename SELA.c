#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


struct Entrada {
	int n;
	double* X;
	double* Y;
};

struct Spline {
	double **polinomios;
	struct Entrada *entrada;
};

void imprimeEntrada (struct Entrada *entrada);
void gaussSeidel(double** A, int size_A, double* estimativa, double w, int maximoDeIteracoes);
void imprimeArray(double* A, int size_A);
void imprimeArray2D(double** A, int linhas, int colunas);
double estimaValor(double x, struct Spline *spl);
struct Spline* criaSpline(struct Entrada *entrada);

int main(void) {
	char buffer[1024] ;
	char *record,*line;
	int i=0,j=0;
	double data[3][1024];
	FILE *fstream = fopen("entrada.csv","r");
	if(fstream == NULL)
	{
		printf("\n file opening failed ");
		return -1 ;
	}
	while((line=fgets(buffer, sizeof(buffer),fstream))!=NULL)
	{
		record = strtok(line,";");
		j = 0;
		while(record != NULL)
		{
			data[i][j++] = atof(record) ;
			record = strtok(NULL,";");
		}
		++i;
	}


	struct Entrada entrada1;
	int n = (int)data[0][0];
	double* X = malloc(n * sizeof(double));
	double* Y = malloc(n * sizeof(double));
	for (i = 0; i < n; ++i) {
		X[i] = data[1][i];
		Y[i] = data[2][i];
	}

	entrada1.n = n;
	entrada1.X = malloc(n * sizeof(double));
	entrada1.Y = malloc(n * sizeof(double));
	for (i = 0; i < n; ++i) {
		entrada1.X[i] = data[1][i];
		entrada1.Y[i] = data[2][i];
	}
	imprimeEntrada(&entrada1);

	printf("estimativa %f \n", spline(polinomios, X, n, 0.5));

	free(X);
	free(Y);
	free(polinomios);
	return 0;
}

void imprimeEntrada (struct Entrada *entrada) {
	printf("n: %d\n", entrada->n);
	printf("X: ");
	imprimeArray(entrada->X, entrada->n);
	printf("Y: ");
	imprimeArray(entrada->Y, entrada->n);
}



struct Spline* criaSpline(struct Entrada *entrada){
	int i;
	int n = entrada->n;
	double *X = entrada->X;
	double *Y = entrada->Y;
	double** A = malloc((entrada->n-2) * sizeof(double*));
	for (i = 0; i < (n-2); ++i) {
		A[i] = calloc((n-1), sizeof(double));
	}
	double *a;


	i = 1;
	a = A[i-1];
	a[i-1] = 2*(X[i+1] - X[i-1]);
	a[i] = (X[i+1] - X[i]);
	a[n-2] = 6/(X[i+1] - X[i])*(Y[i+1] - Y[i]) + 6/(X[i] - X[i-1])*(Y[i-1] - Y[i]);
	for (i = 2; i < n-2; ++i) {
		a = A[i-1];
		a[i-2] = X[i] - X[i-1];
		a[i-1] = 2*(X[i+1] - X[i-1]);
		a[i] = (X[i+1] - X[i]);
		a[n] = 6/(X[i+1] - X[i])*(Y[i+1] - Y[i]) + 6/(X[i] - X[i-1])*(Y[i-1] - Y[i]);
	}
	i = n-2;
	a = A[i-1];
	a[i-2] = X[i] - X[i-1];
	a[i-1] = 2*(X[i+1] - X[i-1]);
	a[n-2] = 6/(X[i+1] - X[i])*(Y[i+1] - Y[i]) + 6/(X[i] - X[i-1])*(Y[i-1] - Y[i]);

	imprimeArray2D(A, n-2, n-1);

	double* F2 = malloc(n*sizeof(double));
	for (i = 0; i < n; ++i) {
		F2[i] = 0;
	}
	gaussSeidel(A, n-2, F2+1, 1, 500);
	imprimeArray(F2, n);

	struct Spline *spl = malloc(sizeof(struct Spline));
	spl->polinomios = malloc((n-1) * sizeof(double *));
	for (i = 0; i < n-1; ++i) {
		spl->polinomios[i] = malloc(4 * sizeof(double));
	}
	for (i = 1; i < n; ++i) {
		spl->polinomios[i-1][0] = F2[i-1]/(6*(X[i] - X[i-1]));
		spl->polinomios[i-1][1] = F2[i]/(6*(X[i] - X[i-1]));
		spl->polinomios[i-1][2] = Y[i-1]/(X[i] - X[i-1]) - F2[i-1]*(X[i] - X[i-1])/6;
		spl->polinomios[i-1][3] = Y[i]/(X[i] - X[i-1]) - F2[i]*(X[i] - X[i-1])/6;
	}

	imprimeArray2D(spl->polinomios, n-1, 4);

	free(A);
	free(F2);
	for (i = 0; i < (n-2); ++i) {
		free(A[i]);
	}
	return spl;
}

double estimaValor(double x, struct Spline *spl){
	int i;
	double *X = spl->entrada->X;
	for (i = 0; i < spl->entrada->n-1; ++i) {
		if (x <= X[i+1]) {
            i += 1;
            break;    
		}
	}
	return (       
		spl->polinomios[i-1][0] * pow(X[i] - x, 3)
		+ spl->polinomios[i-1][1] * pow(x - X[i-1],3)
		+ (X[i] - x)* spl->polinomios[i-1][2]
		+ (x - X[i-1])* spl->polinomios[i-1][3]
	);
}

void imprimeArray(double *A, int size_A){
	int i;
	printf("[ ");
	for (i = 0; i < size_A-1; ++i) {
		printf("%f, ", A[i]);
	}
	printf("%f ", A[i]);
	printf("]\n");
}

void imprimeArray2D(double** A, int linhas, int colunas){
	int i;
	printf("[\n");
	for (i = 0; i < linhas; ++i) {
		imprimeArray(A[i], colunas);
	}
	printf("]\n");
}

void gaussSeidel(double** A, int size_A, double* estimativa, double w, int maximoDeIteracoes){
	int i, j;
	double x;

	int iteracoes = 1;
	int n = size_A;
	double* Xold = estimativa;
	double* X = estimativa;

	while (iteracoes < maximoDeIteracoes){
		for (i = 0; i < n ; i++) {
			x = A[i][n];
			for (j = 0; j < i ; j++) {
				x -= A[i][j]*X[j];
			}
			for (j = i+1; j < n ; j++) {
				x -= A[i][j]*X[j];
			}
			x *= 1/A[i][i];
			X[i] = w*x + (1-w)*X[i];
		}
		iteracoes++;
	}

	/*return X;*/
}
