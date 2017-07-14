#ifndef _DERIVACAO_E_INTEGRACAO_H
#define _DERIVACAO_E_INTEGRACAO_H

#include "spline.h"

double deriva(struct Spline *spl, float x, float h, int ordem);
double integra(struct Spline *spl, float a, float b, int n);

#endif
