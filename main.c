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
	struct Spline *spl = criaSpline(entrada);
	imprimeSpline(spl);
	printf("f'(0,5): %lf\n", deriva(spl, 0.5, 0.01, 1));
	printf("f''(0,5): %lf\n", deriva(spl, 0.5, 0.01, 2));
	printf("Integral de 0 a 1: %lf\n", integra(spl, 0, 1, 10));
	int i;
	for (i = 0; i < entrada->n-1; ++i) {
		/*printf("Integral de %lf a %lf: %lf\n", entrada->X[i], entrada->X[i+1], integra(spl, entrada->X[i], entrada->X[i+1], 1));*/
	}
	return 0;
}
