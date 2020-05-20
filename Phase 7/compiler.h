#include "syntaxerrors.h"
#define CODE_X 7
#define CODE_Y 12

extern int CURSOR_X;	//Contains the Current X location of cursor
extern int CURSOR_Y;	//Contains the Current X location of cursor
extern bool CURSOR;
extern bool Error;
extern char filename[50];
extern int countOpenBraces;
extern int countOpenParens;
char compiler[7];

int Braces[NO_OF_BRACES][2];	//stores location of open braces with line no. and col no.
int Parens[NO_OF_PARANTHESIS][2];	//stores location of open paranthesis with line no. and col no.

void selectCompiler();
bool compile();
void execute();
void error();

void selectCompiler(){
	char* ext = returnFileExtension(filename);
	
	if(strcmp(ext,"c") == 0)
		strcpy(compiler,"gcc");
	else if(strcmp(ext,"cpp") == 0)
		strcpy(compiler,"g++");
	else if(strcmp(ext,"py" )== 0)
		strcpy(compiler,"python");
	else if(strcmp(ext,"java") == 0)
		strcpy(compiler,"javac");
	else if(strcmp(ext,"html") == 0)
		strcpy(compiler,"html");
}
bool compile(){
	char command[50];
	strcpy(command, compiler);
	strcat(command," ");
	strcat(command,filename);
	if(strcmp(compiler, "javac") == 0)
	{
		system(command);
		char *classFilename = substr(filename, 0, strpos(filename,".",0));
		strcat(classFilename, ".class");
		if(!isFileExists(classFilename))
	    	return false;
	}
	else{
		strcat(command," -o temp.exe");
	    system(command);
	    if(!isFileExists("temp.exe"))
	    	return false;
    }
    return true;
}
void execute(){
	char* ext = returnFileExtension(filename);
	if(strcmp(ext, "html") == 0)
		system(filename);
	else if(strcmp(ext, "java") == 0){
		char command[50];
		strcpy(command, "start cmd /c run_java_code.exe ");
		strcat(command, filename);
		system(command);
	}
	else if(strcmp(ext, "py") == 0){
		char command[50];
		strcpy(command, "start cmd /c run_python_code.exe ");
		strcat(command, filename);
		system(command);
	}	
	else
		system("start cmd /c run_code.exe");
}
void error(){
	int LINE = 1;
	int COL = 1;
	if(countOpenBraces > 0 ){
		LINE = Braces[countOpenBraces-1][0];
		COL = Braces[countOpenBraces-1][1]+1;
		printf("\nOpen braces '{' at line %d col %d", LINE, COL);
	}
	else if(countOpenParens > 0 ){
		LINE = Parens[countOpenParens-1][0];
		COL = Parens[countOpenParens-1][1]+1;
		printf("\nOpen braces '(' at line %d col %d", LINE, COL);
	}
	changeTextColor(BLUE);
	printf("\nPress Enter to go back to Orion...");
	changeTextColor(WHITE);
	while(getch() != 13);
	CURSOR_X = CODE_X+COL;
	CURSOR_Y = CODE_Y+LINE;
	CURSOR = true;
	Error = true;
}
