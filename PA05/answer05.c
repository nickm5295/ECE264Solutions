#include <stdio.h>
#include <stdlib.h>
#include "answer05.h"

#define TRUE  1
#define FALSE 0

void intitiateArray(int *result, int initialNum, int size);
void partition(int *result, int length);
void printResult(int *arr, int len);
void printArray(int *arr, int len);
void initiateArray(int *result, int initialNum, int size);
void spread(int *loc, int remainder, int prev, int *length);
void partInc(int *result, int size);
void printArrayBack(int *arr, int len);
void printResultBack(int *arr, int len);
void partOdd(int *result, int remainder, int loc);
void partEven(int *result, int remainder, int loc);
void oddPart(int *result, int remainder, int loc);
void evenPart(int *result, int remainder, int loc);
int isPrime(int num);
void partPrim(int *result, int remainder, int loc);

void partitionPrime(int value)
{
    if(value > 1)
    {
        int *result = malloc(sizeof(int) * (value/ 2));
      	partPrim(result, value, 0);
    	free(result);
    }
}

int isPrime(int n)
{
    if(n <= 3)
    {
	return n > 1;
    }
    else if(n % 2 == 0 || n % 3 == 0)
    {
	return FALSE;
    }
    else
    {
	int i;
	for(i = 5; (i-1)*(i-1) < n; i += 6)
	{
	    if(n % i == 0 || n % (i + 2) == 0)
	    {
		return FALSE;
     	    }
	}
	return TRUE;
    }
}

void partitionOddAndEven(int value)
{
    int i;
    int remainder;
    int odd = 0;
    int *result = malloc(sizeof(int) * value);
    for(i = 1; i <= value; i++)
    {
	*result = i;
	remainder = value - i;
	odd = !odd;
	if(remainder != 0)
	{
	    if(odd)
	    {
		evenPart(result, remainder, 1);
	    }
	    else
	    {
		oddPart(result, remainder, 1);
	    }
	}
    }
    printf("= %d\n", *result);
    free(result);
}

void oddPart(int *result, int remainder, int loc)
{
    int i;
    int rem;
    for(i = 1; i <= remainder; i+=2)
    {
	result[loc] = i;
	rem = remainder - i;
	if(rem != 0)
	{
	    rem != 1 ? evenPart(result, rem, loc + 1): 0;
	}
	else
	{
	    printResult(result, loc + 1);
	}
    }
}

void evenPart(int *result, int remainder, int loc)
{
    int i;
    int rem;
    for(i = 2; i <= remainder; i+=2)
    {
	result[loc] = i;
	rem = remainder - i;
	if(rem != 0)
	{
	    rem != 2 ? oddPart(result, rem, loc + 1): 0;
	}
	else
	{
	    printResult(result, loc+1);
	}
    } 
}

void partitionEven(int value)
{
    int *result = malloc(sizeof(int) * value);
    partEven(result, value, 0);
    free(result);
}

void partitionAll(int value)
{
    int *result = malloc(sizeof(int) * value);
    initiateArray(result, 1, value);
    printResult(result, value);
    partition(result, value);
    free(result);
}

void partitionOdd(int value)
{
    int *result = malloc(sizeof(int) * value);
    partOdd(result, value, 0);
    result[0] = 1;
    free(result);
}

void partOdd(int *result, int remainder, int loc)
{
    int i;
    int rem;
    for(i = 1; i <= remainder; i+=2)
    {
	result[loc] = i;
	rem = remainder - i;
	rem != 0 ? partOdd(result, rem, loc + 1) : printResult(result, loc + 1);
    }
}

void partPrim(int *result, int remainder, int loc)
{
    int i;
    int rem;
    result[loc] = 2;
    rem = remainder - 2;
    if(rem > 1)
    {
	partPrim(result, rem, loc + 1);
    }
    else
    {
	result[loc] += rem;
        if(isPrime(result[loc]))
        {
            printResult(result, loc + 1);
	}
        return;
    }
    for(i = 3; i <= remainder; i+=2)
    {
	if(isPrime(i))
	{
	    result[loc] = i;
	    rem = remainder - i;
	    if(rem > 1)
	    {
		partPrim(result, rem, loc + 1);
	    }
	    else
	    {
		result[loc] += rem;
		if(isPrime(result[loc]))
	      	{
		    printResult(result, loc + 1);
		}
	    }
	}
    }
}

void partEven(int *result, int remainder, int loc)
{
    int i;
    int rem;
    for(i = 2; i <= remainder; i+=2)
    {
	result[loc] = i;
	rem = remainder - i;
	rem != 0? partEven(result, rem, loc + 1) : printResult(result, loc + 1);
    }
}

void partition(int *result, int length)
{
    int len = length - 1;
    int i;
    while(len > 0)
    {
	if(result[len] == 1)
	{
	    (result[len - 1])++;
	    result[len] = 0;
	    len--;
	}
	else
	{
	    (result[len - 1])++;
	    (result[len])--;
	    for(i = result[len]; i > 0; i--)
	    {
		result[len] = 1;
		len++;
	    }
	    len--;
	}
	printResult(result, len + 1);
    }
}

void printResult(int *arr, int len)
{
    printf("= ");
    printArray(arr, len);
}

void printResultBack(int *arr, int len)
{
    printf("= ");
    printArrayBack(arr, len);
}

void printArrayBack(int *arr, int len)
{
    int i;
    if(len > 0)
    {
	for(i = len - 1; i > 0; i--)
	{
	    printf("%d + ", arr[i]);
	}
	printf("%d", arr[i]);
    }
    printf("\n");
}

void printArray(int *arr, int len)
{
    int i;
    if(len > 0)
    {
	for(i = 0; i < len - 1; i++)
	{
	    printf("%d + ", arr[i]);
	}
	printf("%d", arr[len - 1]);
    }
    printf("\n");
}

void initiateArray(int *result, int initialNum, int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
	result[i] = initialNum;
    }
}

void partitionIncreasing(int value)
{
    int *result = malloc(sizeof(int) * (value / 2 + 2));
    partInc(result, value);
    free(result);
}

void partitionDecreasing(int size)
{
    int remainder, prev;
    int length = 0;
    int *result = malloc(sizeof(int) * (size / 2 + 2));
    spread(result, size, 0, &length);
    printResultBack(result, length);
    while(length != 1)
    {
	length--;
	prev = ++(result[length - 1]);
        remainder = result[length] - 1;
        if(remainder > prev)
        {
            spread(result+length, remainder, prev, &length);
        }
        else
        {
            (result[length - 1]) += remainder;
        }
        printResultBack(result, length);
    }
    free(result);
}

void partInc(int *result, int size)
{
    int remainder;
    int prev;
    int length = 0;
    spread(result, size, 0, &length);
    printResult(result, length);
    while(length!=1)
    {
	length--;
	prev = ++(result[length - 1]);
	remainder = result[length] - 1;
	if(remainder > prev)
	{
	    spread(result+length, remainder, prev, &length);
	}
	else
	{
	    (result[length - 1]) += remainder;
	}
    	printResult(result, length);
    }
}

void spread(int *loc, int remainder, int prev, int *length)
{
    (*length)++;
    if((remainder - 1) / 2 > prev)
    {
	int new = prev + 1;
	*loc = new;
	spread(loc+1, remainder - new, new, length);
    }
    else
    {
	*loc = remainder;
    }
}
