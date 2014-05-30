#ifndef WORDS_H
#define WORDS_H


#include <stdio.h>
#include <stdlib.h>


#define PMAX 32	/* maximum size of a word */


typedef struct word {
	char string[PMAX];
	int  valor;
} word;


typedef struct words {
	word *p;
	int n;
} words;


/* Load all words from 'in' and appends to 'words' */
words *load_words(FILE *in);


/**
	Compare two words and returns:
	-1 if p1 > p2
	1 if p1 < p2
	0 if p1 == p2
*/
int compare(char *p1,char *p2);


// Search the word in dict and returns its polarity or 0 case not found
int word_value_l(words *dict, char *word);	// linear approach
int word_value_bo(words *dict, char *word);	// sorted approach (logarithmic)

void print_words(words *dict);

#endif /* WORDS_H */
