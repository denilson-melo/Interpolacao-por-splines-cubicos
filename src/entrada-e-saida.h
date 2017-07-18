#ifndef _ENTRADA_E_SAIDA_H_
#define _ENTRADA_E_SAIDA_H_
#include <stdlib.h>
#include <stdio.h>

struct Entrada {
	size_t n;    // Numero de pontos detectado
	double* X;   // Coordenadas X
	double* Y;   // Coordenadas Y
	size_t nF;   // Numero de valores x a serem avaliados com spline
	double* F;   // Valores a serem avaliados com spline
};

// Lê o arquivo de entrada com o caminho fornecido e retorna
// um ponteiro pra struct Entrada preenchido com os dados lidos
struct Entrada* lerArquivo(char *nomeDoArquivo);

// Imprime os dados de entrada no fluxo de saída
void imprimeEntrada (FILE* fp, struct Entrada *entrada);

// Libera os espaços de memória alocados para a struct Entrada
void freeEntrada (struct Entrada *entrada);

// Imprime o array unidimensional no fluxo de saída
void imprimeArray(FILE* fp, double* A, size_t n);

// Imprime o array bidimensional no fluxo de saída
void imprimeArray2D(FILE* fp, double** A, size_t linhas, size_t colunas);

#endif
