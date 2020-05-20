extern char filename[50];
char compiler[7];

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
	if(strcmp(compiler, "html") == 0)
		return true;
	changeTextColor(RED);
	char command[50];
	strcpy(command, compiler);
	strcat(command," ");
	strcat(command,filename);
	strcat(command," 2> error.txt");
	if(strcmp(compiler, "javac") != 0 && strcmp(compiler, "python") != 0)
		strcat(command," -o temp.exe");
	system(command);
	if(isFileExists("error.txt"))
	    return false;
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
int parseInt(char* str){
	int integer = str[0]-'0';
	int i;
	for(i=1;i<strlen(str);i++)
		integer = integer*10 + str[i]-'0';
	return integer;
}
void error(){
	FILE *fptr = fopen("error.txt", "r");
	char str[LINE_LENGTH];
	char* ext = returnFileExtension(filename);
	while(fgets(str,LINE_LENGTH-1,fptr)){//for c program
		str[strlen(str)-1] = '\0';
		changeTextColor(RED);
		printf("\n%s",str);
		if(strcmp(ext,"c")==0){
			char *fname = strtok(str,":");
			if(!fname && strcmp(fname,"temp.c")!=0)
				continue;
			char *line = strtok(NULL,":");
			if(!line)
				continue;
			char *col = strtok(NULL,":");
			if(!col)
				continue;
			char *err = strtok(NULL,":");
			if(!err && strcmp(err," error")!=0)
				continue;
			heighlightError(parseInt(line));
		}
	}
	fclose(fptr);
	system("del \f error.txt");
	changeTextColor(BLUE);
	printf("\nPress Enter to go back to Orion...");
	changeTextColor(WHITE);
	while(getch() != 13);
}
