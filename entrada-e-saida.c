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
		if (i > 0 && j != data[0][0]) {
			erro(ERRO_NUMERO_DE_PONTOS_DIFERENTES);
			return NULL;
		}
		++i;
	}
	fclose(fp);

	struct Entrada *entrada = malloc(sizeof(struct Entrada));
	if (entrada == NULL) {
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}
	entrada-> n = (int)data[0][0];

	entrada->X = malloc(entrada->n * sizeof(double));
	if (entrada->X == NULL) {
		free(entrada);
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}

	entrada->Y = malloc(entrada->n * sizeof(double));
	if (entrada->Y == NULL) {
		free(entrada->X);
		free(entrada);
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}
	for (i = 0; i < entrada->n; ++i) {
		entrada->X[i] = data[1][i];
		entrada->Y[i] = data[2][i];
	}
	imprimeEntrada(entrada);
	return entrada;
}

void imprimeEntrada (struct Entrada *entrada) {
	printf("n: %d\n", entrada->n);
	printf("X: ");
	imprimeArray(entrada->X, entrada->n);
	printf("Y: ");
	imprimeArray(entrada->Y, entrada->n);
}
