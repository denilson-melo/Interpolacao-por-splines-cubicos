#ifndef _DERIVACAO_E_INTEGRACAO_H
#define _DERIVACAO_E_INTEGRACAO_H

#include "spline.h"

// Derivação numérica pela fórmula de 3 pontos centrada
double deriva(struct Spline *spl,
		float x,  // Ponto
		double h, // h da derivação
		int ordem // Ordem 1 ou 2
		);

// Integração numérica pela fórmula de Simpson composta
double integra(struct Spline *spl,
		float a, // Início do intervalo
		float b, // Fim do intervalo
		int n    // Numero de subdivisões);
		);

#endif
