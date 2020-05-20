#include <conio.h>

#include "color.h"
#include "strfunc.h"
#include "filefunc.h"
#include "spellcheck.h"
#include "cursor.h"
#include "compiler.h"
#include "logo.h"

#define CODE_X 7
#define CODE_Y 12
#define TAB_SIZE 4


int NOL;	//Number of lines
char** CODE;	//Code
int CURSOR_X;	//Contains the Current X location of cursor
int CURSOR_Y;	//Contains the Current X location of cursor
bool CURSOR;
bool Error = false;
bool changedWithoutSave = false;
int countOpenBraces = 0;
int countOpenParens = 0;
char filename[50];

void printCoordinates();
bool autofill(bool showHint);
void insertCharacter(char *c);
void insert(char *c);
void replaceTabWithSpaces();
void printFilename();
void spelling(int LINE);
void fileEdit();


void printCoordinates(){
	int X = getCursorX();
	int Y = getCursorY();
	setCursor(0,CODE_Y-2);
	printf("\r                                     ");
	printf("\rLine: %d\tCol: %d", Y-CODE_Y, X-CODE_X);
	setCursor(X,Y);
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
void heighlightError(int LINE){
	setCursor(CODE_X+1,CODE_Y+LINE);
	printf("%d\t%s",LINE,CODE[LINE-1]);
}
void insert(char* c){
	changedWithoutSave = true;
	printFilename();
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
			autofill(true);
		return;
	}
	setCursor(getCursorX()-1,getCursorY());
}
bool autofill(bool showHint){
	int X = getCursorX();
	int Y = getCursorY();
	
	int COL = X - CODE_X;
	int LINE = Y - CODE_Y;
	
	char *word = autocomplete(CODE[LINE-1],COL,returnFileExtension(filename));
	if(strlen(word) == 0)
		return false;
	 
	if(showHint){
		changeTextColor(DARKGRAY);
		printf("%s",word);
		changeTextColor(WHITE);
		setCursor(X,Y);
	}
	else{
		char c[2];
		c[1] = '\0';
		int j;
		for(j=0; j<strlen(word); j++){
			c[0] = word[j];
			insert(c);
		}
	}
	return true;
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
void printFilename(){
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
	checkSpelling(CODE[LINE-1], returnFileExtension(filename));
	printf("                      ");
	setCursor(X,Y);
}
void fileEdit(){
	printName();
	printFilename();
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
	if(Error){
		int X = getCursorX();
		int Y = getCursorY();
		
		int COL = X - CODE_X;
		int LINE = Y - CODE_Y;
		printf("\r%d\t",LINE);
		changeBackgroundColor(RED,WHITE);
		printf("%s",CODE[LINE-1]);
		changeTextColor(WHITE);
		setCursor(X,Y);
	}
	
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
			fileEdit(compiler);
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
				fileEdit(compiler);
				return;
			}
			changedWithoutSave = true;
			printFilename();
			substrDelete(CODE[LINE-1],COL-2,COL-1);
			printf("\r%d\t%s ",LINE,CODE[LINE-1]);
			printf("                      ");
			setCursor(X-1,Y);
			if(COL-2 == strlen(CODE[LINE-1]))
				autofill(true);
			spelling(LINE);
		}
		else if(c[0] == 19 ){	//15 ASCII value of CTRL + s
			if(changedWithoutSave == false)
				continue;
			writeArrayOfStrings(CODE, NOL, filename);
			changedWithoutSave = false;
			printFilename();
		}
		else if(32<=c[0] && c[0]<=126){
			insert(c);
		}
		else if(c[0] == 9){	//9 ASCII value of TAB
			if(!autofill(false)){
				int j;
				for(j=0; j<TAB_SIZE; j++)
					insertCharacter(" ");
				changedWithoutSave = true;
				printFilename();
			}
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
		else if(c[0] == 0){
			c[0] = getch();
			if(c[0] == 59){//ASCII value for F1
				CURSOR_X = getCursorX();
				CURSOR_Y = getCursorY();
				CURSOR = true;
				help();
				fileEdit(compiler);
				return;
			}
			else if(c[0] == 68){//ASCII value for F10
				if(changedWithoutSave == true)
					writeArrayOfStrings(CODE, NOL, filename);
				changedWithoutSave = false;
				printFilename();
				CURSOR_X = getCursorX();
				CURSOR_Y = getCursorY();
				CURSOR = true;
				compileAndRun = true;
				break;
			}
		}
		else
			printf("\a");
	
		fflush(stdin);
		printCoordinates();
	}
	if(strcmp(compiler, "html") == 0 || strcmp(compiler, "python") == 0)
		execute();
	else if(compile){
		setCursor(0, CODE_Y+NOL+1);
		changeTextColor(RED);
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
		else if(countOpenBraces > 0 || countOpenParens > 0 )
			error();
		else{
			if(compile())
				execute();
			else
				error();
		}
		changeTextColor(WHITE);
	}
	fileEdit();
}
int main(int argc,char* argv[]){
	
	saveDefaultAttributes();
	if(argc == 1){
	    printf("Enter filename(with extension):\t");
	    scanf("%s",filename);
	}
	else
		strcpy(filename, argv[1]);
	
	selectCompiler();
	NOL = -1;
	
    if(isFileExists(filename))
    	fileEdit();
    else{
    	writeString(" ",filename);
    	fileEdit();
    }
	   
	return 0;
}
