#include <string.h>
#include <stdbool.h>

bool checkword(const char *str)
{
	char c[1000];
    FILE *fptr;

    if ((fptr = fopen("words.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        // Program exits if file pointer returns NULL.
        exit(1);         
    }

    // reads text until newline 
    fscanf(fptr,"%[^\n]", c);

    printf("Data from the file:\n%s", c);
    fscanf(fptr,"%[^\n]", c);

    printf("Data from the file:\n%s", c);
    fclose(fptr);
	return false;
}

