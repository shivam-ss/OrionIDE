#define NO_OF_BRACES 100
#define NO_OF_PARANTHESIS 100

extern int countOpenBraces;
extern int Braces[NO_OF_BRACES][2];//stores location of open braces with line no. and col no.
extern int countOpenParens;
extern int Parens[NO_OF_PARANTHESIS][2];//stores location of open paranthesis with line no. and col no.
extern WORD DEFAULT_COLOR;//Default color of text in console

bool checkSyntax(const char **str, int l);
bool missingBraces(const char **str, int l);
bool missingParenthesis(const char **str, int l);

bool checkSyntax(const char **str, int l)
{
	return missingParenthesis(str, l) || missingBraces(str, l);
}

bool missingBraces(const char **str, int l)
{
	int i=0;
	int *openBrace = strcount(str[l-1],"{",0);
	for(i=0 ; i<openBrace[0] ; i++)
	{
		countOpenBraces++;
		Braces[countOpenBraces-1][0] = l;
		Braces[countOpenBraces-1][1] = openBrace[i+1];
	}
	
	int *closeBrace = strcount(str[l-1],"}",0);
	for(i=0 ; i<closeBrace[0] ; i++)
	{
		countOpenBraces--;
		if(countOpenBraces<0)
		{
			printf("\nError at line %d col %d",l,closeBrace[i+1]+1);
			return true;
		}
	}
	return false;
}

bool missingParenthesis(const char **str, int l)
{
	int i=0;
	int *openParen = strcount(str[l-1],"(",0);
	for(i=0 ; i<openParen[0] ; i++)
	{
		countOpenParens++;
		Parens[countOpenParens-1][0] = l;
		Parens[countOpenParens-1][1] = openParen[i+1];
	}
	
	int *closeParen = strcount(str[l-1],")",0);
	for(i=0 ; i<closeParen[0] ; i++)
	{
		countOpenParens--;
		if(countOpenParens<0)
		{
			printf("\nError at line %d col %d",l,closeParen[i+1]+1);
			return true;
		}
	}
	return false;
}
