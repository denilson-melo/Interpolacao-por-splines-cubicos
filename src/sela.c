#include <stdio.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include "sela.h"

struct RetornoGaussSeidel* gaussSeidel(double** A, size_t n, double* estimativa, double w, double tolerancia, int maximoDeIteracoes){
	int i, j;
	double x;
	int iteracoes = 1;
	double* X = estimativa;
	double* Xold = malloc(n * sizeof(double));
	memcpy(Xold, X, n * sizeof(double)); // Copia o array para comparar as iterações

	double erro = 100;
	while (erro > tolerancia && iteracoes < maximoDeIteracoes){
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
		erro = erroNormaMaxima(Xold, X, n);
		memcpy(Xold, X, n * sizeof(double));
		iteracoes++;
	}

	struct RetornoGaussSeidel* retorno = malloc(sizeof(struct RetornoGaussSeidel));
	retorno->iteracoes = iteracoes;
	retorno->erro = erro;
	return retorno;
}

double erroNormaMaxima(double* Xold, double* X, size_t size) {
	double norma, normaMaxima;
	int i = 0;
	normaMaxima = fabs((X[i] - Xold[i])/(DBL_MIN+X[i])); //DBL_MIN para evitar divisão por 0
	for (i = 1; i < size; ++i) {
		norma = fabs((X[i] - Xold[i])/(DBL_MIN+X[i]));
		if (norma > normaMaxima)
			normaMaxima = norma;
	}
	return normaMaxima;
}
