#include <conio.h>
#include <SDKDDKVer.h>
#include<stdio.h>

#include "color.h"
#include "strfunc.h"
#include "filefunc.h"
#include "spellcheck.h"
#include "cursor.h"
#include "compiler.h"
#include "logo.h"
#include "preprocessing.h"

#define CODE_X 7
#define CODE_Y 8

int NOL = -1;	//Number of lines
char** CODE;	//Code
int CURSOR[2] = { 0 };	//Contains the Current X and Y location of cursor
bool changedWithoutSave = false;
char filename[50];
bool hintAvailable = false;	//tells whether there is any hint available

void printCoordinates();
bool autofill(bool showHint);
void insert(char *c);
void printLine();
void replaceTabWithSpaces();
void printFilename();
void spelling(int LINE);
void heighlightError(int LINE);
void editFile();
void saveFile();


void printCoordinates(){
	int X = getCursorX();
	int Y = getCursorY();
	setCursor(0,CODE_Y+NOL+1);
	changeTextColor(DARKGRAY);
	printf("\r                                     ");
	printf("\rLine: %d\tCol: %d", Y-CODE_Y, X-CODE_X);
	changeTextColor(WHITE);
	setCursor(X,Y);
}
void printLine(){
	if(!hintAvailable)
		return;
	hintAvailable = false;
	int X = getCursorX();
	int Y = getCursorY();
	
	int COL = X - CODE_X;
	int LINE = Y - CODE_Y;
	spelling(LINE);
	setCursor(X,Y);
}
void insert(char* c){
	changedWithoutSave = true;
	bool closure = true;
	printFilename();
	if(c[0] == '{')
		strcpy(c,"{}");
	else if(c[0] == '(')
		strcpy(c,"()");
	else if(c[0] == '[')
		strcpy(c,"[]");
	else if(c[0] == '"')
		strcpy(c,"\"\"");
	else if(c[0] == '\'')
		strcpy(c,"''");
	else{
		closure = false;
		if(c[0] == '\\' && c[1] == 't'){
			strcpy(c,"");
			int i;
			for(i=0;i<TAB_SIZE;i++)
				strcat(c," ");
		}
	}
	int X = getCursorX();
	int Y = getCursorY();
	int COL = X - CODE_X;
	int LINE = Y - CODE_Y;
	substrInsert(CODE[LINE-1],c,COL-1);
	printf("\r%d\t%s",LINE,CODE[LINE-1]);
	printf("                           ");
	COL += strlen(c);
	if(closure)
		setCursor(CODE_X+COL-1,Y);
	else{
		setCursor(CODE_X+COL,Y);
		spelling(LINE);
		if(COL-1 == strlen(CODE[LINE-1]))
			autofill(true);
	}	
}
bool autofill(bool showHint){
	hintAvailable = false;
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
		hintAvailable = true;
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
void heighlightError(int LINE){
	int X = getCursorX();
	int Y = getCursorY();
	
	changeBackgroundColor(RED,WHITE);
	setCursor(CODE_X+1,CODE_Y+LINE);
	printf("%s",CODE[LINE-1]);
	changeBackgroundColor(BLACK,WHITE);
	
	setCursor(X,Y);
}
void saveFile(){
	if(changedWithoutSave == false)
		return;
	writeArrayOfStrings(CODE, NOL, filename);
	changedWithoutSave = false;
	printFilename();
}
void editFile(){
	printName();
	printFilename();
	setCursor(0,CODE_Y+1);
	
	int i;
	for(i=0; i<NOL; i++){
		spelling(i+1);
		if(i != NOL-1)
			printf("\n");
		else
			setCursor(CODE_X+strlen(CODE[i])+1, getCursorY());
	}
	
	char c[TAB_SIZE+1];
	c[1] = '\0';
	printCoordinates();
	if(CURSOR[0])
		setCursor(CURSOR[0],CURSOR[1]);
	bool compileAndRun = false;
	
	while(1){
		c[0] = getch();
		c[1] = '\0';
		
		if(c[0]== 13){	//13 ASCII value of ENTER==>To move to new line
			
			int COL = getCursorX() - CODE_X;
			int LINE = getCursorY() - CODE_Y;
			
			for(i=NOL; i>LINE; i--)
				strcpy(CODE[i],CODE[i-1]);
			char *add = substr(CODE[LINE-1], COL-1, strlen(CODE[LINE-1]));
			strcpy(CODE[LINE], add);
			CODE[LINE-1][COL-1] = '\0';
			NOL++;
			CURSOR[0] = CODE_X+1;
			CURSOR[1] = CODE_Y+LINE+1;
			changedWithoutSave = true;
			editFile();
			return;
		}
		else if(c[0] == 8){	//8 ASCII value of BACKSPACE==>To delete previous character
			int X = getCursorX();
			int Y = getCursorY();
			
			int COL = X - CODE_X;
			int LINE = Y - CODE_Y;
			if(COL==1 && LINE ==1)//As Cursor is on 1,1 there can't be delete operation
				continue;
			if(COL==1){
				int len = strlen(CODE[LINE-2]);
				strcat(CODE[LINE-2],CODE[LINE-1]);
				for(i=LINE; i<NOL; i++)
					strcpy(CODE[i-1],CODE[i]);
				NOL--;
				strcpy(CODE[NOL],"");
				CURSOR[0] = CODE_X+len+1;
				CURSOR[1] = CODE_Y+LINE-1;
	
				changedWithoutSave = true;
				editFile(compiler);
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
		else if(c[0] == 19)	//19 ASCII value of CTRL + s==>Save
			saveFile();
		else if(32<=c[0] && c[0]<=126)
			insert(c);
		else if(c[0] == 9){	//9 ASCII value of TAB==>For autofill and TAB spaces
			if(!autofill(false))
				insert("\t");
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
					printLine();
					setCursor(strlen(CODE[LINE-2])+CODE_X+1, Y-1);
				}
			}
			else if(c[0] == 'H' && Y != CODE_Y+1){
				printLine();
				setCursor(X, Y-1);
			}
			else if(c[0] == 'P' && Y != CODE_Y+NOL){
				printLine();
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
		else if(c[0]==27){	//27 ASCII value of ESC==>Exit
			saveFile();
			setCursor(0,CODE_Y+NOL);
			system("cls");
			exit(0);
		}
		else if(c[0] == 0){
			c[0] = getch();
			if(c[0] == 59){//ASCII value for F1==>Help
				CURSOR[0] = getCursorX();
				CURSOR[1] = getCursorY();
				help();
				editFile();
				return;
			}
			else if(c[0] == 68){//ASCII value for F10==>Compile and run
				saveFile();
				CURSOR[0] = getCursorX();
				CURSOR[1] = getCursorY();
				compileAndRun = true;
				break;
			}
			else//Invalid key press
				printf("\a");
		}
		else//Invalid key press
			printf("\a");
	
		fflush(stdin);
		printCoordinates();
	}
	setCursor(0, CODE_Y+NOL+2);// to print error message at last
	if(compile())
		execute();
	else
		error();	
	editFile();
}

int main(int argc,char* argv[]){
	
	if(argc == 1){
	    printf("Enter filename(with extension):\t");
	    scanf("%s",filename);
	}
	else
		strcpy(filename, argv[1]);
	preprocessing();
		   
	return 0;
}
