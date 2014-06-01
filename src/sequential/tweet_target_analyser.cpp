#include <iostream>
#include "tweet_target_analyser.h"

using namespace std;

int *analyse_target(words *dict, int Ncandidates, words **candidate_tags, int Ntweet,tweet *t)
{
	int i,j,sentiment,*result;

	result = new int[Ncandidates*2];
	for (i = 0; i < 2 * Ncandidates; i++)
		result[i] = 0;
	for (i = 0; i < Ntweet; i++)
	{
		sentiment = analyse(dict, t[i].t);
		if (sentiment)
		{
			for (j = 0; j < Ncandidates; j++)
			{
				if (analyse(candidate_tags[j], t[i].t))
				{
					if (sentiment > 0)
						result[j * 2]++;
					else
						result[j * 2+1]++;
				}
			}
		}
	}
	return result;
}

int count_lines(FILE *in)
{
	int number_of_lines;
	char c;

	number_of_lines = 0;
	while (EOF != (c = getc(in)))
		if ('\n' == c)
			++number_of_lines;
	rewind(in);
	return number_of_lines;
}