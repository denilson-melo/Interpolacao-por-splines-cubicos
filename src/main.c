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

	// Lê o nome do arquivo da entrada
	char* nomeDoArquivo;
	if (argc < 2) {
		printf("Forneca o nome do arquivo\n");
		return 0;
	} else {
		nomeDoArquivo = argv[1];
	}


	printf("Interpolando os pontos de entrada com splines cubicos...\n");

	// Recebe os dados de entrada organizados
	struct Entrada *entrada = lerArquivo(nomeDoArquivo);
	if (entrada == NULL) {
		erro(ERRO_NA_LEITURA_DA_ENTRADA);
		return 1;
	}

	// Efetua a interpolação
	struct Spline *spl = criaSpline(entrada);
	if (entrada == NULL) {
		freeEntrada(entrada);
		erro(ERRO_NA_CRIACAO_DO_SPLINE);
		return 1;
	}
	printf("Pronto.\n");

	// Abre um arquivo de saída com o nome do arquivo de entrada com o
	// sufixo '.out' ao final
	char* nomeDoArquivoDeSaida = malloc((strlen(nomeDoArquivo) + 5) * sizeof(char));
	sprintf(nomeDoArquivoDeSaida, "%s.out", nomeDoArquivo);
	FILE *fp = fopen(nomeDoArquivoDeSaida,"w");
	if(fp == NULL)
	{
		freeEntrada(entrada);
		freeSpline(spl);
		erro(ERRO_FALHA_NO_ARQUIVO);
		return 1;
	}

	// Imprime os coeficientes do spline no arquivo
	imprimeSpline(fp, spl);

	// Cria valores de h para a derivação numérica:
	// [0.1, 0.01, 0.001, ...]
	int i;
	double H[10];
	H[0] = 0.1;
	for (i = 1; i < 11; ++i) {
		H[i] = H[i-1] * 0.1;
	}

	// Avalia a função para os valores fornecidos na terceira linha da entrada
	// A terceira linha é opcional, se estiver vazia ignora este passo
	if (entrada->nF > 0)
	{
		fprintf(fp, "Avaliando f(x) f'(x) f''(x) nos pontos:\n");
		imprimeArray(fp, entrada->F, entrada->nF);
		fprintf(fp, "\n");
	}
	int j;
	float x; 
	for (i = 0; i < entrada->nF; ++i) {
		x = entrada->F[i];
		fprintf(fp, "x     : %f\n", x);
		// Avalia a função no ponto x
		fprintf(fp, "f(x)  : %0.31f\n", estimaValor(spl, x));

		// Calcula a primeira derivada f'(x) para os diferentes valores de h
		for (j = 0; j < 10; ++j) {
			fprintf(fp, "f'(x) h=%.10f: %0.31f\n", H[j], deriva(spl, x, H[j], 1));
		}

		// Calcula a primeira derivada f''(x) para os diferentes valores de h
		for (j = 0; j < 10; ++j) {
			fprintf(fp, "f''(x) h=%.10f: %0.31f\n", H[j], deriva(spl, x, H[j], 2));
		}
		fprintf(fp, "\n");
	}

	// Calcula as integrais do spline
	float a = entrada->X[0];
	float b = entrada->X[entrada->n-1];
	for (i = 0; i < 6; ++i) {
		fprintf(fp, "Integral (n = %lf) do spline de %f a %f: %0.31f\n", 1/H[i], a, b, integra(spl, a, b, 1/H[i]));
	}
	fprintf(fp, "Integrais dos intervalos do spline:\n");
	for (i = 0; i < entrada->n-1; ++i) {
		fprintf(fp, "de %lf a %lf: %0.31f\n", entrada->X[i], entrada->X[i+1], integra(spl, entrada->X[i], entrada->X[i+1], 1));
	}

	// FIM
	printf("As saidas do programa foram impressas em: '%s'\n", nomeDoArquivoDeSaida);
	fclose(fp);

	free(nomeDoArquivoDeSaida);
	freeSpline(spl);
	freeEntrada(entrada);
	return 0;
}
