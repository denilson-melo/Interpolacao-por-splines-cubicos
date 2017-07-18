Para compilar o projeto basta utilizar o comando 'make' no terminal:
$ make

Caso o makefile não esteja disponível utilize a seguinte linha de compilação (estando na pasta base do projeto):
gcc -o projeto2 src/entrada-e-saida.c src/spline.c src/sela.c src/erro.c src/main.c src/derivacao-e-integracao.c -lm -Wall

O projeto pode ser executado com (estando na pasta base do projeto):
./projeto2 <arquivo>
onde <arquivo> é o caminho do arquivo de entrada.

Exemplos de execução:
$ ./projeto2 entradas/entrada1.csv
$ ./projeto2 entradas/entrada2.csv

A saída será em um arquivo na mesma pasta e com o mesmo nome que o arquivo de saída porém com o sufixo '.out'.
Por exemplo, com o arquivo 'entradas/entrada1.csv' o programa irá gerar a saída 'entradas/entrada1.csv.out'





Este projeto foi desenvolvido com multiplos arquivos, iremos descrever a finalidade da cada um:
main(.c)                      - Fluxo principal do programa
entrada-e-saida(.c .h)        - Leitura e escrita no arquivo
spline(.c .h)                 - Criação da interpolação por slines cúbicos
sela(.c .h)                   - Resolução de sistemas de equações lineares
derivacao-e-integracao(.c .h) - Derivação e Integração numérica
erro(.c .h)                   - Padronização das mensagens de erro

Recomendamos ler primeiro os arquivos de cabeçalho (.h) para coompreender a visão geral do funcionamento do projeto.
Nos arquivos .c são comentados apenas os detalhes de implementação de cada função.
Sugerimos também que os arquivos sejam corrigidos nesta ordem:
1 - main.c
2 - entrada-e-saida.h
3 - spline.h
4 - sela.h
5 - derivacao-e-integracao.h
6 - erro.h
7 - main.c