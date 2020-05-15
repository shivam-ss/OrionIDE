#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <conio.h>

#include "strfunc.h"
#include "syntaxerrors.h"
#include "spellcheck.h"
#define COL 100
#define LINE 100
#define NO_OF_BRACES 100
#define NO_OF_PARANTHESIS 100

int countOpenBraces = 0;
int Braces[NO_OF_BRACES][2];//stores location of open braces with line no. and col no.
int countOpenParens = 0;
int Parens[NO_OF_PARANTHESIS][2];//stores location of open paranthesis with line no. and col no.

int main()
{
	int i;
//	printf("%d",checkword("a"));
	char **str;
	str = malloc(LINE * sizeof *str);
    for (i=0; i<LINE; i++)
    {
    	str[i] = malloc(COL * sizeof *str[i]);
    }
    
	int nol;//no. of lines

	printf("Enter Code:\n");
	i=0;
	while(1)
	{
		printf("%d\t",i+1);
//		strcpy(str[i],"");
//		char c[2];
//		while((c[0] = getch()) != 13) //13 ASCII value of ENTER
//			printf("%c",c[0]);
//			strcat(str[i],c);
//		printf("\n");
		scanf("%[^'\n']s",str[i]);
		fflush(stdin);
		
		if(strcmp(str[i],"/") == 0)
		{
			nol = i;
			break;
		}
		i++;
		if(checkSyntax(str,i))
			exit(0);
	}
	if(countOpenBraces > 0 )
		printf("Open braces '{' at line %d col %d",Braces[countOpenBraces-1][0],Braces[countOpenBraces-1][1]+1);
	else if(countOpenParens > 0 )
		printf("Open braces '(' at line %d col %d",Parens[countOpenParens-1][0],Parens[countOpenParens-1][1]+1);
	else
		compileAndExecute(str, nol);
	return 0;
}

void compileAndExecute(const char **str, int nol)
{
	int i;
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
