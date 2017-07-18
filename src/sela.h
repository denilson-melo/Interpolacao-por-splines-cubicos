#ifndef _SELA_H_
#define _SELA_H_

#include <stdlib.h>

// Estrutura para ser retornada na execução do método de GaussSeidel
struct RetornoGaussSeidel {
	int iteracoes; //Numero de iterações
	double erro;   //Erro
};


// Método de Gauss-Seidel para a resolução de Sistemas de Equações Lineares Algébricas
struct RetornoGaussSeidel* gaussSeidel(
		double** A,             // Matriz do sistema
		size_t n,               // Ordem da matriz
		double* estimativa,     // Estimativa inicial
		                          // A solução do SELA será feita diretamente neste array
		double w,               // Relaxamento
		double tolerancia,    
		int maximoDeIteracoes);

// Calcula o erro entre dois vetores considerando a norma máxima
double erroNormaMaxima(double* Xold, double* X, size_t size);

#endif
