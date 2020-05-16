#include "textcolor.h"
#include "strfunc.h"
#include "filefunc.h"
#include "syntaxerrors.h"
#include "spellcheck.h"

#define COL 100
#define LINE 1000
#define NO_OF_BRACES 100
#define NO_OF_PARANTHESIS 100

int countOpenBraces = 0;
int Braces[NO_OF_BRACES][2];//stores location of open braces with line no. and col no.
int countOpenParens = 0;
int Parens[NO_OF_PARANTHESIS][2];//stores location of open paranthesis with line no. and col no.
WORD DEFAULT_COLOR;//Default color of text in console
HANDLE hConsole;//Default Console Details

void printName(){
	changeTextColor(FOREGROUND_BLUE);
	printf("\t     ****     *****      *******     ****     **     *   \n");
	printf("\t    *    *    *    *        *       *    *    * *    *   \n");
	printf("\t   *      *   *****         *      *      *   *  *   *   \n");
	printf("\t   *      *   *    *        *      *      *   *   *  *   \n");
	printf("\t    *    *    *     *       *       *    *    *    * *   \n");
	printf("\t     ****     *      *   *******     ****     *     **   \n\n\n");
	changeTextColor(FOREGROUND_INTENSITY);
	printf("SHORTCUTS:\n");
	printf("Ctrl + s -> Save\n");
	printf("Ctrl + c -> Compile and Run\n");
	printf("Ctrl + o -> Autofill\n");
	printf("Ctrl + e -> Edit\n\n\n");
	changeTextColor(DEFAULT_COLOR);
}

void compileAndExecute(const char* filename){
	printf("\n\nOutput:\n");
	char command[50] = "gcc ";
	strcat(command,filename);
    system(command);
    
	system("a.exe");
}

