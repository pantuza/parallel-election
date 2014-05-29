#include <stdio.h>
#include <stdlib.h>
#include "palavras.h"

int main(int argc, char *argv[])

{
	FILE *in,*out;
	int i,j;
	palavra e;
	palavras *dicionario;
	char string[256];
	
	in=fopen(argv[1],"r");
	if(!in)return 404;
	sprintf(string,"%s_ordenado",argv[1]);
	out=fopen(string,"w");
	dicionario=carregar_palavras(in);
//insertion
	for(i=1; i< dicionario->n ; i++)
	{
		e = dicionario->p[i];
		j=i-1;
		while((j>=0) && (comparar_palavra(e.string , dicionario->p[j].string)>0))
		{
			dicionario->p[j+1] = dicionario->p[j];
			j--;
		}
		dicionario->p[j+1] = e;
	}
	for (i=0;i<dicionario->n;i++)
		fprintf(out,"%d\t%s\n",dicionario->p[i].valor,dicionario->p[i].string);
	fclose(in);
	fclose(out);
}
