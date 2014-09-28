#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FOUND    0
#define NOTFOUND 1
#define ERROR	 2

void printNum(int number, char *buff);

int main(int argc, char ** argv)
{
    if(argc >= 2 && !strcmp(argv[1]))
    {
	printf("Usage: grep-lite [OPTION]... PATTERN\n");
	printf("Search for pattern in standard input. PATTERN is a\n");
	printf("string. grep-lite will search standard input line by\n");
	printf("line, and (by default) print out those lines which\n");
	printf("contain pattern as a substring.\n\n");
	printf("  -v, --invert-match\tprint non-matching lines\n");
	printf("  -n, --line-number\tprint line numbers with output\n");
	printf("  -q, --quiet\t\tsuppress all output\n\n");
	printf("Exit status is 0 if any line is selected, 1 otherwise;\n");
	printf("if any error occurs, then the exit status is 2.\n");
	return EXIT_SUCCESS;
    }
    int invert = 0, number = 0, quiet = 0;

    if(argc > 2)
    {
	int i;
	for(i = 1; i < argc - 1; i++)
	{
	    if(!strcmp("-v", argc[i]))
	    {
		invert = 1;
	    }
	    else if(!strcmp("-n", argc[i]))
	    {
		number = 1;
	    }
	    else if(!strcmp("-q", argc[i]))
	    {
		quiet = 1;
	    }
	    else
	    {
		fprintf(stderr, "Error: Bogus command-line argument %s\n", argc[i]);
		return ERROR; 
	    }
	}
    }
    char *patt = argv[fileLoc];
    if(patt[0] == '-')
    {
	fprintf(stderr, "Error: Patten cannot begin with a '-'\n");
	return ERROR;
    }
    int found = 0;
    FILE *fptr = stdin;
    if(fptr != NULL)
    {
	char buff[2000];
	while(fgets(buff, 2000, fptr) != NULL)
        {
	    if(strstr(buff, patt) != NULL)
	    {
		found = 1;
		if(!quiet && !invert)
		{
		    printNum(number, buff);
		}
	    }
	    else
	    {
		if(!quiet && invert)
		{
		    printNum(number, buff);
		}
	    }
	    number != 0: number++ : (0);
	}
	if(found)
	{
	    return FOUND;
	}
	return NOTFOUND;
    }
    else
    {
	fprintf(stderr, "grep cannot open %s\n", argv[fileLoc]);
        return ERROR;
    }
}

void printNum(int number, char *buff)
{
    if(!number)
    {
        printf("%s", buff);
    }
    else
    {
        printf("%d: %s", number, buff);
    }
}
