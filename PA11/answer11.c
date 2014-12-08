#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "answer11.h"

#define TRUE 1
#define FALSE 0

void freeSNode(StackNode * sNode);
int getBit(int input, int * readLoc);
int buildInput(int input, int * readLoc, int res, int writeLoc);

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

int Stack_isEmpty(Stack * stack)
{
    if(stack != NULL && stack->head != NULL && (stack->head)->tree != NULL)
    {
	return FALSE;
    }
    return TRUE;
}

HuffNode * Stack_popFront(Stack * stack)
{
    StackNode * sNode = stack->head;
    HuffNode * res = sNode->tree;
    stack->head = (stack->head)->next;
    free(sNode);
    return res;
}

void Stack_pushFront(Stack * stack, HuffNode * tree)
{
    StackNode * newStack = malloc(sizeof(StackNode));
    newStack->tree = tree;
    newStack->next = stack->head;
    stack->head = newStack;
}

void Stack_popPopCombinePush(Stack * stack)
{
    HuffNode * top = HuffNode_create(0), * right = Stack_popFront(stack);
    HuffNode * left = Stack_popFront(stack);
    top->left = left;
    top->right = right;
    Stack_pushFront(stack, top);
}

HuffNode * HuffTree_readTextHeader(FILE * fp)
{
    int input, done = FALSE;
    Stack * st = Stack_create();
    HuffNode * huffN = NULL;
    long int fileLoc;
    do
    {
	fileLoc = ftell(fp);
	input = fgetc(fp);
	if(input == '1')
	{
	    fileLoc = ftell(fp);
	    input = fgetc(fp);
	    if(input != EOF)
	    {
		huffN = HuffNode_create(input);
		Stack_pushFront(st, huffN);
	    }
	    else done = TRUE;
	}
	else if(input == '0')
	{
	    if((st->head)->next != NULL)
	    {
		Stack_popPopCombinePush(st);
	    }
	    else
	    {
		done = TRUE;
	    }
	}
	else done = TRUE;
    } while(done == FALSE);
    fseek(fp, fileLoc, SEEK_SET);
    if(st->head == NULL || (st->head)->next != NULL)
    {
	Stack_destroy(st);
	return NULL;
    }
    huffN = (st->head)->tree;
    (st->head)->tree = NULL;
    Stack_destroy(st);
    return huffN;
}
/* What we want to do is to check bit by bit following the commands requested on the file. First, the first bit must be read. If it is one, then the process 
 * for getting the char must be followed. Once this is done, the following command must be read and so on.
 *
 * The process for getting the char must be to get bit by bit and assigning it where it should go. To get the bit, a location must be known. Depending
 * on the location, a comparison to another char must be done and if true then a 1 or 0 must be assigned at the location of the char where it must
 * be added. 
 *
 * Once the char completion is done, then the same logic for the creation of the tree is done as in text.
 */
HuffNode * HuffTree_readBinaryHeader(FILE * fp)
{
    int input, i, builtInput, command, done = FALSE, readLoc = 0;
    Stack * st = Stack_create();
    HuffNode * huffN = NULL;
    long int fileLoc;
    do
    {
        fileLoc = ftell(fp);
	if(readLoc == 0)
	{
	    input = fgetc(fp);
	}
	command = getBit(input, &readLoc);
        if(command == 1)
        {
	    fileLoc = ftell(fp);
	    if(readLoc != 0)
	    {
            	//if(read2 == TRUE) input2 = fgetc(fp);
	        /*if(input2 != EOF)
            	{
		    builtInput = buildInput(input1, input2, &readLoc);
		    read2 = TRUE;
                    huffN = HuffNode_create(builtInput);
                    Stack_pushFront(st, huffN);
            	}
            	else done = TRUE;*/
		builtInput = 0;
		for(i = 0; i < 8; i++)
		{
		    if(readLoc == 0) input = fgetc(fp);
		    builtInput = buildInput(input, &readLoc, builtInput, i);
		}
		huffN = HuffNode_create(builtInput);
		Stack_pushFront(st, huffN);
	    }
	    else
	    {
		input = getc(fp);
		if(input != EOF)
		{
		    huffN = HuffNode_create(input);
                    Stack_pushFront(st, huffN);
		}
		else done = TRUE;
	    }
        }
        else if(command == 0)
        {
            if((st->head)->next != NULL)
            {
                Stack_popPopCombinePush(st);
            }
            else done = TRUE;
        }
        else done = TRUE;
	//oldCommand = command;
    } while(done == FALSE);
    fseek(fp, fileLoc, SEEK_SET);
    if(st->head == NULL || (st->head)->next != NULL)
    {
	Stack_destroy(st);
        return NULL;
    }
    huffN = (st->head)->tree;
    (st->head)->tree = NULL;
    Stack_destroy(st);
    return huffN;
}
/*
int buildInput(int input1, int input2, int * readLoc)
{
    int i, first = 1, bit, res = 0;
    //unsigned int temp = 0;
    for(i = 7; i >= 0; i--)
    {
	if(first == 1)
	{
	    bit = getBit(input1, readLoc);
	}
	else bit = getBit(input2, readLoc);
	res = (res | (bit << i));
	if(*readLoc == 0) first = 0;
    }
    return res;
}*/

int buildInput(int input, int * readLoc, int res, int writeLoc)
{
    int bit = getBit(input, readLoc);
    res = (res | (bit << (7 - writeLoc)));
    return res;
}

int getBit(int input, int * readLoc)
{
    int comp = 1, i, res, limit = 7 - *readLoc;
    for(i = 0; i < limit; i++) comp*=2;
    res = (input & comp) == comp;
    *readLoc = (*readLoc + 1) % 8;
    return res;
}
