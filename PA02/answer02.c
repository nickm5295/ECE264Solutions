#include <stdio.h>
#include "answer02.h"



size_t my_strlen(const char * str)
{
    //LOCAL DECLARATIONS
    int counter = 0;

    //EXECUTABLE STATEMENTS
    while(str[counter] != '\0')
    {
	counter++;
    }
    
    return counter;    
}

int my_countchar(const char * str, char ch)
{
    //LOCAL DECLARATIONS
    int length = my_strlen(str);
    int counter;
    int count = 0;

    //EXECUTABLE STATEMENTS
    for(counter = 0; counter < length; counter++)
    {
	if(str[counter] == ch)
	{
	    count++;
	}
    }
    
    return count;
}

char * my_strchr(const char * str, int ch)
{
    //LOCAL DECLARATIONS
    char *result = NULL;
    int end = 0;
    int counter = 0;    

    //EXECUTABLE STATEMENTS
    while(!end)
    {
	if(str[counter] == ch)
	{
	    result = (char *) &str[counter];
	    end = 1;
	}
        if(str[counter] == '\0')
        {
	    end = 1;
	}
	counter++;
    }

    return result;
}

char * my_strrchr(const char * str, int ch)
{
    //LOCAL DECLARATIONS
    int length = my_strlen(str); //the length of the string (also used as the counter for the location being viewed
    char *result = NULL;
    int end = 0;

    //EXECUTABLE STATEMENTS
    while(!end && length >= 0)
    {
	if(str[length] == ch)
	{
	    result = (char *) &str[length];
	    end = 1;
	}
	length--;
    }

    return result;
}

char * my_strstr(const char * haystack, const char * needle)
{
    /*
     *There has to be a resulting value address
* however, there must be a complete comparison cycle for the function to know its true
*
    */
    //LOCAL DECLARATIONS
    int needleLength = my_strlen(needle);
    char *result = NULL;
    int haystackCnt = 0;
    int needleCnt = 0;
  
    //EXECUTABLE STATEMENTS
    if(needle[0] == '\0')
    {
	result = (char *) &haystack[0];
    }
    else
    {
        while(needleCnt < needleLength && haystack[haystackCnt] != '\0')
        {
	    if(haystack[haystackCnt] == needle[needleCnt])
	    {
	        needleCnt++;
	    }
	    else
	    {
	        needleCnt = 0;
	    }
	    haystackCnt++;
        }
        if(needleCnt == needleLength)
        {
	    result = (char *) &haystack[haystackCnt - needleCnt];
        }
    }

    return result;
}

char * my_strcpy(char * dest, const char * src)
{
    //LOCAL DECLARATIONS    
    int srcLoc = 0;

    //EXECUTABLE STATEMENTS
    while(src[srcLoc] != '\0')
    {
	dest[srcLoc] = src[srcLoc];
	srcLoc++;
    }
    dest[srcLoc] = '\0';
    return dest;
}

char * my_strcat(char * dest, const char * src)
{
    //LOCAL DECLARATIONS
    int destLen = my_strlen(dest); //used to find the first place where the string should be copied to and also to keep the count    
    int srcLoc = 0; //the location within src that is being copied

    //EXECUTABLE STATEMENTS
    while(src[srcLoc] != '\0')
    {
	dest[destLen] = src[srcLoc++];
	destLen++;
    }
    dest[destLen] = '\0';
    
    return dest;
}

int my_isspace(int ch)
{
    //LOCAL DECLARATIONS
    int result = 0;    

    //EXECUTABLE STATEMENTS
    switch(ch)
    {
	case 9 ... 13:	
	case 32:	result = 1; break;
    }

    return result;
}

int my_atoi(const char * str)
{
   //LOCAL DECLARATIONS
   int neg = 0;
   int count = 0;
   int end = 0;   
   int numRes = 0;

   //EXECUTABLE STATEMENTS
   while(my_isspace(str[count]))
   {
	count++;
   }
   while(!end)
   {
	switch(str[count])
	{
	    case '-':		neg = !neg;				break;
	    case 48 ... 57: 	numRes = numRes * 10 + str[count] - 48;	break;
	    default: end = 1;
	}
	count++;
   }
   if(neg)
   {
	numRes *= -1;
   }
   
   return numRes;
}
