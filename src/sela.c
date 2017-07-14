#include "sela.h"

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
