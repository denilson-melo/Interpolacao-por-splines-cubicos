#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrada-e-saida.h"
#include "erro.h"

struct Entrada* lerArquivo(char *nomeDoArquivo){
	int i=0,j=0;
	char buffer[1024] ;
	char *record, *line;
	double data[3][1024];
	int quantidadeDeItems[3];

	FILE *fp = fopen(nomeDoArquivo,"r");
	if(fp == NULL)
	{
		erro(ERRO_FALHA_NO_ARQUIVO);
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

	return entrada;
}

void freeEntrada(struct Entrada* entrada) {
	free(entrada->X);
	free(entrada->Y);
	free(entrada->F);
}

void imprimeEntrada (FILE* fp, struct Entrada *entrada) {
	fprintf(fp, "Entrada: \n");
	fprintf(fp, "X: ");
	imprimeArray(fp, entrada->X, entrada->n);
	fprintf(fp, "Y: ");
	imprimeArray(fp, entrada->Y, entrada->n);
	fprintf(fp, "F: ");
	if (entrada->nF != 0) {
		imprimeArray(fp, entrada->F, entrada->nF);
	} else {
		fprintf(fp, "[]");
	}
	fprintf(fp, "\n");
}

void imprimeArray(FILE* fp, double *A, int size_A){
	int i;
	fprintf(fp, "[ ");
	for (i = 0; i < size_A-1; ++i) {
		fprintf(fp, "%lf, ", A[i]);
	}
	fprintf(fp, "%lf ", A[i]);
	fprintf(fp, "]\n");
}

void imprimeArray2D(FILE* fp, double** A, int linhas, int colunas){
	int i;
	fprintf(fp, "[\n");
	for (i = 0; i < linhas; ++i) {
		imprimeArray(fp, A[i], colunas);
	}
	fprintf(fp, "]\n");
}
