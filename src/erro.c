#include <stdio.h>
#include "erro.h"

void erro(int e){
	printf("ERRO:\n");
	switch (e){
		case ERRO_FALHA_NO_ARQUIVO:
			printf("Falha ao abrir arquivo\n");
			break;
		case ERRO_ENTRADA_VAZIA:
			printf("Entrada vazia\n");
			break;
		case ERRO_NUMERO_DE_PONTOS_DIFERENTES:
			printf("Quantidade de pontos nao coicidem\n");
			break;
		case ERRO_DE_ALOCACAO_DE_MEMORIA:
			printf("Erro de alocação de memória\n");
			break;
	}
}
