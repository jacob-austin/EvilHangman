#include <stdlib.h>
#include <stdio.h>
#include "vector.h"
#include "status.h"
#include "generic.h"
#include "boolean.h"
#include "my_string.h"

struct vector
{
    int size;
    int capacity;
    MY_STRING* word;
};

typedef struct vector vector;


VECTOR vector_init_default(void)
{
    vector *pVector;
    int i;
    pVector = (vector*)malloc(sizeof(vector));
    if (pVector != NULL)
    {
	
	pVector->size = 0;
	pVector->capacity = 1000;
	
	pVector->word = (MY_STRING*)malloc(sizeof(MY_STRING) * pVector->capacity);
	
	if (pVector->word == NULL)
	{
	    free(pVector);
	    return NULL;
	}
	for(i = 0; i < pVector->capacity; i++) //initialize words to NULL for later NULL checking
	{
	    pVector->word[i] = NULL;
	}
    }
    
    return (VECTOR) pVector;
}

int vector_get_size(VECTOR hVector)
{
    vector* pVector = (vector*) hVector;
    return pVector->size;
}

int vector_get_capacity(VECTOR hVector)
{
    vector* pVector = (vector*) hVector;
    return pVector->capacity;
}

Status vector_push_back(VECTOR hVector, Item* pItem)
{
    vector *pVector = (vector*) hVector;

    if (pVector->size >= pVector->capacity)
    {
	vector_set_capacity(hVector, (pVector->capacity * 2));
    }
    
    my_string_assignment((Item*) &pVector->word[pVector->size], (Item*) pItem);
    pVector->size++;

    return SUCCESS;
}

Status vector_pop_back(VECTOR hVector)
{
    vector *pVector = (vector*) hVector;

    if (pVector->size <=0)
    {
	return FAILURE;
    }
    
    my_string_destroy((Item*) &pVector->word[pVector->size - 1]);
    pVector->size--;
    
    return SUCCESS;
}

MY_STRING* vector_at(VECTOR hVector, int index)
{
   vector *pVector = (vector*) hVector;
   MY_STRING *pTemp;
   
   if (index < 0 || index >= pVector->size)
   {
     return NULL;
   }

   pTemp = pVector->word;
   
   return pTemp[index];
}

Status vector_set_capacity(VECTOR hVector, int capacity)
{
    vector *pVector = (vector*) hVector;
    MY_STRING *pTemp;
    int i;

    if (capacity < pVector->capacity)
    {
	return SUCCESS;
    }

    pTemp = (MY_STRING*)realloc(pVector->word, (sizeof(MY_STRING) * capacity));
    if(!pTemp)
    {
	fprintf(stderr, "Error allocating memory in vector_set_capacity\n");
	return FAILURE;
    }
    pVector->word = pTemp;
    pVector->capacity = capacity;
    for(i = pVector->size; i < pVector->capacity; i++) //initialize new capacity spaces to NULL
    {
	pVector->word[i] = NULL;
    }

    return SUCCESS;
}

Boolean vector_empty(VECTOR hVector)
{
    vector *pVector = (vector*) hVector;

    if (pVector->size)
    {
	return FALSE;
    }

    return TRUE;
}

void vector_copy(VECTOR copyTo, VECTOR copyFrom)
{
    vector* pTo = (vector*) copyTo;
    vector* pFrom = (vector*) copyFrom;
    int i;
    
    if(pTo->size)
    {
	fprintf(stderr, "passed non-empty vector to vector_copy\n");
	return;
    }

    for(i = 0; i < pFrom->size; i++)
    {
	vector_push_back(pTo, (Item*) pFrom->word[i]);
    }
}

void vector_destroy(Item* pItem)
{
    Item * ppItem = (Item *) *pItem;
    vector* pVector = (vector*) ppItem;

    while(pVector->size) //free all My_string objects in vector first
    {
	vector_pop_back(pVector);
    }
    free(pVector->word);
    free(pVector);
    *pItem = NULL;
}
