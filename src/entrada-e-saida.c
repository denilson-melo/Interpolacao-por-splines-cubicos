#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "entrada-e-saida.h"
#include "erro.h"

struct Entrada* lerArquivo(char *nomeDoArquivo){

	FILE *fp = fopen(nomeDoArquivo,"r");
	if(fp == NULL)
	{
		erro(ERRO_FALHA_NO_ARQUIVO);
		return NULL;
	}

	int i = 0, j = 0;
	char buffer[1024];        // Buffer para ler linhas do arquivo
	double data[3][1024];     // Onde os dados de entrada serão armazenados,
							  // cada linha corresponde a uma linha do arquivo de entrada

	int quantidadeDeItems[3]; // Quantidade de items de cada linha lida

	char *elemento, *linha;   // Ponteiros auxiliares

	// Para cada linha, coleta cada elemento separado por ';' como double
	// e armazena em data
	while((linha=fgets(buffer, sizeof(buffer),fp))!=NULL)
	{
		elemento = strtok(linha,";");
		j = 0;
		while(elemento != NULL)
		{
			data[i][j++] = strtod(elemento, NULL);
			elemento = strtok(NULL,";");
		}
		quantidadeDeItems[i] = j;
		++i;
	}
	fclose(fp);

	// Aborta caso não seja fornecido nenhum ponto
	if (quantidadeDeItems[0] == 0) {
		erro(ERRO_ENTRADA_VAZIA);
		return NULL;
	}

	// Aborta caso o numero de coordenadas X e Y sejam diferentes
	if (quantidadeDeItems[0] != quantidadeDeItems[1]) {
		erro(ERRO_NUMERO_DE_PONTOS_DIFERENTES);
		return NULL;
	}

	// Cria a estrutura de entrada que irá armazenar os dados
	struct Entrada *entrada = malloc(sizeof(struct Entrada));
	if (entrada == NULL) {
		freeEntrada(entrada);
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}
	entrada->X = entrada->Y = entrada->F = NULL;

	// Armazena o numero de pontos
	entrada->n = quantidadeDeItems[0];

	// Aloca espaço para o array de coordenadas X
	entrada->X = malloc(entrada->n * sizeof(double));
	if (entrada->X == NULL) {
		freeEntrada(entrada);
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}

	// Aloca espaço para o array de coordenadas Y
	entrada->Y = malloc(entrada->n * sizeof(double));
	if (entrada->Y == NULL) {
		freeEntrada(entrada);
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}

	// Armazena a quantidade de valores a serem avaliados
	entrada->nF = quantidadeDeItems[2];

	// Aloca espaço para o array de F de valores a serem avaliados
	if (entrada->nF != 0) {
		entrada->F = malloc(entrada->nF * sizeof(double));
		if (entrada->F == NULL) {
			freeEntrada(entrada);
			erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
			return NULL;
		}
	}

	// Preenche os arrays de coordenadas X e Y
	for (i = 0; i < entrada->n; ++i) {
		entrada->X[i] = data[0][i];
		entrada->Y[i] = data[1][i];
	}

	// Preenche o array de valores a serem avaliados
	for (i = 0; i < entrada->nF; ++i) {
		entrada->F[i] = data[2][i];
	}

	// Retorna o ponteiro para a estrutura preenchida
	return entrada;
}

void freeEntrada(struct Entrada* entrada) {
	free(entrada->X);
	free(entrada->Y);
	if (entrada->nF > 0) {
		free(entrada->F);
	}
	free(entrada);
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

void imprimeArray(FILE* fp, double *A, size_t n){
	int i;
	fprintf(fp, "[ ");
	for (i = 0; i < n-1; ++i) {
		fprintf(fp, "%lf, ", A[i]);
	}
	fprintf(fp, "%lf ", A[i]);
	fprintf(fp, "]\n");
}

void imprimeArray2D(FILE* fp, double** A, size_t linhas, size_t colunas){
	int i;
	fprintf(fp, "[\n");
	for (i = 0; i < linhas; ++i) {
		imprimeArray(fp, A[i], colunas);
	}
	fprintf(fp, "]\n");
}