void oldFileEdit(const char* filename){
	countOpenBraces = 0;
	countOpenParens = 0;
	int i;
	char **str;
	str = malloc(LINE * sizeof *str);
    for (i=0; i<LINE; i++)
    	str[i] = malloc(COL * sizeof *str[i]);
    
	int nol;//no. of lines

	printf("\nEnter Code:\n");
	i=0;
	FILE *fp = fopen(filename,"r");
	if(fp == NULL){
		printf("Cannot open file");
		return;
	}
	bool first=true;
	while(1){//File is printed here till second last line
		if(first){
			first = false;
			if(fgets(str[i],79,fp) == NULL)
				break;
		}
		str[i][strlen(str[i])-1] = '\0';
		if(fgets(str[i+1],79,fp) == NULL)
			break;	
		printf("%d\t",i+1);
		printf("%s",str[i]);
		i++;
		if(checkSyntax(str,i)){
			printf("\n");
			continue;
		}
		printf("\n");
	}
	fclose(fp);
	bool flag = true;
	while(1)
	{
		if(flag){
			printf("%d\t%s",i+1,str[i]);
			flag = false;
		}
		else{
			printf("%d\t",i+1);
			strcpy(str[i],"");
		}
		char c[2];
		c[1] = '\0';
		
		bool compile = false;
		while((c[0] = getch())!= 13){ //13 ASCII value of ENTER
			
			
			if(c[0] == 8){	//8 ASCII value of BACKSPACE
				char *space = malloc(strlen(str[i])+1 * sizeof(char));
				strcpy(space,""); 
				int j;
				for(j=0; j<strlen(str[i]); j++)
					strcat(space," ");
				str[i] = substr(str[i],0,strlen(str[i])-1);
				printf("\r%d\t%s",i+1,space);
				printf("\r%d\t%s",i+1,str[i]);
			}
			else if(c[0] == 15){	//15 ASCII value of CTRL + o
				char word[30];
				// Returns first token 
				char* temp;
				strcpy(temp,str[i]);
				char* token = strtok(temp, " "); 
				strcpy(word, token);
				
				// Keep printing tokens while one of the 
				// delimiters present in str[]. 
				while (token != NULL) {
					strcpy(word, token);
				    token = strtok(NULL, " "); 
				}
		
				char *space = malloc(strlen(str[i])+1 * sizeof(char));
				strcpy(space,""); 
				int j;
				for(j=0; j<strlen(str[i]); j++)
					strcat(space," ");
				printf("\r%d\t%s",i+1,space);
				str[i] = substr(str[i],0,strlen(str[i])-strlen(word));
				
				char *newword = word; //returnHint(word); 
				strcat(str[i],newword);
				printf("\r%d\t%s",i+1,str[i]);
				           
			}
			else if(c[0] == 32){	//13 ASCII value of SPACE
				char word[30];
				// Returns first token 
				char temp[80];
				strcpy(temp,str[i]);
				char* token = strtok(temp, " "); 
				printf("\r%d\t",i+1);
				strcpy(str[i], "");
				
				// Keep printing tokens while one of the 
				// delimiters present in str[]. 
				while (token != NULL) {
					strcpy(word, token);
					if(!checkword(word)){
						strcat(str[i],word);
						strcat(str[i]," ");
						changeTextColor(FOREGROUND_GREEN);
						printf("%s ",word);
						changeTextColor(DEFAULT_COLOR);
					}
					else{
						strcat(str[i],word);
						strcat(str[i]," ");
						printf("%s ",word);
					}
				    token = strtok(NULL, " "); 
				}
				         
			}
			else if(c[0] == 19){	//15 ASCII value of CTRL + s
				writeArrayOfStrings(str, i+1, filename);
			}
			else if(c[0] == 5){	//5 ASCII value of CTRL + e
				writeArrayOfStrings(str, i+1, filename);
				float l2;
				printf("\nEnter Line number to Edit:");
				scanf("%f",&l2);
				if(l2>i+1 || l2<0 || (int)l2 != l2){
					printf("Invalid Input");
					oldFileEdit(filename);
					return;
				}
				int l = l2;
				printf("Previous Code:\n%d\t%s", l, str[l-1]);
				printf("\nNew Code:\n");
				char *newline = malloc(COL * sizeof(char));
				strcpy(newline, str[l-1]);
				printf("%d\t%s", l, newline);
				while((c[0] = getch())!= 13){ //13 ASCII value of ENTER
				
					if(c[0] == 8){	//8 ASCII value of BACKSPACE
						char *space = malloc(strlen(newline)+1 * sizeof(char));
						strcpy(space,""); 
						int j;
						for(j=0; j<strlen(newline); j++)
							strcat(space," ");
						newline = substr(newline,0,strlen(newline)-1);
						printf("\r%d\t%s",l,space);
						printf("\r%d\t%s",l,newline);
					}
					else if(c[0] == 15){	//15 ASCII value of CTRL + o
						char word[30];
						// Returns first token 
						char* temp;
						strcpy(temp,newline);
						char* token = strtok(temp, " "); 
						strcpy(word, token);
						
						// Keep printing tokens while one of the 
						// delimiters present in str[]. 
						while (token != NULL) {
							strcpy(word, token);
						    token = strtok(NULL, " "); 
						}
				
						char *space = malloc(strlen(newline)+1 * sizeof(char));
						strcpy(space,""); 
						int j;
						for(j=0; j<strlen(newline); j++)
							strcat(space," ");
						printf("\r%d\t%s",l,space);
						newline = substr(newline,0,strlen(newline)-strlen(word));
						
						char *newword = word; //returnHint(word); 
						strcat(newline,newword);
						printf("\r%d\t%s",l,newline);	           
					}
					else if(32<=c[0] && c[0]<=126){
						strcat(newline,c);
						printf("%c",c[0]);
					}
				}
				strcpy(str[l-1],newline);
				writeArrayOfStrings(str, i+1, filename);
				system("cls");
				printName();
				oldFileEdit(filename);
				return;
			}
			else if(32<=c[0] && c[0]<=126){
				strcat(str[i],c);
				printf("%c",c[0]);
			}
			else if(c[0] == 3){	//3 ASCII value of CTRL + c
				writeArrayOfStrings(str, i+1, filename);
				compile = true;
				break;
			}
		}
		i++;
		if(compile){
			checkSyntax(str,i);
			break;
		}
		if(checkSyntax(str,i)){
			printf("\n");
			continue;
		}
		printf("\n");
	}
	if(countOpenBraces > 0 ){
		changeTextColor(FOREGROUND_RED);
		printf("\nOpen braces '{' at line %d col %d",Braces[countOpenBraces-1][0],Braces[countOpenBraces-1][1]+1);
	}
	else if(countOpenParens > 0 ){
		changeTextColor(FOREGROUND_RED);
		printf("\nOpen braces '(' at line %d col %d",Parens[countOpenParens-1][0],Parens[countOpenParens-1][1]+1);
	}	
	else
		compileAndExecute(filename);
	changeTextColor(DEFAULT_COLOR);
}

