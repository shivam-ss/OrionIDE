#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int strpos(const char *str, const char *substr, int start_index);
char* substr(const char *str, int start_index, int end_index);
void substrInsert(char *str, const char* strInsert, int index);
void substrDelete(char *str, int start_index, int end_index);
int* strcount(const char *str, const char *substr, int start_index);

int strpos(const char *str, const char *substr, int start_index)
{
	char *ret = strstr(str + start_index,substr);
	if(ret)
		return (ret - str);
	return -1;
}


char* substr(const char *str, int start_index, int end_index)
{
	if(start_index > end_index || start_index > strlen(str) || end_index > strlen(str))
		return NULL;
	int len = end_index-start_index+1;
	char *ret = (char*)malloc(len * sizeof(char));
	int i;
	for(i = start_index; i<end_index && (*str != '\0'); i++){
		*ret = *(str+i);
		ret++;
	}
	*ret = '\0';
	return ret - len +1;
}

void substrInsert(char *str, const char* strInsert, int index){
	if(strlen(str) <= index){
		strcat(str,strInsert);
		return;
	}
	char *endstr = substr(str, index, strlen(str));
	str[index] = '\0';
	strcat(str,strInsert);
	strcat(str,endstr);
}
void substrDelete(char *str, int start_index, int end_index){
	if(strlen(str) < end_index || strlen(str) <= start_index)
		return;
	if(end_index == strlen(str)){
		str[start_index] = '\0';
		return;
	}
	char *endstr = substr(str, end_index, strlen(str));
	str[start_index] = '\0';
	strcat(str,endstr);
}

int* strcount(const char *str, const char *substr, int start_index)
{
	static int occurences[10];//1st index tells no of occurences and other tells locations
	int count=0;
	char *found = strstr(str + start_index,substr);
	while(found)
	{
		count++;
		occurences[count] = found - str;
		found = strstr(found+1,substr);
	}
	occurences[0] = count;
	return occurences;
}
