#ifndef _ERRO_H_
#define _ERRO_H_

// Códigos de erro

#define ERRO_FALHA_NO_ARQUIVO 1
#define ERRO_ENTRADA_VAZIA 2
#define ERRO_NUMERO_DE_PONTOS_DIFERENTES 3
#define ERRO_DE_ALOCACAO_DE_MEMORIA 4
#define ERRO_NA_LEITURA_DA_ENTRADA 5
#define ERRO_NA_CRIACAO_DO_SPLINE 6

// Exibe uma mensagem de erro de acordo com o código de erro passado
void erro(int);

#endif