void newFileEdit(const char *filename){
	int i;
	char **str;
	str = malloc(LINE * sizeof *str);
    for (i=0; i<LINE; i++)
    	str[i] = malloc(COL * sizeof *str[i]);
    
	int nol;//no. of lines

	printf("Enter Code:\n");
	i=0;
	while(1)
	{
		printf("%d\t",i+1);
		strcpy(str[i],"");
		char c[2];
		c[1] = '\0';
		bool compile = false;
	
		while((c[0] = getch())!= 13){ //13 ASCII value of ENTER
			
			
			if(c[0] == 8){	//8 ASCII value of BACKSPACE
				char *space = malloc(strlen(str[i])+1 * sizeof(char));
				strcpy(space,""); 
				int j;
				for(j=0; j<strlen(str[i]); j++)
					strcat(space," ");
				str[i] = substr(str[i],0,strlen(str[i])-1);
				printf("\r%d\t%s",i+1,space);
				printf("\r%d\t%s",i+1,str[i]);
			}
			else if(c[0] == 15){	//15 ASCII value of CTRL + o
				char word[30];
				// Returns first token 
				char* temp;
				strcpy(temp,str[i]);
				char* token = strtok(temp, " "); 
				strcpy(word, token);
				
				// Keep printing tokens while one of the 
				// delimiters present in str[]. 
				while (token != NULL) {
					strcpy(word, token);
				    token = strtok(NULL, " "); 
				}
		
				char *space = malloc(strlen(str[i])+1 * sizeof(char));
				strcpy(space,""); 
				int j;
				for(j=0; j<strlen(str[i]); j++)
					strcat(space," ");
				printf("\r%d\t%s",i+1,space);
				str[i] = substr(str[i],0,strlen(str[i])-strlen(word));
				
				char *newword = word; //returnHint(word); 
				strcat(str[i],newword);
				printf("\r%d\t%s",i+1,str[i]);
				           
			}
			else if(c[0] == 19){	//15 ASCII value of CTRL + s
				writeArrayOfStrings(str, i+1, filename);
			}
			else if(c[0] == 5){	//5 ASCII value of CTRL + e
				writeArrayOfStrings(str, i+1, filename);
				float l2;
				printf("\nEnter Line number to Edit:");
				scanf("%f",&l2);
				if(l2>i+1 || l2<0 || (int)l2 != l2){
					printf("Invalid Input");
					oldFileEdit(filename);
					return;
				}
				int l = l2;
				
				printf("Previous Code:\n%d\t%s", l, str[l-1]);
				printf("\nNew Code:\n");
				char *newline = malloc(COL * sizeof(char));
				strcpy(newline, str[l-1]);
				printf("%d\t%s", l, newline);
				while((c[0] = getch())!= 13){ //13 ASCII value of ENTER
				
					if(c[0] == 8){	//8 ASCII value of BACKSPACE
						char *space = malloc(strlen(newline)+1 * sizeof(char));
						strcpy(space,""); 
						int j;
						for(j=0; j<strlen(newline); j++)
							strcat(space," ");
						newline = substr(newline,0,strlen(newline)-1);
						printf("\r%d\t%s",l,space);
						printf("\r%d\t%s",l,newline);
					}
					else if(c[0] == 15){	//15 ASCII value of CTRL + o
						char word[30];
						// Returns first token 
						char* temp;
						strcpy(temp,newline);
						char* token = strtok(temp, " "); 
						strcpy(word, token);
						
						// Keep printing tokens while one of the 
						// delimiters present in str[]. 
						while (token != NULL) {
							strcpy(word, token);
						    token = strtok(NULL, " "); 
						}
				
						char *space = malloc(strlen(newline)+1 * sizeof(char));
						strcpy(space,""); 
						int j;
						for(j=0; j<strlen(newline); j++)
							strcat(space," ");
						printf("\r%d\t%s",l,space);
						newline = substr(newline,0,strlen(newline)-strlen(word));
						
						char *newword = word; //returnHint(word); 
						strcat(newline,newword);
						printf("\r%d\t%s",l,newline);
						           
					}
					else if(32<=c[0] && c[0]<=126){
						strcat(newline,c);
						printf("%c",c[0]);
					}
				}
				strcpy(str[l-1],newline);
				writeArrayOfStrings(str, i+1, filename);
				system("cls");
				printName();
				oldFileEdit(filename);
				return;
			}
			else if(32<=c[0] && c[0]<=126){
				strcat(str[i],c);
				printf("%c",c[0]);
			}
			else if(c[0] == 3){	//3 ASCII value of CTRL + c
				writeArrayOfStrings(str, i+1, filename);
				compile = true;
				break;
			}
		}
		if(compile){
			checkSyntax(str,i);
			break;
		}
		i++;
		if(checkSyntax(str,i)){
			printf("\n");
			continue;
		}
		printf("\n");
	}
	if(countOpenBraces > 0 ){
		changeTextColor(FOREGROUND_RED);
		printf("\nOpen braces '{' at line %d col %d",Braces[countOpenBraces-1][0],Braces[countOpenBraces-1][1]+1);
	}
	else if(countOpenParens > 0 ){
		changeTextColor(FOREGROUND_RED);
		printf("\nOpen braces '(' at line %d col %d",Parens[countOpenParens-1][0],Parens[countOpenParens-1][1]+1);
	}	
	else
		compileAndExecute(filename);
	changeTextColor(DEFAULT_COLOR);
}

int main(int argc,char* argv[])
{
	char filename[50];
	saveDefaultAttributes();
	if(argc==1){
		printName();
	    printf("Enter filename(with extension):\t");
	    scanf("%s",filename);
	    system("cls");
	}
	else{
		strcpy(filename, argv[1]);
	}
	printName();
    
    if(isFileExists(filename))
    	oldFileEdit(filename);
    else
    	newFileEdit(filename);
	   
	return 0;
}
