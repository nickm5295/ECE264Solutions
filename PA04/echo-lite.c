#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
    int i;
    for(i = 1; i < argc; i++)
    {
	//if(!strstr(argv[i], "\""))
	//{
	    printf("%s ", argv[i]);
	/*}
	else
	{

	}*/
    }    
    printf("\n");

    return EXIT_SUCCESS;
}
