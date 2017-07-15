#ifndef _SPLINE_H_
#define _SPLINE_H_

#include <stdio.h>
#include "entrada-e-saida.h"

struct Spline {
	double **polinomios;
	struct Entrada *entrada;
};

struct Spline* criaSpline(struct Entrada *entrada);
double estimaValor (struct Spline *spl, float x);
void imprimeSpline (FILE* fp, struct Spline *spl);
void freeSpline (struct Spline *spl);

#endif
