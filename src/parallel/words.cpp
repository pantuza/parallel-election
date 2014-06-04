#include "words.h"


int count_words(FILE *in)
{
	int number_of_lines;
	char c[1024];
	for (number_of_lines = 0; 2 == fscanf(in, "%s %s", c, c); number_of_lines++);
	rewind(in);
	return number_of_lines;
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


words::words(int Nwords)
{
	n = Nwords;
	p = new word[n];
}

void words::fload_words(FILE *in)
{
	int i, j;

    for(i = 0; (i < n) && (!feof(in)); i++)
	{
		fscanf(in, "%d %s", &(p[i].valor), p[i].string);
        
        /* Turn everything into lower case */
		for(j = 0; j < PMAX && p[i].string[j] != '\0'; j++)
			if(p[i].string[j] < 91 && p[i].string[j] > 64)
				p[i].string[j] += 32;
	}
	return;
}


void words::print_words()
{
	int i;
	printf("\n Dic print\n");
	for (i = 0; i < n; i++)
		fprintf(stdout, "%d %s\n", p[i].valor, p[i].string);
}


int words::word_value_la(char *word)
{
	int i;
	
	for(i = 0; i < n; i++)
		if(!compare(p[i].string, word))
            return p[i].valor;
	return 0;
}

int words::word_value_sa(char *word)
{
	int factor, pos, last;

	pos = n - 1;
	factor = pos;
	if (!factor)factor = 1;
	last = 0;
	while((pos > -1) && (pos < n))
	{
		if(factor != 1)
            factor = factor / 2;

		switch(compare(p[pos].string, word))
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
				return p[pos].valor;
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
