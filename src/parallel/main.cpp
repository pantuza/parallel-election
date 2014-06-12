#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include "words.h"
#include "sentiment_analyser.h"
#include "tweet_target_analyser.h"
#define	NTHREADS	4
#define BLOCKSIZE	1024

using namespace std;

//Mutex global para controlar a leitura do arquivo de entrada:
pthread_mutex_t mutexreading =  PTHREAD_MUTEX_INITIALIZER;

class threadset
{
//Classe responsável por todos os parâmetros necessários para a thread
public:
	int Ntargets;				//Número de alvos (politicos) a procurar nos tweets
	int Ntweets;				//Número de tweets que cada thread computa entra as leituras
	int *result;				//Vetor de resposta da thread, com (Ntargets X 2) elementos, cada par tento o número tweets negativos e positivos de cada target
	int end;				//Flag que indica que a entrada acabaou
	pthread_mutex_t 			//Bloqueios para cada thread para gerenciar o consumidor/produtor
		mprod,					//--Esse bloqueio indica que uma entrada está sendo lida
		mcons;					//--Esse bloqueio indica que um bloco está sendo processado
	FILE 	*in;				//Arquivo de entrada dos tweets
	words 	*dict;				//Ponteiro para o dicionário de sentimentos
	words 	**candidate_tag;		//Ponteiro para um vetor de dicionários de termos que identificam os alvos
	tweet 	*prod,				//Bloco de tweets para leitura de entrada
		*cons;				//Bloco de tweets para processamento

	void set(int N_candidates,int N_tweets, words *dictionary, words **candidate_tags,FILE *input)
	{
	//Função que inicializa a classe
		int i;	
		Ntargets = N_candidates;
		mcons = PTHREAD_MUTEX_INITIALIZER;		
		mprod = PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_lock(&mcons);	//Bloqueia o consumo para ler a entrada
		Ntweets = N_tweets;
		dict = dictionary;
		end = 0;
		candidate_tag = candidate_tags;
		prod = new tweet[N_tweets];
		cons = new tweet[N_tweets];
		result = new int[Ntargets*2];
		for(i=0;i<Ntargets*2;i++)
			result[i]=0;
		in=input;
	}

	void read_in()
	{
	//Carrega Ntweets (Tamanho do bloco) do arquivo de entrada em "prod"
		int i;
		pthread_mutex_lock(&mprod);		//Bloqueia produtores concorrentes. O produtor que desbloqueia "mprod"
		pthread_mutex_lock(&mutexreading);	//Bloqueia todas as threads de acessar o arquivo de entrada
		for(i=0;i< Ntweets; i++)
			prod[i].load_from_file(in);
		pthread_mutex_unlock(&mutexreading);
		if(feof(in))end = 1;
		pthread_mutex_unlock(&mcons);		//Libera a trava do consumidor para iniciar o processamento.
	}
};

void *iniciar_produtor(void *p)
{
	threadset *threadp;

	threadp = (threadset*) p;
	while(!feof(threadp->in))	//Enquanto a entrada não acabar, leia.
		threadp->read_in();
	pthread_exit(NULL);
}


void *iniciar_consumidor(void *p)
{
	threadset *threadp;
	tweet  *ttmp;
	threadp = (threadset*) p;
	while(1)
	{
		pthread_mutex_lock(&(threadp->mcons));
	//Troca o bloco do produtor com o do consumidor. O produtor recebe o bloco já processado para uma nova leitura:
		ttmp=threadp->cons;
		threadp->cons = threadp->prod;
		threadp->prod = ttmp;
	//Feito a troca, desbloqueia o produtor para a nova leitura:
		pthread_mutex_unlock(&(threadp->mprod));
	//processa:
		analyse_target(threadp->dict, threadp->Ntargets, threadp->candidate_tag, threadp->Ntweets, threadp->cons, threadp->result);
		if(threadp->end)break;
	}
	pthread_exit(NULL);
}

FILE *openfile(const char * filename, const char * mode)
{
//Criado apenas para deixar o código mais limpo
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
	pthread_t 	*thread_consumidor,
			*thread_produtor;
	threadset 	*threadp;
	float timestart;
	
	timestart=(float)clock()/CLOCKS_PER_SEC;	//Inicia a medição de tempo
	if (argc < 3)
	{
		cout <<endl<< "expected input files:"<<endl<<"./exec <tweets> <dictionary> <target_dic1> <target_dic2> ..." << endl;
		return-1;
	}
//Carrega dicionário de sentimentos:
	in = openfile(argv[2], "r");
	if (!in)return EXIT_FAILURE;
	dict = new words(count_words(in));
	dict->fload_words(in);
	fclose(in);
//Carrega dicionpario
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
	thread_consumidor = (pthread_t*) calloc(NTHREADS,sizeof(pthread_t));
	thread_produtor = (pthread_t*) calloc(NTHREADS,sizeof(pthread_t));
	threadp = new threadset[NTHREADS];
	Ntweets = BLOCKSIZE;
//Executa:
	for(i=0;i<NTHREADS;i++)	//MAP
	{
		threadp[i].set(Ntargets,Ntweets,dict,candidate_tag,in);	//Inicia a classe de argumentos para a thread i
		pthread_create(&(thread_produtor[i]), NULL,iniciar_produtor, &(threadp[i]));
		pthread_create(&(thread_consumidor[i]), NULL,iniciar_consumidor, &(threadp[i]));
	}
//Enquanto as threads computam, inicializa o vetor de resposta:
	result = new int[Ntargets*2];
	for(i=0;i<Ntargets*2;i++)
		result[i]=0;
//Barreira de resultados:
	for(i=0;i<NTHREADS;i++)	//Reduce
	{
		pthread_join(thread_consumidor[i],NULL);	//Espera a thread i terminar
		for(j=0;j<Ntargets*2;j++)					
			result[j]+=threadp[i].result[j];		//Soma seus resultados com os resultados gerais
	}
//Imprime resultados:
	for (i = 0; i < Ntargets; i++)
		printf("Candidate: %d\n\tPositive: %d\tNegative: %d\n", i, result[i * 2], result[i * 2+1]);
//Limpa a memória:
	delete(dict);
	for (i = 0; i < Ntargets; i++)
		delete(candidate_tag[i]);
	delete(candidate_tag);
	free(thread_consumidor);
	free(thread_produtor);
	fclose(in);
//Imprime tempo de execução:
	printf ("\nExecution time: %.3f\n",((float)clock()/CLOCKS_PER_SEC) - timestart);
	return EXIT_SUCCESS;
}
