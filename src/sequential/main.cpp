#include <stdio.h>
#include <iostream>
#include "words.h"
#include "sentiment_analyser.h"
#include "tweet_target_analyser.h"

using namespace std;

int main(int argc, char *argv[])
{
	FILE *in;
	int i,Ntweets, Ntargets,*result;
	words *dict,**candidate_tag;
	tweet *twt;
	
	if (argc < 3)
	{
		cout <<endl<< "expected input files:"<<endl<<"./exec <tweets> <dictionary> <target_dic1> <target_dic2> ..." << endl;
		return-1;
	}
	in = fopen(argv[1], "r");
//Load tweets
	cout <<endl<< "Loading tweets...";
	Ntweets = count_lines(in);
	twt = new tweet[Ntweets];
	for (i = 0; i < Ntweets; i++)
	{
		twt[i].load_from_file(in);
		//fgets(twt[i].t, TMAX, in);
	}
	fclose(in);
//Load dictionary
	cout << endl << "Loading dictionaries...";
	in = fopen(argv[2], "r");
	dict = new words(word_count(in));
	dict->fload_words(in);
	fclose(in);
//Load targets tags
	Ntargets = argc - 3;
	candidate_tag = new words*[Ntargets];
	for (i = 0; i < Ntargets; i++)
	{
		in = fopen(argv[i+3], "r");
		candidate_tag[i] = new words(word_count(in));
		candidate_tag[i]->fload_words(in);
		fclose(in);
	}
//
cout<<endl<<"computing ...";
	result = analyse_target(dict, Ntargets, candidate_tag, Ntweets, twt);
	for (i = 0; i < Ntargets; i++)
		printf("Candidate: %d\n\tPositive: %d\tNegative: %d\n", i, result[i * 2], result[i * 2+1]);
	delete(dict);
	for (i = 0; i < Ntargets; i++)
		delete(candidate_tag[i]);
	delete(candidate_tag);
    return EXIT_SUCCESS;
}
