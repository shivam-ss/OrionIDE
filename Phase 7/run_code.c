#include <stdio.h>

int main(){
	system("cls");
	printf("Output:\n");
	system("temp.exe");
	system("del temp.exe");
	printf("\n\nPress Enter to go back to Orion...");
	while(getch() != 13);// ASCII value of ENTER
	system("exit_cmd.bat");
	return 0;
}
