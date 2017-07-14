#ifndef _ENTRADA_E_SAIDA_H_
#define _ENTRADA_E_SAIDA_H_
#include <stdlib.h>

struct Entrada {
	size_t n;
	double* X;
	double* Y;
	size_t nF;
	double* F;
};
struct Entrada* lerArquivo(char *nomeDoArquivo);
void imprimeEntrada (struct Entrada *entrada);
void freeEntrada (struct Entrada *entrada);

#endif
