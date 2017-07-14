#ifndef _ENTRADA_E_SAIDA_H_
#define _ENTRADA_E_SAIDA_H_

struct Entrada {
	int n;
	double* X;
	double* Y;
};
struct Entrada* lerArquivo(char *nomeDoArquivo);
void imprimeEntrada (struct Entrada *entrada);

#endif
