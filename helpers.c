#include <stdio.h>

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
