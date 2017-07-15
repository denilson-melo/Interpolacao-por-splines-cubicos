#ifndef _ENTRADA_E_SAIDA_H_
#define _ENTRADA_E_SAIDA_H_
#include <stdlib.h>
#include <stdio.h>

struct Entrada {
	size_t n;
	double* X;
	double* Y;
	size_t nF;
	double* F;
};

struct Entrada* lerArquivo(char *nomeDoArquivo);
void imprimeEntrada (FILE* fp, struct Entrada *entrada);
void freeEntrada (struct Entrada *entrada);
void imprimeArray(FILE* fp, double* A, int size_A);
void imprimeArray2D(FILE* fp, double** A, int linhas, int colunas);

#endif
