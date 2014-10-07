#include <stdio.h>
#include <stdlib.h>
#include "answer05.h"

void intitiateArray(int *result, int initialNum, int size);
void partition(int *result, int length);
void printResult(int *arr, int len);
void printArray(int *arr, int len);
void initiateArray(int *result, int initialNum, int size);
void spread(int *loc, int remainder, int prev, int *length);
void partInc(int *result, int size);


void partitionAll(int value)
{
    int *result = malloc(sizeof(int) * value);
    initiateArray(result, 1, value);
    printResult(result, value);
    partition(result, value);
    free(result);
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

void printArray(int *arr, int len)
{
    int i;
    if(len > 0)
    {
	for(i = 0; i < len - 1; i++)
	{
	    printf("%d, ", arr[i]);
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
