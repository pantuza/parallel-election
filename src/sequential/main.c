#include <stdio.h>


#include "words.h"
#include "sentiment_analyser.h"


int main(int argc, char *argv[])
{
	FILE *in,*out;

	words *dict;
	char string[256];
	
	in = fopen(argv[1], "r");
	out = fopen(argv[2], "w");
	
    
    if (!in)
	{
		fprintf(stdout, "A dictionary file must be passed\n");
		return EXIT_FAILURE;
	}

	
    dict=carregar_palavras(in);
	while (1)
	{
		fprintf(stdout, "\nEnter a phrase or 0 for exit.\nPhrase: ");
		fgets(string, 256, stdin);

		if(string[0] == '0') 
            return EXIT_SUCCESS;


		fprintf(stdout, "polarity of the sentence: %d\n",
                analyse(dict, string, out));
	}

    return EXIT_SUCCESS;
}
