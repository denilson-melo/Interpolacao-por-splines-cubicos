#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "entrada-e-saida.h"
#include "sela.h"
#include "derivacao-e-integracao.h"
#include "spline.h"
#include "erro.h"
#include "helpers.h"

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
	printf("Pronto.\n");

	struct Spline *spl = criaSpline(entrada);
	imprimeSpline(spl);

	int i;
	float x; 

	printf("Avaliando f(x) f'(x) f''(x) nos pontos:\n");
	imprimeArray(entrada->F, entrada->nF);
	printf("\n");

	for (i = 0; i < entrada->nF; ++i) {
		x = entrada->F[i];
		printf("x: %f\n", x);
		printf("f(x): %lf\n", estimaValor(spl, x));
		printf("f'(x): %lf\n", deriva(spl, x, 0.01, 1));
		printf("f''(x): %lf\n", deriva(spl, x, 0.01, 2));
		printf("\n");
	}

	float a = entrada->X[0];
	float b = entrada->X[entrada->n-1];
	printf("Integral do spline de %f a %f:\n%lf\n", a, b, integra(spl, a, b, 10));
	printf("Integrais dos intervalos do spline:\n");
	for (i = 0; i < entrada->n-1; ++i) {
		printf("de %lf a %lf: %lf\n", entrada->X[i], entrada->X[i+1], integra(spl, entrada->X[i], entrada->X[i+1], 1));
	}
	return 0;
}
