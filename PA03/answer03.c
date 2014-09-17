#include <stdio.h>
#include <string.h>
#include "answer03.h"

char * strcat_ex(char * * dest, int * n, const char * src)
{
    //EXECUTABLE STATEMENTS
    if(*dest == NULL || *n <  strlen(*dest) + strlen(src) + 1)
    {
        int totLen = (*dest == NULL ? 0 : strlen(*dest)) + strlen(src);
	*n = 1 + 2 * totLen;
	char * buffer = malloc(*n);
	strcpy(buffer, *dest);	
	free(*dest);
	*dest = buffer;
    }
    strcat(*dest, src);

    return *dest;
}

/*char * * explode(const char * str, const char * delims, int * arrLen)
{

}*/
