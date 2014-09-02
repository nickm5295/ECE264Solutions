#include <stdlib.h>
#include "answer01.h"

int arraySum(int * array, int len)
{
    //declarations
    int sum = 0;
    int count;
    
    //executable statements
    for(count = 0; count < len; count++)
    {
	sum += array[count];
    }
    return sum;
}

int arrayCountNegative(int * array, int len)
{
    //declarations
    int negCnt = 0;
    int counter;

    //executable statements
    for(counter = 0; counter < len; counter++)
    {
	if(array[counter] < 0)
	{
	     negCnt++;
	}
    }
    return negCnt;
}

int arrayIsIncreasing(int * array, int len)
{
    //LOCAL DECLARATIONS
    int res = 1; //the result returned to user
    int counter; //used for the for loop
    int previousNum; //number being compared to as the previosu number in the array

    //EXECUTABLE STATEMENTS
    if(len > 0)//checks that array is long enough to use arrray[0]
    {
	previousNum = array[0];
    }
    for(counter = 1; counter < len; counter++)
    {
	if(previousNum > array[counter])
	{
	    res = 0; //result gets permanently changed to 0 if the number is not increasing or staying the same
	}
	previousNum = array[counter];
    }    
    return res;
}

int arrayIndexRFind(int needle, const int * haystack, int len)
{
    //LOCAL DECLARATIONS
    int result = len - 1;

    //EXECUTABLE STATEMENTS
    while(result >= 0 && needle != haystack[result])
    {
	result--;
    }
    return result;
}

int arrayFindSmallest(int * array, int len)
{
    //LOCAL DECLARATIONS
    int result = 0;   //the index of the smallest integr
    int counter;  //the counter used for the for loop
    int smallest; //the smallest int 

    //EXECUTABLE STATEMENTS
    len > 0 ? smallest = array[0] : (0);
    for(counter = 1; counter < len; counter++)
    {
	if(array[counter] < smallest)
	{
	    smallest = array[counter];
	    result = counter;
	}
    }
    return result;
}   
