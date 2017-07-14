#include <math.h>
#include "derivacao-e-integracao.h"
#include "spline.h"

double deriva(struct Spline *spl, float x, float h, int ordem){
	switch (ordem) {
		case 1:
			return (1/(2*h))*(-estimaValor(x - h, spl) + estimaValor(x + h, spl));
			break;
		case 2:
			return (1/pow(h,2))*(estimaValor(x - h, spl) - 2*estimaValor(x, spl) + estimaValor(x + h, spl));
			break;
		default:
			return 123456;
			
	}
}

double integra(struct Spline *spl, float a, float b, int n){
	int i;
	double h = (b-a)/n;
	double E = 0;
	for (i = 1; i < n; ++i) {
		if (i%2 == 0) {
			E += 4*estimaValor(i*h, spl); 
		} else {
			E += 2*estimaValor(i*h, spl);       
		}
	}
	return (h/3)*(estimaValor(a, spl) + E + estimaValor(b, spl));
}
