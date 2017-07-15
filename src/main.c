#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "entrada-e-saida.h"
#include "sela.h"
#include "derivacao-e-integracao.h"
#include "spline.h"
#include "erro.h"

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

	printf("Interpolando os pontos de entrada com splines cubicos...\n");

	struct Spline *spl = criaSpline(entrada);

	printf("Pronto.\n");

	int i;
	float x; 

	char* nomeDoArquivoDeSaida = malloc((strlen(nomeDoArquivo) + 5) * sizeof(char));
	sprintf(nomeDoArquivoDeSaida, "%s.out", nomeDoArquivo);
	FILE *fp = fopen(nomeDoArquivoDeSaida,"w");
	if(fp == NULL)
	{
		erro(ERRO_FALHA_NO_ARQUIVO);
		return 1;
	}

	imprimeSpline(fp, spl);
	fprintf(fp, "Avaliando f(x) f'(x) f''(x) nos pontos:\n");
	imprimeArray(fp, entrada->F, entrada->nF);
	fprintf(fp, "\n");

	for (i = 0; i < entrada->nF; ++i) {
		x = entrada->F[i];
		fprintf(fp, "x     : %f\n", x);
		fprintf(fp, "f(x)  : %0.31f\n", estimaValor(spl, x));
		fprintf(fp, "f'(x) : %0.31f\n", deriva(spl, x, 0.0001, 1));
		fprintf(fp, "f''(x): %0.31f\n", deriva(spl, x, 0.0001, 2));
		fprintf(fp, "\n");
	}

	float a = entrada->X[0];
	float b = entrada->X[entrada->n-1];
	fprintf(fp, "Integral do spline de %f a %f:\n%0.31f\n", a, b, integra(spl, a, b, 100));
	fprintf(fp, "Integrais dos intervalos do spline:\n");
	for (i = 0; i < entrada->n-1; ++i) {
		fprintf(fp, "de %lf a %lf: %0.31f\n", entrada->X[i], entrada->X[i+1], integra(spl, entrada->X[i], entrada->X[i+1], 1));
	}
	printf("As saidas do programa foram impressas em: '%s'\n", nomeDoArquivoDeSaida);
	fclose(fp);

	freeEntrada(entrada);
	freeSpline(spl);
	return 0;
}
