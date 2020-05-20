extern char filename[50];

void printName();
void help();

void printName(){
	system("cls");
	changeTextColor(BLUE);
	printf("\t     ****     *****      *******     ****     **     *   \n");
	printf("\t    *    *    *    *        *       *    *    * *    *   \n");
	printf("\t   *      *   *****         *      *      *   *  *   *   \n");
	printf("\t   *      *   *    *        *      *      *   *   *  *   \n");
	printf("\t    *    *    *     *       *       *    *    *    * *   \n");
	printf("\t     ****     *      *   *******     ****     *     **   \n\n");
	changeTextColor(DARKGRAY);
	printf("F1 ==> Help\n");
	changeTextColor(WHITE);
}

void help(){
	system("cls");
	char* ext = returnFileExtension(filename);
	changeTextColor(LIGHTGRAY);
	printf("SHORTCUTS:\n");
	printf("Ctrl + s -> Save\n");
	
	if(strcmp(ext, "html") == 0)
		printf("F10      -> Run\n");
	else
		printf("F10      -> Compile and Run\n");
	
	printf("Tab      -> Autofill\n");
	printf("Esc      -> Exit\n\n");
	
	changeTextColor(LIGHTGREEN);
	printf("Green - predefined functions\n");
	changeTextColor(BROWN);
	printf("Brown - keyword\n");
	changeTextColor(LIGHTCYAN);
	printf("Cyan  - preprocessor directives\n");
	changeTextColor(LIGHTBLUE);
	printf("Blue  - predefined header files\n");
	changeTextColor(RED);
	printf("Red   - word doesn't exist\n");
	changeTextColor(LIGHTGRAY);
	printf("Gray  - hint\n\n");
	changeTextColor(BLUE);
	printf("\n\nPress Enter to go back to Orion...");
	changeTextColor(WHITE);
	while(getch() != 13);	//12 ASCII value of Enter
}
