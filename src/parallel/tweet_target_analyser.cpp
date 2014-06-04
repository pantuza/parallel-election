#include <iostream>
#include "tweet_target_analyser.h"

using namespace std;

void analyse_target(words *dict, int Ncandidates, words **candidate_tags, int Ntweet,tweet *t,int *result)
{
	int i,j,sentiment;

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
	return;
}

int tweet::load_from_file(FILE *in)
{
	int i;

	i = 0;
	if (feof(in))
	{
		t[0] = '\0';
		return -1;
	}
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
	return 0;
}
