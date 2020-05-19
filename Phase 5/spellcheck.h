#include <string.h>
#include <stdbool.h>

bool checkword(const char *str);
bool checkkeyword(const char *str);

bool checkword(const char *str)
{
	char word[1000];
    FILE *fptr;

    if ((fptr = fopen("words.txt", "r")) == NULL)
        // Program exits if file pointer returns NULL.
        return true;        

    // reads text until newline
    while(fscanf(fptr,"%s", word) == 1){
	    if(strcmp(word,str)==0)
	    	return true;
	}
    fclose(fptr);
	return false;
}

bool checkkeyword(const char *str)
{
	char word[1000];
    FILE *fptr;

    if ((fptr = fopen("keywords.txt", "r")) == NULL)
        // Program exits if file pointer returns NULL.
        return true;        

    // reads text until newline
    while(fscanf(fptr,"%s", word) == 1){
	    if(strcmp(word,str)==0)
	    	return true;
	}
    fclose(fptr);
	return false;
}

void autocomplete(char *str)
{
	char word[30];
    FILE *fptr;

    if ((fptr = fopen("autocomplete.txt", "r")) == NULL)
        // Program exits if file pointer returns NULL.
        return;        

    // reads text until newline
    while(fscanf(fptr,"%s", word) == 1){
    	if(strlen(str) > strlen(word));
	    else if(strcmp(substr(word,0,strlen(str)),str) == 0)
	    	break;
	    strcpy(word,str);
	}
    fclose(fptr);
	strcpy(str,word);
}

