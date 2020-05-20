#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NO_OF_LINES 1000
#define LINE_LENGTH 80

extern int NOL; //Number of lines

bool writeArrayOfStrings(const char **str, int len, const char *filename){
	int i;
	char *line;
	line = (char*)malloc(LINE_LENGTH * sizeof(char));
	strcpy(line,"");
	for(i=0; i < len ;i++)
	{
		strcat(line,str[i]);
		if(i != len-1)	//So that there is no \n at the end of file
			strcat(line,"\n");
	}
	
	FILE *fptr = fopen(filename, "w"); //fptr - File Pointer
	
    if (fptr == NULL) 
        return false;
        
    // Write data line by line into the file 
    if (strlen(line) > 0) 
    { 
        fputs(line, fptr) ;    
        fputs("\n", fptr) ; 
    } 
    
    fclose(fptr) ;
    return true;
}

bool writeString(const char *str, const char *filename){
	
	FILE *fptr = fopen(filename, "w"); //fptr - File Pointer
	
    if (fptr == NULL) 
        return false;
        
    // Write data into the file 
    if (strlen(str) > 0){
    	int i;
    	for (i = 0; str[i]!='\0'; i++)
        	fputc(str[i], fptr); 
	}
    
    fclose(fptr);
    return true;
}

char** readArrayOfStrings(const char *filename){
	FILE *fptr = fopen(filename, "r"); //fptr - File Pointer

    if (fptr == NULL) 
        return NULL;
    
	static char **str;
	str = (char**)calloc(NO_OF_LINES, sizeof(char*));
	int i;
    for (i=0; i<NO_OF_LINES; i++)
    	str[i] = (char*)calloc(LINE_LENGTH, sizeof(char));
    
    i = 0;
    //File is read line by line to an array of strings
	while(fgets(str[i],LINE_LENGTH-1,fptr)){
		str[i][strlen(str[i])-1] = '\0';
		i++;
	}
	NOL = i;
	fclose(fptr);
	
	return str;
}

char* readString(const char *filename){
	FILE *fptr = fopen(filename, "r"); //fptr - File Pointer

    if (fptr == NULL) 
        return NULL;
    
	static char *str;
	str = (char*)malloc(NO_OF_LINES * LINE_LENGTH * sizeof(char));
	strcpy(str,"");
	
	char c[2];
	c[1] ='\0';
	c[0] = fgetc(fptr);
	
	//File is read character by character 
    while (c[0] != EOF){	
        c[0] = fgetc(fptr); 
        strcat(str,c);
    }
    fclose(fptr);
	
	return str;
}

bool isFileExists(const char *path){
    FILE *fptr = fopen(path, "r");
    
    if (fptr == NULL)
        return false;
        
    fclose(fptr);

    return true;
}

char* returnFileExtension(const char *path){
	return strchr(path, '.')+1;
}
