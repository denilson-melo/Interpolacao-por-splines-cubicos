#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrada-e-saida.h"
#include "erro.h"
#include "helpers.h"

struct Entrada* lerArquivo(char *nomeDoArquivo){
	int i=0,j=0;
	char buffer[1024] ;
	char *record, *line;
	double data[3][1024];
	int quantidadeDeItems[3];

	FILE *fp = fopen(nomeDoArquivo,"r");
	if(fp == NULL)
	{
		printf("\n Erro ao abrir arquivo");
		return NULL;
	}

	while((line=fgets(buffer, sizeof(buffer),fp))!=NULL)
	{
		record = strtok(line,";");
		j = 0;
		while(record != NULL)
		{
			data[i][j++] = strtod(record, NULL) ;
			record = strtok(NULL,";");
		}
		quantidadeDeItems[i] = j;
		++i;
	}
	fclose(fp);

	if (quantidadeDeItems[0] == 0) {
		erro(ERRO_ENTRADA_VAZIA);
		return NULL;
	}

	if (quantidadeDeItems[0] != quantidadeDeItems[1]) {
		erro(ERRO_NUMERO_DE_PONTOS_DIFERENTES);
		return NULL;
	}

	struct Entrada *entrada = malloc(sizeof(struct Entrada));
	if (entrada == NULL) {
		freeEntrada(entrada);
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}
	entrada->X = entrada->Y = entrada->F = NULL;

	entrada-> n = quantidadeDeItems[0];
	entrada->X = malloc(entrada->n * sizeof(double));
	if (entrada->X == NULL) {
		freeEntrada(entrada);
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}

	entrada->Y = malloc(entrada->n * sizeof(double));
	if (entrada->Y == NULL) {
		freeEntrada(entrada);
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}

	entrada->nF = quantidadeDeItems[2];
	if (entrada->nF != 0) {
		entrada->F = malloc(entrada->nF * sizeof(double));
		if (entrada->F == NULL) {
			freeEntrada(entrada);
			erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
			return NULL;
		}
	}

	for (i = 0; i < entrada->n; ++i) {
		entrada->X[i] = data[0][i];
		entrada->Y[i] = data[1][i];
	}
	for (i = 0; i < entrada->nF; ++i) {
		entrada->F[i] = data[2][i];
	}

	imprimeEntrada(entrada);
	return entrada;
}

void freeEntrada(struct Entrada* entrada) {
	printf("LIBERANDO TUDO\n");
	free(entrada->X);
	free(entrada->Y);
	free(entrada->F);
	printf("LIBERED TUDO\n");
}

void imprimeEntrada (struct Entrada *entrada) {
	printf("Entrada: \n");
	printf("X: ");
	imprimeArray(entrada->X, entrada->n);
	printf("Y: ");
	imprimeArray(entrada->Y, entrada->n);
	printf("F: ");
	if (entrada->nF != 0) {
		imprimeArray(entrada->F, entrada->nF);
	} else {
		printf("[]");
	}
	printf("\n");
}
