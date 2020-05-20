#include <string.h>
#include <stdbool.h>

bool check(const char *str, const char *name, const char* extension)
{
	char word[1000];
    FILE *fptr;
	char filename[20];
	strcpy(filename, name);
	strcat(filename, "_");
	strcat(filename, extension);
	strcat(filename, ".txt");
	
    if ((fptr = fopen(filename, "r")) == NULL)
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

void fillWord(char *str, const char* extension){
	char word[30];
    FILE *fptr;
	char filename[20];
	strcpy(filename, "autocomplete_");
	strcat(filename, extension);
	strcat(filename, ".txt");
    if ((fptr = fopen(filename, "r")) == NULL)
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
char* autocomplete(const char *str, int COL, const char* extension)
{
	char temp[LINE_LENGTH];
	strcpy(temp,str);
	
	if(COL == 1 || temp[COL-2] == ' ')
		return "";
	int j =  COL-2;
	char separator[6] = {' ','{','}','(',')',','};
	
	while(j>=0){
		int k;
		bool flag = false;
		for(k=0;k<6;k++){
			if(temp[j] == separator[k]){
				flag = true;
				break;
			}
		}
		if(flag)
			break;
		j--;
	}
	
	int start_index = j+1;
	int end_index = COL-1;
	char *word = substr(temp,start_index,end_index);
	
	int prevlen = strlen(word);
	fillWord(word, extension);
	return substr(word, prevlen, strlen(word));
}

int returnStartIndex(const char* temp, int len, int *j){
	char separator[9] = {' ','{','}','(',')',',','"',';',':'};
	while(*j<len){
		int k;
		bool flag = true;
		for(k=0; k<9; k++){
			if(temp[*j] == separator[k]){
				flag = false;
				break;
			}
		}
		if(flag)
			break;
		*j = *j +1;
	}
	return *j;
}

int returnEndIndex(const char* temp,int len, int *j){
	char separator[9] = {' ','{','}','(',')',',','"',';',':'};
	while(*j<len){
		int k;
		bool flag = true;
		for(k=0; k<9; k++){
			if(temp[*j] == separator[k]){
				flag = false;
				break;
			}
		}
		if(!flag)
			break;
		*j = *j + 1;
	}
	return *j;
}

void checkSpelling(const char* str, const char* extension){
	char temp[LINE_LENGTH];
	strcpy(temp,str);
	int len = strlen(temp);
	
	if(len == 0)
		return;
	
	int j = 0;
	int start_index,end_index = 0;
	
	while(j<len){
		start_index = returnStartIndex(temp, len, &j);
		if(start_index == 0);
		else if(end_index == 0)
			printf("%s", substr(temp,0,start_index));
		else
			printf("%s", substr(temp,end_index,start_index));
		end_index = returnEndIndex(temp, len, &j);
		
		char *word = substr(temp, start_index, end_index);
		char wordtemp[30];
		strcpy(wordtemp,word);
		strlwr(wordtemp);
		if(check(word, "keywords", extension)){
			changeTextColor(BROWN);
			printf("%s",word);
			changeTextColor(WHITE);
		}
		else if(check(word, "headers", extension)){
			changeTextColor(LIGHTBLUE);
			printf("%s",word);
			changeTextColor(WHITE);
		}
		else if(check(word, "functions", extension)){
			changeTextColor(LIGHTGREEN);
			printf("%s",word);
			changeTextColor(WHITE);
		}
		else if(check(word, "directives", extension)){
			changeTextColor(LIGHTCYAN);
			printf("%s",word);
			changeTextColor(WHITE);
		}
		else if(!check(wordtemp, "words", "")){
			changeTextColor(RED);
			printf("%s",word);
			changeTextColor(WHITE);
		}
		else
			printf("%s",word);
	}
}

