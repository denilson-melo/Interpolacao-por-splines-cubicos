#ifndef _SPLINE_H_
#define _SPLINE_H_

#include <stdio.h>
#include "entrada-e-saida.h"

struct Spline {
	double **coeficientes;     // Coeficientes do splines
	struct Entrada *entrada;   // Entrada utilizada
	// Retorno do metodo Gauss-Seidel
	struct RetornoGaussSeidel *retornoGaussSeidel;
};

// Efetua a intporlação por splines cubicos com a entrada fornecida
// Esta função preenche todos os campos da struct Spline
struct Spline* criaSpline(struct Entrada *entrada);

// Utiliza a função spline para estimar um valor de x
double estimaValor (struct Spline *spl, float x);

// Imprime os coeficientes do spline no arquivo
void imprimeSpline (FILE* fp, struct Spline *spl);

// Libera os espaços de memória alocados para o spline
void freeSpline (struct Spline *spl);

#endif
