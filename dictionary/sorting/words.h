#ifndef _PALAVRAS
#include <stdio.h>
#include <stdlib.h>
#define _PALAVRAS

#define PMAX 32	//Tamanho máximo da palavra

typedef struct palavra{
	char string[PMAX];
	int  valor;
}palavra;

typedef struct palavras{
	palavra *p;
	int n;

}palavras;

palavras *carregar_palavras(FILE *in);
// Carrega todas as palavras do arquivo "in" e devolve em "*palavras"

int comparar_palavra(char *p1,char *p2);
/*
	Compara duas palavras e devolve:
	-1 se p1>p2
	1 se p1<p2
	0 se p1 == p2
*/
int valor_palavra_l(palavras *dicionario, char *palavra);
// Procura a palavra em "dicionario" e devolve a polaridade ou 0 caso não encontre.

int valor_palavra_b(palavras *dicionario, char *palavra);

void imprime_palavras(palavras *dicionario);

#endif
