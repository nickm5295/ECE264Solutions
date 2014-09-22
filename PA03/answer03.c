#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer03.h"
int insideString(char chr, const char * strng);
int smallerString(const void **str1,const void **str2);
void addString(char *test, const char *delims, const char *str, int *loc, int *arrLen, char ***result);
int cmpChar(const void *arg1, const void *arg2);

char * strcat_ex(char * * dest, int * n, const char * src)
{
    //EXECUTABLE STATEMENTS
    if(*dest == NULL || *n <  strlen(*dest) + strlen(src) + 1)
    {
        int totLen = (*dest == NULL ? 0 : strlen(*dest)) + strlen(src);
	*n = 1 + 2 * totLen;
	char * buffer = malloc(sizeof(char) * (*n));
	if(buffer == NULL)
	{
	    printf("Memory is full, string cannot be allocated.\n");
	    printf("Program will exit now.\n\n");
	    exit(1);
	}
	(*dest != NULL) ? strcpy(buffer, *dest) : strcpy(buffer, "\0");
	free(*dest);
	*dest = buffer;
    }
    strcat(*dest, src);

    return *dest;
}

char ** explode(const char * str, const char * delims, int * arrLen)
{
    int loc = 0;
    int strLen = strlen(str);
    char **result = malloc(0);
    char test = str[loc];
    *arrLen = 0;
    do
    {
        addString(&test, delims, str, &loc, arrLen, &result);
	test != '\0' ? test = str[++loc] : (0);
    }
    while(test != '\0');
    if(strLen != 0 && insideString(str[strLen - 1], delims))
    {
	test = '\0';
	addString(&test, delims, str, &loc, arrLen, &result);
    }
    
    return result;
}

void addString(char *test, const char *delims, const char *str, int *loc, int *arrLen, char ***result)
{
    int startLoc = *loc;
    char ** buffer;
    char *bufferStr;
    int i;
    int strLen;
    while(!insideString(*test, delims))
    {
	*test = str[++(*loc)];
    }
    buffer = malloc(sizeof(char*) * (*arrLen + 1));
    for(i = 0; i < *arrLen; i++)
    {
	buffer[i] = (*result)[i];
    }
    free(*result);
    *result = buffer;
    strLen = *loc - startLoc;
    bufferStr = malloc(sizeof(char) * (strLen + 1));
    memcpy(bufferStr, str+startLoc, strLen);
    bufferStr[strLen] = '\0';
    (*result)[*arrLen] = bufferStr;
    (*arrLen)++;
    
    return;
}

int insideString(char chr, const char * strng)
{
    int testInt = 0;
    char test = strng[testInt];
    int length = strlen(strng);
    while(testInt <= length)
    {
	if(test == chr)
	{
	    return 1;
	}
	else
	{
	    test = strng[++testInt];
	}
    }
    return 0;
}

char * implode(char ** strArr, int len, const char *glue)
{
    int i;
    int sum = 0;
    for(i = 0; i < len; i++)
    {
	sum += strlen(strArr[i]);
    }
    sum+= (i - 1) * (strlen(glue));
    sum++;
    char *result = malloc(sizeof(char) * sum);
    if(result == NULL)
    {
	printf("Computer has run out of memory.\n");
        printf("Program is exiting now.\n");
        exit(1);
    }
    sum = 0;
    for(i = 0; i < len - 1; i++)
    {
	memcpy(result+sum, strArr[i], strlen(strArr[i]));
        sum+= strlen(strArr[i]);
        memcpy(result+sum, glue, strlen(glue));
        sum+= strlen(glue);
    } 
    memcpy(result+sum, strArr[i], strlen(strArr[i]));
    sum+= strlen(strArr[i]);
    result[sum] = '\0';
    return result;
}

void sortStringArray(char **arrString, int len)
{
    qsort(arrString, len, sizeof(char*), (int(*)(const void*, const void *))smallerString);
    return;
}

int smallerString(const void **str1a,const void **str2a)
{
    const char *str1 = (const char*)(*str1a);
    const char *str2 = (const char*)(*str2a);
    return strcmp(str1, str2);
}

void sortStringCharacters(char *str)
{
    qsort(str,strlen(str), sizeof(char), cmpChar);
    return;
}

int cmpChar(const void *arg1, const void *arg2)
{
    const char char1 = *((const char*)arg1);
    const char char2 = *((const char*)arg2);
    return char1 - char2;
}

void destroyStringArray(char **strArr, int len)
{
    if(strArr != NULL)
    {
	int i;
        for(i = 0; i < len; i++)
	{
	    free(strArr[i]);
	}
	free(strArr);
    }
    return;
}
