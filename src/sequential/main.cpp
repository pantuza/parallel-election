#include <stdio.h>
#include <iostream>
#include <time.h>
#include "words.h"
#include "sentiment_analyser.h"
#include "tweet_target_analyser.h"
#define BLOCKSIZE	1048576

using namespace std;

FILE *openfile(const char * filename, const char * mode)
{
	FILE *f;
	f = fopen( filename,  mode);
	if (!f)cout<<endl << "Failed to load file" << filename << endl;
	return f;
}

int main(int argc, char *argv[])
{
	FILE *in;
	int i,j,Ntweets, Ntargets,*result;
	words *dict,**candidate_tag;
	tweet *t;
	float timestart;
	
	timestart=(float)clock()/CLOCKS_PER_SEC;
	if (argc < 3)
	{
		cout <<endl<< "expected input files:"<<endl<<"./exec <tweets> <dictionary> <target_dic1> <target_dic2> ..." << endl;
		return-1;
	}
//Load dictionary
	in = openfile(argv[2], "r");
	if (!in)return EXIT_FAILURE;
	dict = new words(count_words(in));
	dict->fload_words(in);
	fclose(in);
//Load targets tags
	Ntargets = argc - 3;
	candidate_tag = new words*[Ntargets];
	for (i = 0; i < Ntargets; i++)
	{
		in = openfile(argv[i+3], "r");
		if (!in)return EXIT_FAILURE;
		candidate_tag[i] = new words(count_words(in));
		candidate_tag[i]->fload_words(in);
		fclose(in);
	}
	in = openfile(argv[1], "r");
	if (!in)return EXIT_FAILURE;
	Ntweets = BLOCKSIZE;
//Create resul array
	result = new int[Ntargets*2];
	for(i=0;i<Ntargets*2;i++)
		result[i]=0;
//Execute
	t= new tweet[Ntweets];
	while(!feof(in))
	{
		for(j=0;j< Ntweets; j++)
			t[j].load_from_file(in);
		analyse_target(dict, Ntargets, candidate_tag, Ntweets, t, result);
	}
//print results:
	for (i = 0; i < Ntargets; i++)
		printf("Candidate: %d\n\tPositive: %d\tNegative: %d\n", i, result[i * 2], result[i * 2+1]);
	delete(dict);
	for (i = 0; i < Ntargets; i++)
		delete(candidate_tag[i]);
	delete(candidate_tag);
	fclose(in);	
	printf ("\nExecution time: %.3f\n",((float)clock()/CLOCKS_PER_SEC) - timestart);
    return EXIT_SUCCESS;
}
