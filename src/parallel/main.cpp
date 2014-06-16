#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include "words.h"
#include "sentiment_analyser.h"
#include "tweet_target_analyser.h"
#define	NTHREADS	4
#define BLOCKSIZE	1048576

using namespace std;

pthread_mutex_t mutexreading =  PTHREAD_MUTEX_INITIALIZER;

class threadset
{
public:
	int Ntargets;	//Número de alvos (politicos) para buscar nos tweets
	int Ntweets;	//Número de tweets que cada thread trabalha em cada intervalo de leitura da entrada
	int *result;	//Vetor de respostas, 2*Ntargets, cada par contendo a quantidade de tweets positivos e negativos, respectivamente.
	int end;		//Flag que indica quando a entrada acabou
	pthread_mutex_t 
		mprod,		//Bloqueio que indica que o produtor está lendo a entrada
		mcons;		//Bloqueio que indica que o consumidor está processando a entrada
	FILE 	*in;	
	words 	*dict;	//Dicionário de sentimentos
	words 	**target_tag;	//Dicionário de tags que identificam os alvos nos tweets
	tweet 	*prod,	//Conjunto de tweets para ser lidos
			*cons;	//Consjunto de tweets para ser processados

	threadset()
	{
		end = 0;
		result=0;
		prod=0;
		cons=0;
		mcons = PTHREAD_MUTEX_INITIALIZER;
		mprod = PTHREAD_MUTEX_INITIALIZER;
		pthread_mutex_lock(&mcons);		//Bloqueia o consumo para ler a entrada
	}

	~threadset()
	{
		if(cons)	delete(cons);
		if(result)	delete(result);
		if(prod)	delete(prod);
	}
	
	void set(int N_candidates,int N_tweets, words *dictionary, words **candidate_tags,FILE *input)
	{
	//Define os argumentos de trabalho para a thread
		int i;	
		Ntargets = N_candidates;
		Ntweets = N_tweets;
		dict = dictionary;
		target_tag = candidate_tags;
		prod = new tweet[N_tweets];
		cons = new tweet[N_tweets];
		if(!prod || !cons)
			cout<< endl<<"Falha ao alocar memoria"<<endl;
		result = new int[Ntargets*2];
		for(i=0;i<Ntargets*2;i++)
			result[i]=0;
		in=input;
	}

	void read_in()
	{
		int i;
		pthread_mutex_lock(&mprod);			//Bloqueia o consumo da thread
		pthread_mutex_lock(&mutexreading);	//Bloqueia qualquer thread de ler a entrada
		for(i=0;i< Ntweets; i++)	//Carrega a entrada
			prod[i].load_from_file(in);		
		pthread_mutex_unlock(&mutexreading);
		if(feof(in))end = 1;
		pthread_mutex_unlock(&mcons);		//Desbloqueia o consumidor para processamento
	}
};

void *iniciar_produtor(void *p)
{
	threadset *threadp;

	threadp = (threadset*) p;
	while(!threadp->end)
		threadp->read_in();
	pthread_exit(NULL);
}


void *iniciar_consumidor(void *p)
{
	threadset *threadp;
	tweet  *ttmp;
	
	threadp = (threadset*) p;
	while(!threadp->end)
	{
		pthread_mutex_lock(&(threadp->mcons));	//Bloqueia o consumo
	//Troca os tweets do consumidor e produtor, para que o produtor aproveite a alocação de memória que já foi processada.
		ttmp=threadp->cons;
		threadp->cons = threadp->prod;
		threadp->prod = ttmp;
	//Desbloqueia o produtor e processa
		pthread_mutex_unlock(&(threadp->mprod));
		analyse_target(threadp->dict, threadp->Ntargets, threadp->target_tag, threadp->Ntweets, threadp->cons, threadp->result);
	}
	pthread_exit(NULL);
}

FILE *openfile(const char * filename, const char * mode)
{
// Abre o arquivo, criado apenas para deixar o código mais limpo
	FILE *f;
	f = fopen( filename,  mode);
	if (!f)cout<<endl << "Failed to load file" << filename << endl;
	return f;
}

int main(int argc, char *argv[])
{
	FILE *in;
	int i,j,Nthreads,Ntweets, Ntargets,*result;
	words *dict,**candidate_tag;
	pthread_t 	*thread_consumidor,
				*thread_produtor;
	threadset *threadp;
	float time;
	if (argc < 3)
	{
		cout <<endl<< "expected input files:"<<endl<<"./analyser <tweets> <dictionary> <target_dic1> <target_dic2> ..." << endl;
		return-1;
	}
//Carrega dicionário de sentimentos
	in = openfile(argv[2], "r");
	if (!in)return EXIT_FAILURE;
	dict = new words(count_words(in));
	dict->fload_words(in);
	fclose(in);
//Carrega dicionário de tagas para identificação dos alvos
	Ntargets = argc - 3;
	candidate_tag = new words*[Ntargets];
	if(!candidate_tag)return EXIT_FAILURE;
	for (i = 0; i < Ntargets; i++)
	{
		in = openfile(argv[i+3], "r");
		if (!in)return EXIT_FAILURE;
		candidate_tag[i] = new words(count_words(in));
		candidate_tag[i] -> fload_words(in);
		fclose(in);
	}
	cout<<"Entre com os valores desejados. 0 para os padrões"<<endl<<"\t-Threads"<<"\t"<<NTHREADS<<endl<<"\t-Tweets"<<"\t"<<BLOCKSIZE;
	cout<<endl<<"Número de threads: ";
	scanf("%d",&Nthreads);
	if(Nthreads<1)Nthreads = NTHREADS;
	cout<<endl<<"Tamanho do bloco de processamento: ";
	scanf("%d",&Ntweets);
	if(Ntweets<1)Ntweets= BLOCKSIZE;
	in = openfile(argv[1], "r");
	if (!in)return EXIT_FAILURE;
	thread_consumidor = (pthread_t*) calloc(Nthreads,sizeof(pthread_t));
	thread_produtor = (pthread_t*) calloc(Nthreads,sizeof(pthread_t));
	threadp = new threadset[Nthreads];
//Executa
	for(i=0;i<Nthreads;i++)	//MAP
	{
		threadp[i].set(Ntargets,Ntweets,dict,candidate_tag,in);
		pthread_create(&(thread_produtor[i]), NULL,iniciar_produtor, &(threadp[i]));
		pthread_create(&(thread_consumidor[i]), NULL,iniciar_consumidor, &(threadp[i]));
	}
	time=(float)clock()/CLOCKS_PER_SEC;
	result = new int[Ntargets*2];
	for(i=0;i<Ntargets*2;i++)
		result[i]=0;
	for(i=0;i<Nthreads;i++)	//Reduce
	{
		pthread_join(thread_consumidor[i],NULL);
		for(j=0;j<Ntargets*2;j++)
			result[j]+=threadp[i].result[j];
	}
	cout<<endl<<"Tempo de execução: "<<(float)clock()/CLOCKS_PER_SEC-time<<" s";
	for (i = 0; i < Ntargets; i++)
		cout<<endl<<"Candidato: "<<i<<endl<<"\tPositivos: "<< result[i*2]<<"\tNegativos: "<< result[(i*2)+1]<<endl;
	delete(dict);
	for (i = 0; i < Ntargets; i++)
		delete(candidate_tag[i]);
	delete(candidate_tag);
	free(thread_consumidor);
	free(thread_produtor);
	fclose(in);
    return EXIT_SUCCESS;
}
