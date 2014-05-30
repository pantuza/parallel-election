
#include <stdio.h>
#include <stdlib.h>


#include "words.h"


int main(int argc, char *argv[])

{
	FILE *in, *out;
	int i, j;
	palavra e;
	words *dict;
	char string[256];
	
	in = fopen(argv[1], "r");
	if(!in)
        return 404;

	sprintf(string, "%s_ordenado", argv[1]);
	out = fopen(string, "w");
	dict = load_words(in);

    /* insertion */
	for(i = 1; i < dict->n; i++)
	{
		e = dict->p[i];
		j = i - 1;
		while((j >= 0) && (compare(e.string, dict->p[j].string) > 0))
		{
			dict->p[j+1] = dict->p[j];
			j--;
		}
		dict->p[j+1] = e;
	}

	for (i = 0; i < dict->n; i++)
		fprintf(out, "%d\t%s\n", dict->p[i].valor, dict->p[i].string);
	
    fclose(in);
	fclose(out);

    return EXIT_SUCCESS;
}
