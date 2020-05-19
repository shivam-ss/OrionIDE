#include "textcolor.h"
#include "strfunc.h"
#include "filefunc.h"
#include "syntaxerrors.h"
#include "spellcheck.h"
#include "cursor.h"

#define NO_OF_BRACES 100
#define NO_OF_PARANTHESIS 100
#define CODE_X 7
#define CODE_Y 16

int countOpenBraces = 0;
int Braces[NO_OF_BRACES][2];//stores location of open braces with line no. and col no.
int countOpenParens = 0;
int Parens[NO_OF_PARANTHESIS][2];//stores location of open paranthesis with line no. and col no.
WORD DEFAULT_COLOR;//Default color of text in console
int NOL;//Number of lines
char** str;//Code
int CURSOR_X;
int CURSOR_Y;

void printName(){
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
	printf("Ctrl + c -> Compile and Run\n");
	printf("Ctrl + o -> Autofill\n");
	printf("Esc      -> Exit\n\n\n");
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
void compileAndExecute(const char* filename){
	printf("\n\nOutput:\n");
	char command[50] = "gcc ";
	strcat(command,filename);
    system(command);
    
	system("a.exe");
}

void oldFileEdit(const char* filename){
	static bool CURSOR = false;
	printName();

	printf("\nEnter Code:\n");
	if(NOL == -1){
		free(str);
		countOpenBraces = 0;
		countOpenParens = 0;
		str = readArrayOfStrings(filename);
		CURSOR = false;
	}
		
	int i;
	for(i=0; i<NOL; i++){
		printf("%d\t%s",i+1,str[i]);
		if(i != NOL-1)
			printf("\n");
	}
	
	char c[2];
	c[1] = '\0';
	printCoordinates();
	if(CURSOR)
		setCursor(CURSOR_X,CURSOR_Y);
	bool compile = false;
	
	while(1){
		c[0] = getch();
		
		if(c[0]== 13){	//13 ASCII value of ENTER
			int X = getCursorX();
			int Y = getCursorY();
			
			int COL = X - CODE_X;
			int LINE = Y - CODE_Y;
			
			for(i=NOL; i>LINE; i--)
				strcpy(str[i],str[i-1]);
			char *add = substr(str[LINE-1], COL-1, strlen(str[LINE-1]));
			strcpy(str[LINE], add);
			str[LINE-1][COL-1] = '\0';
			NOL++;
			CURSOR_X = CODE_X+1;
			CURSOR_Y = CODE_Y+LINE+1;
			CURSOR = true;
			oldFileEdit(filename);
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
				strcat(str[LINE-2],str[LINE-1]);
				int len = strlen(str[LINE-2]);
				for(i=LINE; i<NOL-1; i++)
					strcpy(str[i-1],str[i]);
				NOL--;
				strcpy(str[NOL],"");
				CURSOR_X = CODE_X+len;
				CURSOR_Y = CODE_Y+LINE-1;
				CURSOR = true;
				oldFileEdit(filename);
				return;
			}
			
			substrDelete(str[LINE-1],COL-2,COL-1);
			printf("\r%d\t%s ",LINE,str[LINE-1]);
			setCursor(X-1,Y);
		}
		else if(c[0] == 19)	//15 ASCII value of CTRL + s
			writeArrayOfStrings(str, NOL, filename);
		else if(32<=c[0] && c[0]<=126){
			int X = getCursorX();
			int Y = getCursorY();
			
			int COL = X - CODE_X;
			int LINE = Y - CODE_Y;
			substrInsert(str[LINE-1],c,COL-1);
			printf("\r%d\t%s",LINE,str[LINE-1]);
			setCursor(X+1,Y);
		}
		else if(c[0] == 3){	//3 ASCII value of CTRL + c
			writeArrayOfStrings(str, NOL, filename);
			compile = true;
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
				else if(X == CODE_X+1 && LINE != 1)
					setCursor(strlen(str[LINE-2])+CODE_X+1, Y-1);
			}
			else if(c[0] == 'H' && Y != CODE_Y+1)
				setCursor(X, Y-1);
			else if(c[0] == 'P' && Y != CODE_Y+NOL)
				setCursor(X, Y+1);
			else if(c[0] == 'M'){
				if(COL-1 != strlen(str[LINE-1]))
					setCursor(X+1, Y);
				else if(COL-1 == strlen(str[LINE-1]) && LINE< NOL)
					setCursor(CODE_X+1,Y+1);
			}
			X = getCursorX();
			Y = getCursorY();
			
			COL = X - CODE_X;
			LINE = Y - CODE_Y;
			
			if((COL-1) >= strlen(str[LINE-1]))	//So that cursor doesn't go beyond line's length
				setCursor(strlen(str[LINE-1])+CODE_X+1,Y);
		}
		else if(c[0]==27){	//27 ASCII value of ESC
			setCursor(0,CODE_Y+NOL);
			exit(0);
		}
		fflush(stdin);
		printCoordinates();
	}
		
	if(compile){
		setCursor(0,CODE_Y+NOL);
		changeTextColor(FOREGROUND_RED);
		for(i=0;i<NOL;i++)
			checkSyntax(str,i+1);
		if(countOpenBraces > 0 ){
			printf("\nOpen braces '{' at line %d col %d",Braces[countOpenBraces-1][0],Braces[countOpenBraces-1][1]+1);
		}
		else if(countOpenParens > 0 ){
			changeTextColor(FOREGROUND_RED);
			printf("\nOpen braces '(' at line %d col %d",Parens[countOpenParens-1][0],Parens[countOpenParens-1][1]+1);
		}	
		else{
			changeTextColor(DEFAULT_COLOR);
			compileAndExecute(filename);
		}
		changeTextColor(DEFAULT_COLOR);	
	}
}

void newFileEdit(const char *filename){
	writeString(" ",filename);
	oldFileEdit(filename);
}

int main(int argc,char* argv[]){
	char filename[50];
	saveDefaultAttributes();
	if(argc==1){
		printName();
	    printf("Enter filename(with extension):\t");
	    scanf("%s",filename);
	}
	else
		strcpy(filename, argv[1]);
	printName();
	
	NOL = -1;
    if(isFileExists(filename))
    	oldFileEdit(filename);
    else
    	newFileEdit(filename);
	   
	return 0;
}
