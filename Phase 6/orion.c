#include "color.h"
#include "strfunc.h"
#include "filefunc.h"
#include "syntaxerrors.h"
#include "spellcheck.h"
#include "cursor.h"

#define NO_OF_BRACES 100
#define NO_OF_PARANTHESIS 100
#define CODE_X 7
#define CODE_Y 21
#define TAB_SIZE 4

int countOpenBraces = 0;
int Braces[NO_OF_BRACES][2];	//stores location of open braces with line no. and col no.
int countOpenParens = 0;
int Parens[NO_OF_PARANTHESIS][2];	//stores location of open paranthesis with line no. and col no.
WORD DEFAULT_COLOR;	//Default color of text in console
int NOL;	//Number of lines
char** CODE;	//Code
int CURSOR_X;	//Contains the Current X location of cursor
int CURSOR_Y;	//Contains the Current X location of cursor
bool CURSOR;
bool changedWithoutSave = false;

void printName(const char* filename);
void printCoordinates();
char* selectCompiler(const char* filename);
bool compile(const char* compiler, const char* filename);
void execute(const char* filename);
void spelling(int LINE);
bool autofill(bool showHint, const char* filename);
void insertCharacter(char *c);
void insert(char *c, const char* filename);
void error();
void replaceTabWithSpaces();
void printFilename(const char* filename);
void oldFileEdit(const char* compiler, const char* filename);
void newFileEdit(const char* compiler, const char* filename);


