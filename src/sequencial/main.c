#include "palavras.h"
#include "analisador_sentimentos.h"

int main(int argc,char *argv[])
{
	FILE *in,*out;
	palavras *dicionario;
	char string[256];
	
	in=fopen(argv[1],"r");
	out=fopen(argv[2],"w");
	if(!in)
	{
		puts("Um arquivo de dicionario deve ser passado como parametro");
		return 0;
	}
	dicionario=carregar_palavras(in);
	while(1)
	{
		printf("\nEntre com a frase ou 0 para sair\nFrase: ");
		fgets(string,256,stdin);
		if(string[0]=='0')return 0;
		printf("Polaridade da frase: %d\n",analisar(dicionario, string, out));
	}
}
