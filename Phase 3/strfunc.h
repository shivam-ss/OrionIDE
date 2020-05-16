#include <string.h>
#include <stdlib.h>

int strpos(const char *str, const char *substr, int start_index)
{
	char *ret = strstr(str + start_index,substr);
	if(ret)
		return (ret - str);
	return -1;
}

char* substr(const char *str, int start_index, int end_index)
{
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
