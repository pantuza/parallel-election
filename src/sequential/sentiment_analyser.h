#ifndef SENTMENT_ANALYSER_H
#define SENTMENT_ANALYSER_H


#include "words.h"


#define SMAX 512 /* maximum size of strings  */


int analyse(words *dict, char *string, FILE *out);

/*
	Analisa uma frase e devolve a polaridade:
		Quanto mais distante de 0 negativamente, mais negativo é o sentimento
		Quanto mais distante de 0 positivamente, mais positivo é o sentimento	 
	Se o arquivo "out" for válido, ele imprimirá as palavras não encontradas no dicionário
*/

#endif /* SENTMENT_ANALYSER_H */
