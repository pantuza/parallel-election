#include "words.h"


int word_count(FILE *in)
{
	char string[1024];
	int q = 0;

	while (fscanf(in, "%s %s", string, string) == 2)
        q++;
	
    rewind(in);
	return q;
}


int compare(char *p1, char *p2)
{
	int i;
	
	for (i = 0; i < PMAX; i++)
	{
		if (p1[i] == '\0' && p1[i] == '\0') 
            return 0;
		if (p1[i] == p2[i])
            continue;
		if (p1[i] > p2[i])
			return -1;
		else
			return 1;
	}
	return 0;
}


words *load_words(FILE *in)
{
	words *dict;
	int i, j;

	dict = malloc(sizeof(words));
	dict->n = word_count(in);
	dict->p = calloc(dict->n, sizeof(word));
	
    for(i = 0; (i < (dict->n)) && (!feof(in)); i++)
	{
		fscanf(in, "%d %s", &(dict->p[i].valor), dict->p[i].string);
        
        /* Turn everything into lower case */
		for(j = 0; j < PMAX && dict->p[i].string[j] != '\0'; j++)
		
        if(dict->p[i].string[j] < 91 && dict->p[i].string[j] > 64)
				dict->p[i].string[j] += 32;
	}
	return dict;
}


void print_words(words *dict)
{
	int i;
	for (i = 0; i < (dict->n); i++)
		fprintf(stdout, "%d %s\n", dict->p[i].valor, dict->p[i].string);
}


int word_value_l(words *dict, char *word)
{
	int i;
	
	for(i = 0; i < (dict->n); i++)
		if(!compare(dict->p[i].string, word))
            return dict->p[i].valor;
	return 0;
}

int word_value_bo(words *dict, char *word)
{
	int factor, pos, last;

	pos = (dict->n) - 1;
	factor = pos;
	last = 0;

	while((pos > 0) && (pos < (dict->n)))
	{
		if(factor != 1)
            factor = factor / 2;

		switch(compare(dict->p[pos].string, word))
		{
			case -1:
				if(factor == 1)
				{
					if(last == 1) return 0;
					last =-1;
				}
				pos -= factor;
				break;
			case 0:
				return dict->p[pos].valor;
			case 1:
		    	if(factor == 1)
				{
					if(last == -1) return 0;
					last = 1;
				}
				pos += factor;
		}
	}
	return 0;	
}
