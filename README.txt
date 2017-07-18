Para compilar o projeto basta utilizar o comando 'make' no terminal:
$ make

Caso o makefile n�o esteja dispon�vel utilize a seguinte linha de compila��o (estando na pasta base do projeto):
gcc -o projeto2 src/entrada-e-saida.c src/spline.c src/sela.c src/erro.c src/main.c src/derivacao-e-integracao.c -lm -Wall

O projeto pode ser executado com (estando na pasta base do projeto):
./projeto2 <arquivo>
onde <arquivo> � o caminho do arquivo de entrada.

Exemplos de execu��o:
$ ./projeto2 entradas/entrada1.csv
$ ./projeto2 entradas/entrada2.csv

A sa�da ser� em um arquivo na mesma pasta e com o mesmo nome que o arquivo de sa�da por�m com o sufixo '.out'.
Por exemplo, com o arquivo 'entradas/entrada1.csv' o programa ir� gerar a sa�da 'entradas/entrada1.csv.out'





Este projeto foi desenvolvido com multiplos arquivos, iremos descrever a finalidade da cada um:
main(.c)                      - Fluxo principal do programa
entrada-e-saida(.c .h)        - Leitura e escrita no arquivo
spline(.c .h)                 - Cria��o da interpola��o por slines c�bicos
sela(.c .h)                   - Resolu��o de sistemas de equa��es lineares
derivacao-e-integracao(.c .h) - Deriva��o e Integra��o num�rica
erro(.c .h)                   - Padroniza��o das mensagens de erro

Recomendamos ler primeiro os arquivos de cabe�alho (.h) para coompreender a vis�o geral do funcionamento do projeto.
Nos arquivos .c s�o comentados apenas os detalhes de implementa��o de cada fun��o.
Sugerimos tamb�m que os arquivos sejam corrigidos nesta ordem:
1 - main.c
2 - entrada-e-saida.h
3 - spline.h
4 - sela.h
5 - derivacao-e-integracao.h
6 - erro.h
7 - main.c