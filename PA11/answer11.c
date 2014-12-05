#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HuffNode * HuffNode_create(int value)
{
    HuffNode * huff = malloc(sizeof(HuffNode));
    huff->value = value;
    huff->left = NULL;
    huff->right = NULL;
    return huff;
}
