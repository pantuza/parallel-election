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


void tweet::load_from_file(FILE *in)
{
	int i;
	i = 0;
	while( i < TMAX)
	{
		t[i] = fgetc(in);
		if (t[i] == ',' || t[i] == ';' || t[i] == '!' || t[i] == '?' || t[i] == '#')
			t[i] = ' ';
		if (t[i] == ' ')	//remove extra spaces
		{
			i++;
			t[i] = fgetc(in);
			while (t[i] == ' ')t[i] = fgetc(in);
		}
		if (t[i] == '\n' || t[i] == '\0')break;
		if (t[i] < 91 && t[i] > 64)
			t[i] += 32;
		i++;
	}
	t[i] = '\0';
	t[TMAX - 1] = '\0';
	return;
}