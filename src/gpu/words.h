#ifndef WORDS_H
#define WORDS_H


#include <stdio.h>
#include <stdlib.h>


#define PMAX 32	/* maximum size of a word */


typedef struct word {
public:
	char string[PMAX];
	int  valor;
} word;


class words {
public:
	word *p;
	int n;
	words(int Nwords);
	void fload_words(FILE *in); /* Load all words from 'in' and appends to 'words' */
	void print_words();
	// Search the word in dict and returns its polarity or 0 case not found:
	int word_value_la(char *word);// linear approach
	int word_value_sa(char *word);// sorted approach (logarithmic)
} ;


/**
	Compare two words and returns:
	-1 if p1 > p2
	1 if p1 < p2
	0 if p1 == p2
*/
int compare(char *p1,char *p2);
int count_words(FILE *in);

#endif /* WORDS_H */
