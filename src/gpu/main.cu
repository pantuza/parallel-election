#include <stdio.h>
#include <iostream>
#include <time.h>
#include "words.h"
#include "sentiment_analyser.h"
#include "tweet_target_analyser.h"

#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>


#define BLOCKSIZE	1048576


/* CUDA */
#define N (2048 * 2048)
#define THREADS_PER_BLOCK 1024;


using namespace std;


__global__ void kernel(words *d_dict, int Ncandidates, words **candidate_tags, int Ntweet, tweet *t, int *result)
{

	__shared__ words candidates[THREADS_PER_BLOCK];
	(words**) candidates = candidates_tags;
	 
	int j;

	__shared__ int sentiments[THREADS_PER_BLOCK];
	int index  = threadIdx.x + blockIdx.x * blockDim.x;

    sentiments[threadIdx.x] = analyse(dict, t[index].t);

	if (sentiments[threadIdx.x])
	{
	
		/* Every thread waits here to compute its sentiment by candidate */
		__syncthreads();
		

		/* Still can be parallelized */
		for (j = 0; j < Ncandidates; j++)
		{
			if (analyse(candidates[j], t[index].t))
			{
					
				/* Increment using atomic operation */
				if (sentiments[threadIdx.x] > 0)
					atomicInc(&result[j * 2], 1);
				else
					atomicInc(&result[j * 2+1], 1);
					
			}
		}
	}
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

	/* CUDA dict */ 
	words *d_dict;
	cudaMalloc(&d_dict, sizeof(dict));

	

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


/* CUDA result */
int *d_result;
int size_result = (NTargets * 2) * sizeof(int);
cudaMalloc((void**) &d_result, size_result);
cudaMemcpy(d_result, result, size_result, cudaMemcpyHostToDevice);


t= new tweet[Ntweets];

/* CUDA tweets */
tweet *d_tweets;
int size_tweets = Ntweets * sizeof(tweet);
cudaMalloc((void**) &d_tweets, size_tweets);
cudaMemcpy( d_tweets, t, size_tweets, cudaMemcpyHostToDevice);


	//Execute
	while(!feof(in))
	{
		for(j=0;j< Ntweets; j++)
			t[j].load_from_file(in);

		/* CALL CUDA KERNEL */
		kernel<<<N/THREADS_PER_BLOCK, THREADS_PER_BLOCK>>> (d_dict, Ntargets, candidate_tag, Ntweets, d_tweets, d_result);
	}

	/* CUDA copy result */
	cudaMemcpy(result, d_result, sizeof( int ) , cudaMemcpyDeviceToHost );


	/* CUDA free */
	cudaFree(d_dict);
	cudaFree(d_result);
	cudaFree(d_tweets);


//print results:
	for (i = 0; i < Ntargets; i++)
		printf("Candidate: %d\n\tPositive: %d\tNegative: %d\n", i, result[i * 2], result[i * 2+1]);
	delete(dict);
	for (i = 0; i < Ntargets; i++)
		delete(candidate_tag[i]);
	delete(candidate_tag);
	fclose(in);	
	printf ("\nExecution time: %.3f\n",((float)clock()/CLOCKS_PER_SEC) - timestart);
    
	puts("Press any key to exit...");
	char c[1];
	gets(c);
	return EXIT_SUCCESS;
}