void printName(const char* filename){
	char* ext = returnFileExtension(filename);
	system("cls");
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
	
	if(strcmp(ext, "html") == 0)
		printf("Ctrl + c -> Run\n");
	else
		printf("Ctrl + c -> Compile and Run\n");
	
	printf("Tab      -> Autofill\n");
	printf("Esc      -> Exit\n\n");
	
	changeTextColor(FOREGROUND_GREEN);
	printf("Green - keyword\n");
	changeTextColor(FOREGROUND_RED);
	printf("Red - word doesn't exist\n");
	changeTextColor(FOREGROUND_INTENSITY);
	printf("Gray - hint\n\n");
	changeTextColor(DEFAULT_COLOR);
}
void printCoordinates(){
	int X = getCursorX();
	int Y = getCursorY();
	setCursor(0,CODE_Y-2);
	printf("\r                                           ");
	printf("\rLine: %d\tCol: %d", Y-CODE_Y, X-CODE_X);
	setCursor(X,Y);
}
char* selectCompiler(const char* filename){
	char static compiler[7];
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
	
	return compiler;
}
bool compile(const char* compiler, const char* filename){
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
void execute(const char* filename){
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
void insertCharacter(char *c){
	int X = getCursorX();
	int Y = getCursorY();
	
	int COL = X - CODE_X;
	int LINE = Y - CODE_Y;
	substrInsert(CODE[LINE-1],c,COL-1);
	printf("\r%d\t%s",LINE,CODE[LINE-1]);
	printf("                   ");
	setCursor(X+1,Y);
	spelling(LINE);
}
void printline(){
	int X = getCursorX();
	int Y = getCursorY();
	
	int COL = X - CODE_X;
	int LINE = Y - CODE_Y;
	spelling(LINE);
	setCursor(X,Y);
}
void insert(char* c, const char* filename){
	changedWithoutSave = true;
	printFilename(filename);
	insertCharacter(c);
	if(c[0] == '{')
		insertCharacter("}");
	else if(c[0] == '(')
		insertCharacter(")");
	else if(c[0] == '[')
		insertCharacter("]");
	else if(c[0] == '"')
		insertCharacter(c);
	else if(c[0] == '\'')
		insertCharacter(c);
	else{
		int X = getCursorX();
		int Y = getCursorY();
		
		int COL = X - CODE_X;
		int LINE = Y - CODE_Y;
		if(COL-1 == strlen(CODE[LINE-1]))
			autofill(true,filename);
		return;
	}
	setCursor(getCursorX()-1,getCursorY());
}
bool autofill(bool showHint, const char* filename){
	int X = getCursorX();
	int Y = getCursorY();
	
	int COL = X - CODE_X;
	int LINE = Y - CODE_Y;
	
	char *word = autocomplete(CODE[LINE-1],COL);
	if(strlen(word) == 0)
		return false;
	 
	if(showHint){
		changeTextColor(FOREGROUND_INTENSITY);
		printf("%s",word);
		changeTextColor(DEFAULT_COLOR);
		setCursor(X,Y);
	}
	else{
		char c[2];
		c[1] = '\0';
		int j;
		for(j=0; j<strlen(word); j++){
			c[0] = word[j];
			insert(c, filename);
		}
	}
	return true;
}
void error(){
	changeTextColor(FOREGROUND_BLUE);
	printf("\nPress Enter to go back to Orion...");
	changeTextColor(DEFAULT_COLOR);
	while(getch() != 13);
}
void replaceTabWithSpaces(){
	int i;
	for(i=0; i<NOL; i++){
		int index = strpos(CODE[i],"\t",0);
		if(index == -1)
			continue;
		substrDelete(CODE[i],index,index+1);
		int j;
		for(j=0; j<TAB_SIZE; j++)
			substrInsert(CODE[i]," ",index);
	}
}
void printFilename(const char* filename){
	int X = getCursorX();
	int Y = getCursorY();
	setCursor(0,CODE_Y);
	if(changedWithoutSave)
		printf("\r[*]%s",filename);
	else
		printf("\r%s   ",filename);
	setCursor(X,Y);
}
void spelling(int LINE){
	int X = getCursorX();
	int Y = getCursorY();
	
	setCursor(0,CODE_Y+LINE);
	printf("%d\t",LINE);
	checkSpelling(CODE[LINE-1]);
	printf("                      ");
	setCursor(X,Y);
}
void oldFileEdit(const char* compiler, const char* filename){
	printName(filename);
	printFilename(filename);
	setCursor(0,CODE_Y+1);
		
	if(NOL == -1){
		changedWithoutSave = false;
		free(CODE);
		CODE = readArrayOfStrings(filename);
		replaceTabWithSpaces();
		CURSOR = false;
	}
		
	int i;
	for(i=0; i<NOL; i++){
		spelling(i+1);
		if(i != NOL-1)
			printf("\n");
		else
			setCursor(CODE_X+strlen(CODE[i])+1, getCursorY());
	}
	
	char c[2];
	c[1] = '\0';
	printCoordinates();
	if(CURSOR)
		setCursor(CURSOR_X,CURSOR_Y);
	bool compileAndRun = false;
	
	while(1){
		c[0] = getch();
		
		if(c[0]== 13){	//13 ASCII value of ENTER
			int X = getCursorX();
			int Y = getCursorY();
			
			int COL = X - CODE_X;
			int LINE = Y - CODE_Y;
			
			for(i=NOL; i>LINE; i--)
				strcpy(CODE[i],CODE[i-1]);
			char *add = substr(CODE[LINE-1], COL-1, strlen(CODE[LINE-1]));
			strcpy(CODE[LINE], add);
			CODE[LINE-1][COL-1] = '\0';
			NOL++;
			CURSOR_X = CODE_X+1;
			CURSOR_Y = CODE_Y+LINE+1;
			CURSOR = true;
			changedWithoutSave = true;
			oldFileEdit(compiler, filename);
			return;
		}
		else if(c[0] == 8){	//8 ASCII value of BACKSPACE
			int X = getCursorX();
			int Y = getCursorY();
			
			int COL = X - CODE_X;
			int LINE = Y - CODE_Y;
			if(COL==1 && LINE ==1)
				continue;
			if(COL==1){
				int len = strlen(CODE[LINE-2]);
				strcat(CODE[LINE-2],CODE[LINE-1]);
				for(i=LINE; i<NOL; i++)
					strcpy(CODE[i-1],CODE[i]);
				NOL--;
				strcpy(CODE[NOL],"");
				CURSOR_X = CODE_X+len+1;
				CURSOR_Y = CODE_Y+LINE-1;
				CURSOR = true;
				changedWithoutSave = true;
				oldFileEdit(compiler, filename);
				return;
			}
			changedWithoutSave = true;
			printFilename(filename);
			substrDelete(CODE[LINE-1],COL-2,COL-1);
			printf("\r%d\t%s ",LINE,CODE[LINE-1]);
			printf("                      ");
			setCursor(X-1,Y);
			if(COL-2 == strlen(CODE[LINE-1]))
				autofill(true,filename);
			spelling(LINE);
		}
		else if(c[0] == 19 ){	//15 ASCII value of CTRL + s
			if(changedWithoutSave == false)
				continue;
			writeArrayOfStrings(CODE, NOL, filename);
			changedWithoutSave = false;
			printFilename(filename);
		}
		else if(32<=c[0] && c[0]<=126){
			insert(c,filename);
		}
		else if(c[0] == 9){	//9 ASCII value of TAB
			if(!autofill(false,filename)){
				int j;
				for(j=0; j<TAB_SIZE; j++)
					insertCharacter(" ");
				changedWithoutSave = true;
				printFilename(filename);
			}
		}
		else if(c[0] == 3){	//3 ASCII value of CTRL + c
			if(changedWithoutSave == true)
				writeArrayOfStrings(CODE, NOL, filename);
			changedWithoutSave = false;
			printFilename(filename);
			CURSOR_X = getCursorX();
			CURSOR_Y = getCursorY();
			CURSOR = true;
			compileAndRun = true;
			break;
		}
		else if(c[0] == -32){	//Arrow Key is Pressed
			//K - left arrow
			//H - up arrow
			//P - down arrow
			//M - right arrow
			c[0] = getch();
			int X = getCursorX();
			int Y = getCursorY();
			
			int COL = X - CODE_X;
			int LINE = Y - CODE_Y;
			
			if(c[0] == 'K'){
				if(X != CODE_X+1)
					setCursor(X-1, Y);
				else if(X == CODE_X+1 && LINE != 1){
					printline();
					setCursor(strlen(CODE[LINE-2])+CODE_X+1, Y-1);
				}
			}
			else if(c[0] == 'H' && Y != CODE_Y+1){
				printline();
				setCursor(X, Y-1);
			}
			else if(c[0] == 'P' && Y != CODE_Y+NOL){
				printline();
				setCursor(X, Y+1);
			}
			else if(c[0] == 'M'){
				if(COL-1 != strlen(CODE[LINE-1]))
					setCursor(X+1, Y);
				else if(COL-1 == strlen(CODE[LINE-1]) && LINE< NOL)
					setCursor(CODE_X+1,Y+1);
			}
			X = getCursorX();
			Y = getCursorY();
			
			COL = X - CODE_X;
			LINE = Y - CODE_Y;
				
			if((COL-1) >= strlen(CODE[LINE-1]))	//So that cursor doesn't go beyond line's length
				setCursor(strlen(CODE[LINE-1])+CODE_X+1,Y);
		}
		else if(c[0]==27){	//27 ASCII value of ESC
			setCursor(0,CODE_Y+NOL);
			system("cls");
			exit(0);
		}
	
		fflush(stdin);
		printCoordinates();
	}
	if(strcmp(compiler, "html") == 0 || strcmp(compiler, "python") == 0)
		execute(filename);
	else if(compile){
		setCursor(0, CODE_Y+NOL+1);
		changeTextColor(FOREGROUND_RED);
		fflush(stdin);
		countOpenBraces = 0;
		countOpenParens = 0;
		bool flag = false;
		for(i=1;i<=NOL;i++){
			if(checkSyntax(CODE,i))
				flag = true;
		}
		if(flag)
			error();
		else if(countOpenBraces > 0 ){
			int LINE = Braces[countOpenBraces-1][0];
			int COL = Braces[countOpenBraces-1][1]+1;
			printf("\nOpen braces '{' at line %d col %d", LINE, COL);
			error();
		}
		else if(countOpenParens > 0 ){
			int LINE = Parens[countOpenParens-1][0];
			int COL = Parens[countOpenParens-1][1]+1;
			printf("\nOpen braces '(' at line %d col %d", LINE, COL);
			error();
		}	
		else{
			if(compile(compiler, filename))
				execute(filename);
			else
				error();
		}
		changeTextColor(DEFAULT_COLOR);
	}
	oldFileEdit(compiler, filename);
}
void newFileEdit(const char* compiler, const char* filename){
	writeString(" ",filename);
	oldFileEdit(compiler, filename);
}
int main(int argc,char* argv[]){
	
	char filename[50];
	saveDefaultAttributes();
	if(argc == 1){
	    printf("Enter filename(with extension):\t");
	    scanf("%s",filename);
	}
	else
		strcpy(filename, argv[1]);
	
	char *compiler = selectCompiler(filename);
	NOL = -1;
	
    if(isFileExists(filename))
    	oldFileEdit(compiler, filename);
    else
    	newFileEdit(compiler, filename);
	   
	return 0;
}
