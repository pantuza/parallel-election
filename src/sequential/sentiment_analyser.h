#include "palavras.h"
#define SMAX 512 //Tamanho máximo da string

int analisar(palavras *dicionario, char *frase,FILE *out);
/*
	Analisa uma frase e devolve a polaridade:
		Quanto mais distante de 0 negativamente, mais negativo é o sentimento
		Quanto mais distante de 0 positivamente, mais positivo é o sentimento	 
	Se o arquivo "out" for válido, ele imprimirá as palavras não encontradas no dicionário
*/
