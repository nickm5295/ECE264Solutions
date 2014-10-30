#include <stdio.h>
#include <stdlib.h>
#include "answer08.h"
#include <string.h>

List * List_createNode(const char *str)
{
    List * lst = NULL;
    lst = malloc(sizeof(List));
    if(lst == NULL)
    {
	fprintf(stderr, "Failed to allocate memory for List\n");
    }
    else
    {
	lst->str = malloc(sizeof(char) * (strlen(str) + 1));
	if(lst -> str != NULL)
	{
	    sprintf(lst -> str, "%s", str);
	    lst -> next = NULL;
	}
	else
	{
	    fprintf(stderr, "Failed to allocate memory for list->str\n");
	}
    }
    return lst;
}

void List_destroy(List * list)
{
    if(list != NULL)
    {
	if(list->str != NULL) free(list->str);
	List_destroy(list->next);
	free(list);
    }
}

int List_length(List * list)
{
    int result = 0;
    if(list != NULL) result = 1 + List_length(list->next);
    return result; 
}

List * List_merge(List * lhs, List * rhs, int (*compar)(const char *, const char*))
{
    int order;
    List *head = NULL;
    if(lhs != NULL && rhs != NULL)
    {
	order = compar(lhs->str, rhs->str);
	if(order > 0)
	{
	    head = rhs;
	    rhs = rhs->next;
	}
	else
	{
	    head = lhs;
	    lhs = lhs->next;
	}
	List *end = head;
    	while(lhs != NULL && rhs != NULL)
    	{
	    order = compar(lhs->str, rhs->str);
	    if(order > 0)
	    {
	     	end->next = rhs;
		end = rhs;
		rhs = rhs->next;
	    }
	    else
	    {
		end->next = lhs;
		end = lhs;
		lhs = lhs->next;
	    }
    	}
	if(lhs != NULL)
	{
	    end->next = lhs;
	}
	else if(rhs != NULL)
	{
	    end->next = rhs;
	}
    }
    else if(lhs != NULL)
    {
	head = lhs;
    }
    else if(rhs != NULL)
    {
	head = rhs;
    }
    else
    {
	return NULL;
    }
    return head;
}

List * List_sort(List * list, int (*compar)(const char *, const char*))
{
    int i, len = List_length(list);
    if(len > 1)
    {
	List *lhs = list, *rhs = list, *prev = list;;
	for(i = 0; i < len / 2; i++)
	{
	    prev = rhs;
	    rhs = rhs->next;
	}
	prev->next = NULL;
	lhs = List_sort(lhs, compar);
	rhs = List_sort(rhs, compar);
	list = List_merge(lhs, rhs, compar);
    }

    return list;
}
