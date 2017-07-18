#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include "spline.h"
#include "erro.h"
#include "sela.h"
#include "entrada-e-saida.h"

struct Spline* criaSpline(struct Entrada* entrada){
	int i;
	int n = entrada->n;

	// Ponteiros para encurtar a sintaxe
	double* X = entrada->X;
	double* Y = entrada->Y;


	// Aloca matriz do sistema de equações
	double** A = (double **)malloc((n-2) * sizeof(double*));
	if (A == NULL) {
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		return NULL;
	}
	for (i = 0; i < (n-2); ++i) {
		A[i] = (double *)calloc((n-1), sizeof(double));
		if (A[i] == NULL) {
			erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
			for (; i >= 0; --i)
				free(A[i]);
			free(A);
			return NULL;
		}
	}

	// Ponteiro para facilitar navegação na matriz
	double* a;

	// Preenche a matriz do sistema de equações
	// A segunda derivada no primeiro ponto é 0, portanto não entra no sistema
	i = 1;
	a = A[i-1];
	a[i-1] = 2*(X[i+1] - X[i-1]);
	a[i] = (X[i+1] - X[i]);
	a[n-2] = 6/(X[i+1] - X[i])*(Y[i+1] - Y[i]) + 6/(X[i] - X[i-1])*(Y[i-1] - Y[i]);
	for (i = 2; i < n-2; ++i) {
		a = A[i-1];
		a[i-2] = X[i] - X[i-1];
		a[i-1] = 2*(X[i+1] - X[i-1]);
		a[i] = (X[i+1] - X[i]);
		a[n-2] = 6/(X[i+1] - X[i])*(Y[i+1] - Y[i]) + 6/(X[i] - X[i-1])*(Y[i-1] - Y[i]);
	}
	// A segunda derivada no ultimo ponto também é 0, também não entra no sistema
	i = n-2;
	a = A[i-1];
	a[i-2] = X[i] - X[i-1];
	a[i-1] = 2*(X[i+1] - X[i-1]);
	a[n-2] = 6/(X[i+1] - X[i])*(Y[i+1] - Y[i]) + 6/(X[i] - X[i-1])*(Y[i-1] - Y[i]);

	// Array que irá armazenar as segundas derivadas de cada ponto
	// É alocado com calloc pois será passado como a estimativa incial
	// para o método de Gauss-Seidel, sendo vetor x
	// para a resolução do sistema Ax = B.
	double* F2 = calloc(n, sizeof(double));
	if (F2 == NULL) {
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		for (i = 0; i < n-2; ++i)
			free(A[i]);
		free(A);
		return NULL;
	}

	struct Spline *spl = malloc(sizeof(struct Spline));
	spl->entrada = entrada;
	spl->retornoGaussSeidel = gaussSeidel(
			A,     //Matriz do sistema
			n-2,   //Ordem
			F2+1,  //Estimativa inicial
			  // Aqui é utilizada aritmética de ponteiros pois
			  // a matriz do sistema é de ordem n-2, pois dos n pontos
			  // 2 derivadas já são conhecidas, a do primeiro e ultimo ponto.
			  // Portanto a primeira a ultima posição deste array será 0,
			  // e as demais será preenchida pela resolução do sistema
		    1,       //Relaxamento(w)
			DBL_MIN, //Tolerancia
			10000);  //Máximo de iterações

	// Aloca espaço para os coeficientes do spline
	// São alocadas n-1 linhas, referentes aos intervalos
	// São alocados 4 colunas referentes a cada um dos coeficientes
	// [a,b,c,d]: a(Xi - x)³ + b(x - Xi-1)³ + c(Xi - x) + d(x - Xi-1)
	spl->coeficientes = (double **)malloc((n-1) * sizeof(double *));
	if (spl->coeficientes == NULL) {
		erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
		free(F2);
		for (i = 0; i < n-2; ++i)
			free(A[i]);
		free(A);
		return NULL;
	}
	for (i = 0; i < n-1; ++i) {
		spl->coeficientes[i] = (double *)malloc(4 * sizeof(double));
		if (spl->coeficientes[i] == NULL) {
			erro(ERRO_DE_ALOCACAO_DE_MEMORIA);
			for (; i >= 0; --i)
				free(spl->coeficientes[i]);
			free(F2);
			for (i = 0; i < n-2; ++i)
				free(A[i]);
			free(A);
			return NULL;
		}
	}

	// Preenche os coeficientes do spline
	for (i = 1; i < n; ++i) {
		spl->coeficientes[i-1][0] = F2[i-1]/(6*(X[i] - X[i-1]));
		spl->coeficientes[i-1][1] = F2[i]/(6*(X[i] - X[i-1]));
		spl->coeficientes[i-1][2] = Y[i-1]/(X[i] - X[i-1]) - F2[i-1]*(X[i] - X[i-1])/6;
		spl->coeficientes[i-1][3] = Y[i]/(X[i] - X[i-1]) - F2[i]*(X[i] - X[i-1])/6;
	}


	for (i = 0; i < n-2; ++i) {
		free(A[i]);
	}
	free(A);
	free(F2);
	return spl;
}

double estimaValor(struct Spline* spl, float x){
	int i;
	double* X = spl->entrada->X;
	// Encontra o intervalo a qual x percente
	for (i = 0; i < spl->entrada->n-1; ++i) {
		if (x <= X[i+1]) {
			i += 1;
			break;    
		}
	}
	return (       
		spl->coeficientes[i-1][0] * pow(X[i] - x, 3)
		+ spl->coeficientes[i-1][1] * pow(x - X[i-1],3)
		+ (X[i] - x)* spl->coeficientes[i-1][2]
		+ (x - X[i-1])* spl->coeficientes[i-1][3]
	);
}

void imprimeSpline (FILE* fp, struct Spline* spl){
	fprintf(fp, "Forma do polinomio de cada intervalo:\n");
	fprintf(fp, "a(Xi - x)³ + b(x - Xi-1)³ + c(Xi - x) + d(x - Xi-1)\n");
	fprintf(fp, "Coeficientes [a,b,c,d] do spline cubico:\n");
		imprimeArray2D(fp, spl->coeficientes, spl->entrada->n-1, 4);
	fprintf(fp, "\n");
	fprintf(fp, "O metodo de Gauss-Seidel foi executado com:\n");
		fprintf(fp, "Iteracoes: %d \n", spl->retornoGaussSeidel->iteracoes);
		fprintf(fp, "Erro: %.31f \n", spl->retornoGaussSeidel->erro);
	fprintf(fp, "\n");
}

void freeSpline (struct Spline* spl){
	int i;
	for (i = 0; i < spl->entrada->n-1; ++i) {
		free(spl->coeficientes[i]);
	}
	free(spl->coeficientes);
	free(spl);
}
