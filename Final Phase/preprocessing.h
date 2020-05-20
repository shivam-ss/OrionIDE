#define TAB_SIZE 4

extern char filename[50];
extern int NOL;
extern char** CODE;
extern int CURSOR[2];

void replaceTabWithSpaces();
void preprocessing();

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

void preprocessing(){
	saveDefaultAttributes();
	selectCompiler();
	
    if(!isFileExists(filename))
    	writeString(" ",filename);//Creates an empty file
	CODE = readArrayOfStrings(filename);
	replaceTabWithSpaces();
	CURSOR[0] = 0;
	editFile();
}
