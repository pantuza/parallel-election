#include <iostream>
#include "sentiment_analyser.h"
#include "words.h"
#define TMAX 150

class tweet{
public:
	char t[TMAX];
	void load_from_file(FILE *in);
};

int count_lines(FILE *in);
int *analyse_target(words *dict, int Ncandidates, words **candidate_tags, int Ntweet, tweet *t);