#include <stdio.h>
#include "strfunc.h"

int main(int argc,char* argv[]){
	
	char filename[50];
	strcpy(filename, argv[1]);
	
	char command[60];
	strcpy(command, "python ");
	strcat(command, filename);

	system("cls");
	printf("Output:\n");
	system(command);
	printf("\n\nPress Enter to go back to Orion...");
	while(getch() != 13);// ASCII value of ENTER
	system("exit_cmd.bat");
	return 0;
}
