#ifndef _SELA_H_
#define _SELA_H_

#include <stdlib.h>

struct RetornoGaussSeidel {
	int iteracoes;
	double erro;
};

struct RetornoGaussSeidel gaussSeidel(double** A,
		size_t n,
		double* estimativa,
		double w,
		double tolerancia,
		int maximoDeIteracoes);

double erroNormaMaxima(double* Xold, double* X, size_t size);

#endif
