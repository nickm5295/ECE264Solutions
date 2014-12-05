#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "answer11.h"

void freeSNode(StackNode * sNode);

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

Stack * Stack_create()
{
    Stack * st = malloc(sizeof(Stack));
    st->head = NULL;
    return st;
}

void Stack_destroy(Stack * stack)
{
    if(stack == NULL) return;
    freeSNode(stack->head);
    free(stack);
}

void freeSNode(StackNode * sNode)
{
    StackNode * p = NULL;
    while(sNode!= NULL)
    {
	p = sNode;
	sNode = sNode->next;
	HuffNode_destroy(p->tree);
	free(p);
    } 
}
