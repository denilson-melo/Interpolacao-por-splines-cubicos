#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ERRO_FALHA_NO_ARQUIVO 1
#define ERRO_NUMERO_DE_PONTOS_DIFERENTES 2
#define ERRO_DE_ALOCACAO_DE_MEMORIA 3
void erro(int);
void erro(int e){
	switch (e){
		case ERRO_FALHA_NO_ARQUIVO:
			printf("Falha ao abrir arquivo\n");
			break;
		case ERRO_NUMERO_DE_PONTOS_DIFERENTES:
			printf("Quantidade de pontos nao coicidem\n");
			break;
		case ERRO_DE_ALOCACAO_DE_MEMORIA:
			printf("Erro de alocação de memória\n");
			break;
	}
}

//
/// Entrada
//
struct Entrada {
	int n;
	double* X;
	double* Y;
};
struct Entrada* lerArquivo(char *nomeDoArquivo);
void imprimeEntrada (struct Entrada *entrada);

//////////////////////////////////////////////////////////////////////////////////////
/// Spline
//////////////////////////////////////////////////////////////////////////////////////
struct Spline {
	double **polinomios;
	struct Entrada *entrada;
};
struct Spline* criaSpline(struct Entrada *entrada);
void imprimeSpline (struct Spline *spl);
double estimaValor(float x, struct Spline *spl);

//////////////////////////////////////////////////////////////////////////////////////
/// SELA
//////////////////////////////////////////////////////////////////////////////////////
void gaussSeidel(double** A, int size_A, double* estimativa, double w, int maximoDeIteracoes);

//////////////////////////////////////////////////////////////////////////////////////
/// Derivação e Integração
//////////////////////////////////////////////////////////////////////////////////////
double deriva(struct Spline *spl, float x, float h, int ordem);
double integra(struct Spline *spl, float a, float b, int n);

//////////////////////////////////////////////////////////////////////////////////////
/// Helpers
//////////////////////////////////////////////////////////////////////////////////////
void imprimeArray(double* A, int size_A);
void imprimeArray2D(double** A, int linhas, int colunas);

