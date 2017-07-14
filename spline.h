#ifndef _SPLINE_H_
#define _SPLINE_H_

#include "entrada-e-saida.h"

struct Spline {
	double **polinomios;
	struct Entrada *entrada;
};

struct Spline* criaSpline(struct Entrada *entrada);
void imprimeSpline (struct Spline *spl);
double estimaValor(float x, struct Spline *spl);

#endif
