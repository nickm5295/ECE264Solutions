#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if(argc >= 2 && !strcmp("--help", argv[1]))
    {
	printf("Usage: cat-lite [--help] [FILE]...\n");
	printf("With no FILE, or when FILE is -, read standard input.\n\n");
	printf("Examples:\n");
	printf("  cat-lite README\tPrint the file README to standard output.\n");
	printf("  cat-lite f - g\tPrint f's contents, then standard input,\n");
	printf("\t\t\tthen g's contents.\n");
	printf("  cat-lite\t\tCopy standard input to standard output.\n");
	return EXIT_SUCCESS;
    }
    return EXIT_SUCCESS;
}
