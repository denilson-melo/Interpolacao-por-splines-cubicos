#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "spline.h"
#include "erro.h"
#include "sela.h"
#include "helpers.h"

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