int main(int argc, char *argv[]){
	char* nomeDoArquivo;
	if (argc < 2) {
		printf("Forneca o nome do arquivo\n");
		return 0;
	} else {
		nomeDoArquivo = argv[1];
	}

	struct Entrada *entrada = lerArquivo(nomeDoArquivo);
	if (entrada == NULL) {
		return 1;
	}
	struct Spline *spl = criaSpline(entrada);
	imprimeSpline(spl);
	estimaValor(0.5, spl);
	printf("estimativa %lf \n", estimaValor(0.5, spl));
	printf("f'(0,5): %lf\n", deriva(spl, 0.5, 0.01, 1));
	printf("f''(0,5): %lf\n", deriva(spl, 0.5, 0.01, 2));
	printf("Integral de 0 a 1: %lf\n", integra(spl, 0, 1, 10));
	int i;
	for (i = 0; i < entrada->n-1; ++i) {
		printf("Integral de %lf a %lf: %lf\n", entrada->X[i], entrada->X[i+1], integra(spl, entrada->X[i], entrada->X[i+1], 1));
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////
/// Entrada
//////////////////////////////////////////////////////////////////////////////////////
struct Entrada* lerArquivo(char *nomeDoArquivo){
	int i=0,j=0;
	char buffer[1024] ;
	char *record, *line;
	double data[3][1024];

	FILE *fp = fopen(nomeDoArquivo,"r");
	if(fp == NULL)
	{
		printf("\n Erro ao abrir arquivo");
		return NULL;
	}
	while((line=fgets(buffer, sizeof(buffer),fp))!=NULL)
	{
		record = strtok(line,";");
		j = 0;
		while(record != NULL)
		{
			data[i][j++] = strtod(record, NULL) ;
			record = strtok(NULL,";");
		}
		if (i > 0 && j != data[0][0]) {
			erro(ERRO_NUMERO_DE_PONTOS_DIFERENTES);
			return NULL;
		}
		++i;
	}
	fclose(fp);

	struct Entrada *entrada = malloc(sizeof(struct Entrada));
	if (entrada == NULL) {
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}
	entrada-> n = (int)data[0][0];

	entrada->X = malloc(entrada->n * sizeof(double));
	if (entrada->X == NULL) {
		free(entrada);
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}

	entrada->Y = malloc(entrada->n * sizeof(double));
	if (entrada->Y == NULL) {
		free(entrada->X);
		free(entrada);
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}
	for (i = 0; i < entrada->n; ++i) {
		entrada->X[i] = data[1][i];
		entrada->Y[i] = data[2][i];
	}
	imprimeEntrada(entrada);
	return entrada;
}

void imprimeEntrada (struct Entrada *entrada) {
	printf("n: %d\n", entrada->n);
	printf("X: ");
	imprimeArray(entrada->X, entrada->n);
	printf("Y: ");
	imprimeArray(entrada->Y, entrada->n);
}

//////////////////////////////////////////////////////////////////////////////////////
/// Spline
//////////////////////////////////////////////////////////////////////////////////////
struct Spline* criaSpline(struct Entrada *entrada){
	int i;
	int n = entrada->n;
	double *X = entrada->X;
	double *Y = entrada->Y;
	double **A = (double **)malloc(n * sizeof(double*));
	if (A == NULL) {
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}
	for (i = 0; i < (n-2); ++i) {
		A[i] = (double *)calloc((n-1), sizeof(double));
		if (A[i] == NULL) {
			erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
			for (; i >= 0; --i)
				free(A[i]);
			return NULL;
		}
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
		a[n-2] = 6/(X[i+1] - X[i])*(Y[i+1] - Y[i]) + 6/(X[i] - X[i-1])*(Y[i-1] - Y[i]);
	}
	i = n-2;
	a = A[i-1];
	a[i-2] = X[i] - X[i-1];
	a[i-1] = 2*(X[i+1] - X[i-1]);
	a[n-2] = 6/(X[i+1] - X[i])*(Y[i+1] - Y[i]) + 6/(X[i] - X[i-1])*(Y[i-1] - Y[i]);

	double* F2 = (double *)malloc(n*sizeof(double));
	if (F2 == NULL) {
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		for (i = 0; i < n-2; ++i)
			free(A[i]);
		free(A);
		return NULL;
	}

	for (i = 0; i < n; ++i) {
		F2[i] = 0;
	}
	gaussSeidel(A, n-2, F2+1, 1, 500);

	struct Spline *spl = malloc(sizeof(struct Spline));
	spl->entrada = entrada;
	spl->polinomios = (double **)malloc((n-1) * sizeof(double *));
	for (i = 0; i < n-1; ++i) {
		spl->polinomios[i] = (double *)malloc(4 * sizeof(double));
	}
	for (i = 1; i < n; ++i) {
		spl->polinomios[i-1][0] = F2[i-1]/(6*(X[i] - X[i-1]));
		spl->polinomios[i-1][1] = F2[i]/(6*(X[i] - X[i-1]));
		spl->polinomios[i-1][2] = Y[i-1]/(X[i] - X[i-1]) - F2[i-1]*(X[i] - X[i-1])/6;
		spl->polinomios[i-1][3] = Y[i]/(X[i] - X[i-1]) - F2[i]*(X[i] - X[i-1])/6;
	}


	for (i = 0; i < n-2; ++i) {
		free(A[i]);
	}
	free(A);
	free(F2);
	return spl;
}

double estimaValor(float x, struct Spline *spl){
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

void imprimeSpline (struct Spline *spl){
	printf("Polinomios: ");
	imprimeArray2D(spl->polinomios, spl->entrada->n-1, 4);
}

//////////////////////////////////////////////////////////////////////////////////////
/// SELA
//////////////////////////////////////////////////////////////////////////////////////
void gaussSeidel(double** A, int size_A, double* estimativa, double w, int maximoDeIteracoes){
	int i, j;
	double x;

	int iteracoes = 1;
	int n = size_A;
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
}

//////////////////////////////////////////////////////////////////////////////////////
/// Derivação e Integração
//////////////////////////////////////////////////////////////////////////////////////
double deriva(struct Spline *spl, float x, float h, int ordem){
	/*imprimeSpline(spl);*/
	switch (ordem) {
		case 1:
			return (1/(2*h))*(-estimaValor(x - h, spl) + estimaValor(x + h, spl));
			break;
		case 2:
			return (1/pow(h,2))*(estimaValor(x - h, spl) - 2*estimaValor(x, spl) + estimaValor(x + h, spl));
			break;
		default:
			return 123456;
			
	}
}

double integra(struct Spline *spl, float a, float b, int n){
	int i;
	double h = (b-a)/n;
	double E = 0;
	for (i = 1; i < n; ++i) {
		if (i%2 == 0) {
			E += 4*estimaValor(i*h, spl); 
		} else {
			E += 2*estimaValor(i*h, spl);       
		}
	}
	return (h/3)*(estimaValor(a, spl) + E + estimaValor(b, spl));
}

//////////////////////////////////////////////////////////////////////////////////////
/// Helpers
//////////////////////////////////////////////////////////////////////////////////////
void imprimeArray(double *A, int size_A){
	int i;
	printf("[ ");
	for (i = 0; i < size_A-1; ++i) {
		printf("%lf, ", A[i]);
	}
	printf("%lf ", A[i]);
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

