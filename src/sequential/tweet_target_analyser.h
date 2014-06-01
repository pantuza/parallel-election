#include <iostream>
#include "sentiment_analyser.h"
#include "words.h"
#define TMAX 150

class tweet{
public:
	char t[150];
};

int count_lines(FILE *in);

int *analyse_target(words *dict, int Ncandidates, words **candidate_tags, int Ntweet, tweet *t);
/*
	it analyzes tweets and returns two elements for each target in an array of results.
	First is the sum of positive sentiments and second is the sum of negatives.
*/
