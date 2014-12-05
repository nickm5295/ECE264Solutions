#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "answer11.h"

HuffNode * HuffNode_create(int value)
{
    HuffNode * huff = malloc(sizeof(HuffNode));
    huff->value = value;
    huff->left = NULL;
    huff->right = NULL;
    return huff;
}

void HuffNode_destroy(HuffNode * tree)
{
    if(tree == NULL) return;
    HuffNode_destroy(tree->left);
    HuffNode_destroy(tree->right);
    free(tree);
}
