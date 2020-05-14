#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int strpos(const char *str, const char *substr, int start_index)
{
	char *ret = strstr(str + start_index,substr);
	if(ret)
		return (ret - str);
	return -1;
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
bool checkSyntax(const char **str, int l);
bool missingBraces(const char **str, int l);

int countOpenBraces = 0;
int Braces[100][2];//stores location of open braces with line no. and col no.
int main()
{
	int rows =100,cols =100;
	int i;
	
	char **str;
	str = malloc(rows * sizeof *str);
    for (i=0; i<rows; i++)
    {
    	str[i] = malloc(cols * sizeof *str[i]);
    }
    
	int nol;//no. of lines

	printf("Enter Code:\n");
	i=0;
	while(1)
	{
		printf("%d\t",i+1);
		scanf("%[^'\n']s",str[i]);
		fflush(stdin);
		if(strcmp(str[i],"/")==0)
		{
			nol = i;
			break;
		}
		i++;
		if(checkSyntax(str,i))
			exit(0);
	}
	if(countOpenBraces > 0)
		printf("Open braces '{' at line %d col %d",Braces[countOpenBraces-1][0],Braces[countOpenBraces-1][1]+1);
	else
	{
		char *dataToBeWritten;
		dataToBeWritten = malloc(10000 * sizeof dataToBeWritten);
		strcpy(dataToBeWritten,"");
		for(i=0; i < nol ;i++)
		{
			strcat(dataToBeWritten,str[i]);
			strcat(dataToBeWritten,"\n");
		}
		
		// Declare the file pointer 
    	FILE *filePointer ; 
		
		filePointer = fopen("temp.c", "w") ; 
      
	    // Check if this filePointer is null 
	    // which maybe if the file does not exist 
	    if ( filePointer == NULL ) 
	    { 
	        printf( "temp.c file failed to save." ) ; 
	    } 
	    else
	    {     
	        // Write the dataToBeWritten into the file 
	        if ( strlen (  dataToBeWritten  ) > 0 ) 
	        { 
	              
	            // writing in the file using fputs() 
	            fputs(dataToBeWritten, filePointer) ;    
	            fputs("\n", filePointer) ; 
	        } 
	          
	        // Closing the file using fclose() 
	        fclose(filePointer) ;  
	        
	        system("gcc temp.c");
	        
	        printf("\n\nOutput:\n");
			system("a.exe");
	    } 
	}
	return 0;
}
bool checkSyntax(const char **str, int l)
{
	return missingBraces(str, l);
}

bool missingBraces(const char **str, int l)
{
	int i=0;
	int *openBrace = strcount(str[l-1],"{",0);
	for(i=0 ; i<openBrace[0] ; i++)
	{
		countOpenBraces++;
		Braces[countOpenBraces-1][0] = l;
		Braces[countOpenBraces-1][1] = openBrace[i+1];
	}
	
	int *closeBrace = strcount(str[l-1],"}",0);
	for(i=0 ; i<closeBrace[0] ; i++)
	{
		countOpenBraces--;
		if(countOpenBraces<0)
		{
			printf("Error at line %d col %d",l,closeBrace[i+1]+1);
			return true;
		}
	}
	return false;
}
