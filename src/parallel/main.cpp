#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include "words.h"
#include "sentiment_analyser.h"
#include "tweet_target_analyser.h"
#define	NTHREADS	8
#define BLOCKSIZE	4096000

using namespace std;

pthread_mutex_t mutexfile_in = PTHREAD_MUTEX_INITIALIZER;

class threadset
{
public:
	int Ntargets;
	int Ntweets;
	int *result;
	FILE *in;
	words *dict;
	words **candidate_tag;
	tweet *t;
	threadset()
	{
		t=0;
		result=0;
	}
	void set(int N_candidates,int N_tweets, words *dictionary, words **candidate_tags,FILE *input)
	{
		int i;	
		Ntargets = N_candidates;
		Ntweets = N_tweets;
		dict = dictionary;
		candidate_tag = candidate_tags;
		t = new tweet[N_tweets];
		result = new int[Ntargets*2];
		for(i=0;i<Ntargets*2;i++)
			result[i]=0;
		in=input;
	}

	void read_in()
	{
		int i;
		pthread_mutex_lock(&mutexfile_in);
		for(i=0;i< Ntweets; i++)
			t[i].load_from_file(in);
		pthread_mutex_unlock(&mutexfile_in);
	}
};

void *iniciar_thread(void *p)
{
	threadset *threadp;

	threadp = (threadset*) p;
	while(!feof(threadp->in))
	{
		threadp->read_in();
		analyse_target(threadp->dict, threadp->Ntargets, threadp->candidate_tag, threadp->Ntweets, threadp->t, threadp->result);
	}
	pthread_exit(NULL);
return 0;
}

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
	pthread_t *thread;
	threadset threadp[NTHREADS];
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
	thread=(pthread_t*) calloc(NTHREADS,sizeof(pthread_t));
	Ntweets = BLOCKSIZE;
//Run
	for(i=0;i<NTHREADS;i++)	//MAP
	{
		threadp[i].set(Ntargets,Ntweets,dict,candidate_tag,in);
		pthread_create(&(thread[i]), NULL,iniciar_thread, &(threadp[i]));
	}
	result = new int[Ntargets*2];
	for(i=0;i<Ntargets*2;i++)
		result[i]=0;
	for(i=0;i<NTHREADS;i++)	//Reduce
	{
		pthread_join(thread[i],NULL);
		for(j=0;j<Ntargets*2;j++)
			result[j]+=threadp[i].result[j];
	}
//print results:
	for (i = 0; i < Ntargets; i++)
		printf("Candidate: %d\n\tPositive: %d\tNegative: %d\n", i, result[i * 2], result[i * 2+1]);
	delete(dict);
	for (i = 0; i < Ntargets; i++)
		delete(candidate_tag[i]);
	delete(candidate_tag);
	free(thread);
	fclose(in);
	printf ("\nExecution time: %.3f\n",((float)clock()/CLOCKS_PER_SEC) - timestart);
    return EXIT_SUCCESS;
}
