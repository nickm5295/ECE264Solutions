#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "answer09.h"

/* typedef struct bnode {
 * char *name;
 * char *stars;
 * char *address;
 *
 * struct bnode *left;
 * struct bnode *right;
 *} BusinessNode;
 */

#define TRUE 1
#define FALSE 0

void destroyTree(BusinessNode * root);
//BusinessNode * load_node_from_file(FILE * fp, int * uppErr);
BusinessNode * load_node_from_file(FILE * fp);
void insertLeft(BusinessNode * node, BusinessNode * root);
void insertRight(BusinessNode * node, BusinessNode * root);
char * getString(FILE * fp);

void destroy_tree(BusinessNode * root)
{
    if(root!= NULL)
    {
	destroyTree(root);
    }
}

void destroyTree(BusinessNode * root)
{
    if(root->left != NULL)
    {
	destroyTree(root->left);
    }
    if(root->right != NULL)
    {
	destroyTree(root->right);
    }
    free(root->name);
    free(root->stars);
    free(root->address);
    free(root);
}

void print_tree(BusinessNode * tree)
{
    if(tree == NULL)
    {
	printf("tree is NULL\n");
    }
    else
    {
	printf("\n----Printing %p ------\n", tree);
	print_node(tree);
	if(tree->left != NULL) print_tree(tree->left);
	if(tree->right != NULL) print_tree(tree->right);
    }
}

void print_node(BusinessNode * node)
{
    printf("address = %p\n", node);
    printf("name = \"%s\"\n", node->name);
    printf("stars = \"%s\"\n", node->stars);
    printf("address = \"%s\"\n", node->address);
    if(node->left != NULL)
    {
	printf("left = %p\n", node->left);
    }
    else
    {
	printf("left = NULL\n");
    }
    if(node->right != NULL)
    {
	printf("right = %p\n", node->right);
    }
    else
    {
	printf("right = NULL\n");
    }
}

BusinessNode * tree_search_name(char * name, BusinessNode * root)
{
    int res = strcmp(name, root->name);
    if(res < 0)
    {
	if(root->left != NULL)
	{
	    return tree_search_name(name, root->left);
	}
	else
	{
	    return NULL;
	}
    }
    else if(res > 0)
    {
	if(root->right != NULL)
	{
	    return tree_search_name(name, root->right);
	}
	else
	{
	    return NULL;
	}
    }
    return root;
}

BusinessNode * load_tree_from_file(char * filename)
{
    FILE * fp = NULL;
    //int err = FALSE;
    
    fp = fopen(filename, "rb");
    if(fp != NULL)
    {
	//BusinessNode * head = load_node_from_file(fp, &err);
	BusinessNode * head = NULL;
	BusinessNode * tail = NULL;
	int done = FALSE;
        do
	{
	    //tail = load_node_from_file(fp, &err);
	    tail = load_node_from_file(fp);
	    if(tail != NULL) 
	    {
		head = tree_insert(tail, head);
	    	//free(tail);
	    }
	    else
	    {
		done = TRUE;
	    }
	} while(!done);
	return head;
    }
    else
    {
   	return NULL;
    }
}

BusinessNode * load_node_from_file(FILE * fp)
{
    int err = FALSE;
    char * stars = NULL, * address = NULL, * name = NULL;
    
    if(!err)
    { 
	stars = getString(fp);
	if(stars == NULL)
	{
	    err = TRUE;
	}
    }

    if(!err)
    {
	name = getString(fp);
	if(name == NULL) err = TRUE;
    }

    if(!err)
    {
	address = getString(fp);
	if(address == NULL) err = TRUE;
    }

    /*if(!err)
    {
	BusinessNode * left = NULL;
	read = fread(left, sizeof(BusinessNode), 1, fp);
	if(read != 1)
	{
	    fprintf(stderr, "Failed to read left struct\n");
	    err = TRUE;
 	}
    }*/

    /*if(!err)
    {
        read = fread(left, sizeof(struct), 2, fp);
        if(read != 1)
        {
            fprintf(stderr, "Failed to read right struct\n");
            err = TRUE;
        }
    }*/

    if(!err)
    {
 	BusinessNode * node = create_node(stars, name, address);
	return node;
    }
    //*uppErr = TRUE;
    return NULL;
}

char * getString(FILE * fp)
{
    int size = 0, i, ch, done = FALSE, err = FALSE;
    char * result = malloc(sizeof(char));
    char * buffer = NULL;
    if(result == NULL)
    {
	fprintf(stderr, "Failed to allocate memory for string\n");
	return NULL;
    }
    while(done == FALSE)
    {
	ch = fgetc(fp);
        if(ch < 255 && ch != EOF && ch >= 0)
	{
	    size++;
	    buffer = malloc(sizeof(char) * size);
	    if(buffer == NULL)
	    {
		fprintf(stderr, "Failed to allocate memory for string\n");
	  	return NULL;
	    }
	    for(i = 0; i < size - 1; i++)
	    {
		buffer[i] = result[i];
	    }
	    if(ch == '\t' || ch == '\n') 
	    {
		ch = 0;
		//if(fgetc(fp) == EOF) err = TRUE;
	    }
	    buffer[i] = (char) ch;
	    free(result);
	    result = buffer;
	    if(ch == '\0')
	    {
		done = TRUE;
	    }
	}
	else
	{
	    done = TRUE;
	    err = TRUE;
	} 
    }
    if(err == FALSE)
    {
	return result;
    }
    free(result);
    return NULL;
}

BusinessNode * tree_insert(BusinessNode * node, BusinessNode * root)
{
    if(root == NULL)
    {
	return node;
    }
    int res = strcmp(node->name, root->name);
    if(res < 0)
    {
	insertLeft(node, root);
    }
    else if(res > 0)
    {
	insertRight(node, root);
    }
    else
    {
	res = strcmp(node->address, root->address);
	if(res < 0)
	{
	    insertLeft(node, root);
	}	
	else if(res > 0)
	{
	    insertRight(node, root);
	}
	else
	{
	    res = strcmp(node->stars, root->stars);
	    if(res < 0)
	    {
		insertLeft(node, root);
	    }
	    else if(res > 0)
	    {
		insertRight(node, root);
	    }
	    else
	    {
		insertRight(node, root);
	    }
	}
    }
    return root;
}

void insertRight(BusinessNode * node, BusinessNode * root)
{
    if(root->right != NULL)
    {
	tree_insert(node, root->right);
    }
    else
    {
	root->right = node;
    }
}

void insertLeft(BusinessNode * node, BusinessNode * root)
{
    if(root->left != NULL)
    {
	tree_insert(node, root->left);
    }
    else
    {
	root->left = node;
    }
}

BusinessNode * create_node(char * stars, char * name, char * address)
{
    BusinessNode * node = malloc(sizeof(BusinessNode));
    node->name = name;
    node->stars = stars;
    node->address = address;
    node->left = NULL;
    node->right = NULL;
    return node;
}

