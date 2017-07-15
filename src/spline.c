#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "spline.h"
#include "erro.h"
#include "sela.h"
#include "entrada-e-saida.h"

struct Spline* criaSpline(struct Entrada* entrada){
	int i;
	int n = entrada->n;
	double* X = entrada->X;
	double* Y = entrada->Y;
	double** A = (double **)malloc(n * sizeof(double*));
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

	double* a;
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

	double* F2 = calloc(n, sizeof(double));
	if (F2 == NULL) {
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		for (i = 0; i < n-2; ++i)
			free(A[i]);
		free(A);
		return NULL;
	}

	struct RetornoGaussSeidel retornoGaussSeidel = gaussSeidel(
			A,     //Matriz
			n-2,   //Tamanho
			F2+1,  //Estimativa inicial
			1,     //Relaxamento(w)
			DBL_MIN, //Tolerancia
			500);  //Máximo de iterações

	printf("ITER %d\n", retornoGaussSeidel.iteracoes);
	printf("ERRO %lf\n", retornoGaussSeidel.erro);

	struct Spline *spl = malloc(sizeof(struct Spline));
	if (F2 == NULL) {
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		for (i = 0; i < n-2; ++i)
			free(A[i]);
		free(A);
		return NULL;
	}

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

double estimaValor(struct Spline* spl, float x){
	int i;
	double* X = spl->entrada->X;
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

void imprimeSpline (FILE* fp, struct Spline* spl){
	fprintf(fp, "Coeficientes do spline cubico:\n");
	imprimeArray2D(fp, spl->polinomios, spl->entrada->n-1, 4);
	fprintf(fp, "\n");
}

void freeSpline (struct Spline* spl){
	int i;
	for (i = 0; i < 4; ++i) {
		free(spl->polinomios[i]);
	}
	free(spl->polinomios);
	free(spl);
}
