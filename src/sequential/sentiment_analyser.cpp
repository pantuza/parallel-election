#include "words.h"
#include "sentiment_analyser.h"

int analyse(words *dict, char *string)
{
	int pos,sum,i;
	char word[PMAX];
	
	sum=0;
	pos=0;	//next word position in string[]
	while(1)
	{
//Insert a word from string in each iteration:
		for(i=0;i<SMAX;i++)	
		{
			word[i]=string[pos];
			if(string[pos]==' ' || string[pos]=='\0' || string[pos]=='\n')break;
			pos++;
		}
		if(word[0] == ' ')
		{
			pos++;
			continue;
		}
		if(word[0] == '\0')return sum;
		word[i]='\0';
//Analysis:
		sum+=dict->word_value_sa(word);
		if(string[pos]== '\0')return sum;
		pos++;
	}
	return sum;
}
