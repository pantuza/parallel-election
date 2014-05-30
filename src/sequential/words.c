#include "palavras.h"

int contar_palavras(FILE *in)
{
	char string[1024];
	int q=0;

	while(fscanf(in,"%s %s",string,string) ==2)q++;
	rewind(in);
	return q;
}

int comparar_palavra(char *p1,char *p2)
{
	int i;
	
	for(i=0;i<PMAX;i++)
	{
		if(p1[i] == '\0' && p1[i] == '\0')return 0;
		if(p1[i] == p2[i])continue;
		if(p1[i] > p2[i])
			return -1;
		else
			return 1;
	}
	return 0;
}

palavras *carregar_palavras(FILE *in)
{
	palavras *dicionario;
	int i,j;

	dicionario = malloc(sizeof(palavras));
	dicionario->n=contar_palavras(in);
	dicionario->p=calloc(dicionario->n,sizeof(palavra));
	for(i=0;(i<(dicionario->n)) && (!feof(in));i++)
	{
		fscanf(in,"%d %s",&(dicionario->p[i].valor), dicionario->p[i].string);
		for(j=0;j<PMAX && dicionario->p[i].string[j] != '\0' ;j++)	//Transforma tudo pra minusculo 
		if(dicionario->p[i].string[j]<91 && dicionario->p[i].string[j]>64)
				dicionario->p[i].string[j]+=32;
	}
	return dicionario;

}

void imprime_palavras(palavras *dicionario)
{
	int i;
	for(i=0;i<(dicionario->n);i++)
		printf("%d %s\n",dicionario->p[i].valor, dicionario->p[i].string);
	return;
}

int valor_palavra_l(palavras *dicionario, char *palavra)
{
	int i;
	
	for(i=0;i<(dicionario->n);i++)
		if(!comparar_palavra(dicionario->p[i].string,palavra))return dicionario->p[i].valor;
	return 0;
}

int valor_palavra_bo(palavras *dicionario, char *palavra)
{
	int fator,pos,last;

	pos=(dicionario->n) - 1;
	fator=pos;
	last = 0;
	while((pos>0) && (pos<(dicionario->n)))
	{
		if(fator != 1)fator=fator/2;
		switch(comparar_palavra(dicionario->p[pos].string,palavra))
		{
			case -1:
				if(fator == 1)
				{
					if(last == 1)return 0;
					last =-1;
				}
				pos-=fator;
				break;
			case 0:
				return dicionario->p[pos].valor;
			case 1:
			if(fator == 1)
				{
					if(last == -1)return 0;
					last =1;
				}
				pos+=fator;
		}
	}
	return 0;	
}
