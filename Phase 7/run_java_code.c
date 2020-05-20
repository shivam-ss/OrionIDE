#include <stdio.h>
#include "strfunc.h"

int main(int argc,char* argv[]){
	
	char filename[50];
	strcpy(filename, argv[1]);
	char *filenameWithoutExt = substr(filename, 0, strpos(filename,".",0));
	
	char command[60];
	strcpy(command, "java ");
	strcat(command, filenameWithoutExt);

	system("cls");
	printf("Output:\n");
	system(command);
	
	//deleting .class file
	strcpy(command, "del ");
	strcat(command, filenameWithoutExt);
	strcat(command, ".class");
	system(command);
	
	printf("\n\nPress Enter to go back to Orion...");
	while(getch() != 13);// ASCII value of ENTER
	system("exit_cmd.bat");
	return 0;
}
