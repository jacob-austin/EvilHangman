//my_string.c implementation file
#include <stdio.h>
#include <stdlib.h>
#include "my_string.h"
#include "status.h"
#include "boolean.h"
#include "generic.h"

struct My_string
{
  int size;
  int capacity;
  char* data;
};

typedef struct My_string My_string;

void my_string_assignment(Item* pLeft, Item Right)
{
  My_string * plString;
  My_string * prString = (My_string*) Right;
  int i;
  if(NULL == *pLeft)
  {
    *pLeft = my_string_init_default();
    plString = (My_string*) *pLeft;
    if(NULL == plString)
    {
      printf("Error initializing MY_STRING object in my_string_assignment\n");
      //my_string_init_default will free plString on alloc failure
      return;
    }
  }
  else
  {
    plString = (My_string*) *pLeft;
  }
  
  if(plString->capacity < prString->capacity)
  {
    if(!my_string_set_capacity(plString, prString->capacity))
    {
      printf("Error changing capacity in my_string_assignment\n");
      free(plString);
      return;
    }
  }
  for(i = 0; i < prString->size; i++)
  {
    plString->data[i] = prString->data[i];
    if (plString->size < prString->size) //this is a bad workaround, works for this program but not for all cases
    {
	plString->size++;
    }
  }
}

MY_STRING my_string_init_default(void)
{
  My_string *pString;
  
  pString = (My_string *) malloc ( sizeof(My_string));
  if (pString)
  {
    pString->size = 0;
    pString->capacity = 7;
    pString->data = (char *) malloc (sizeof(char) * pString->capacity);
    if (NULL == pString->data)
    {
      free(pString);
      return NULL;
    }
  }
  
  return (MY_STRING) pString;

}

MY_STRING my_string_init_c_string(const char* c_string)
{
  My_string * pString;
  int length = string_length(c_string);
  int i;
  
  pString = (My_string *) malloc (sizeof(My_string));
  if (pString)
  {
    pString->size = 0;
    pString->capacity = length+1;
    pString->data = (char *) malloc (sizeof(char) * pString->capacity);
    if (NULL == pString->data)
    {
      free(pString);
      return NULL;
    }
  }
  for (i = 0; i < length; i++)
  {
    pString->data[i] = c_string[i];
    pString->size++;
  }
  
  return (MY_STRING) pString;
}

int string_length(const char* c_string)
{
  int count = 0;
  while (*c_string != '\0')
  {
    count++;
    c_string++;
  }
  return count;
}

int my_string_get_capacity(MY_STRING hMy_string)
{
  My_string * pString = (My_string *) hMy_string;

  return pString->capacity;
}

int my_string_get_size(MY_STRING hMy_string)
{
  My_string * pString = (My_string *) hMy_string;
  
  return pString->size;
}

Status my_string_set_capacity(MY_STRING hMy_string, int capacity)
{
  char* psTemp;
  My_string * pString = (My_string *) hMy_string;

  if (capacity <= pString->capacity)
  {
    return SUCCESS;
  }
  
  psTemp = (char *) realloc (pString->data, (sizeof(char) * capacity));
  if (!psTemp)               //Check to ensure realloc worked
  {
    fprintf(stderr, "Error reallocating data in my_string_set_capacity\n");
    return FAILURE;
  }

  pString->data = psTemp;
  pString->capacity = capacity;
  return SUCCESS;
}

Status my_string_extraction(MY_STRING hMy_string, FILE* fp)
{
  My_string * pString = (My_string *) hMy_string;

  char  cTemp;
  int i = 0;
  
  cTemp = fgetc(fp);                  //start by reading a character from file

  if (EOF == cTemp)                   //check for end of file
  {
    ungetc(cTemp,fp);
    return FAILURE;
  }

  while (' ' == cTemp || '\n' == cTemp)        //skip leading white space
  {
    cTemp = fgetc(fp);
  }

  for(i = 0; i < pString->size; i++)  //empty string
  {
    pString->data[i] = ' ';
  }
  pString->size = 0;                  //set size of now-empty string to 0

  for (i = 0; cTemp != ' ' && cTemp != EOF && cTemp != '\n'; i++)
  {
    if (i == pString->capacity)  //double capacity of pString if full
    {
      my_string_set_capacity(hMy_string, 2*pString->capacity); 
    }
    pString->data[i] = cTemp;    //Add character to pString
    pString->size++;             //increment size
    cTemp = fgetc(fp);           //get next character
  }
  
  ungetc(cTemp, fp);             //return pointer to character before whitespace
  return SUCCESS;
}

Status my_string_insertion(MY_STRING hMy_string, FILE* fp)
{
  My_string * pString = (My_string *) hMy_string;

  int i;

  if(NULL == fp)
  {
    return FAILURE;
  }
  
  for(i = 0; i < pString->size; i++)
  {
     fputc(pString->data[i], fp);
  }
  
  return SUCCESS; 
}

int my_string_compare(MY_STRING hLeft_string, MY_STRING hRight_string)
{
  My_string *  pLeft = (My_string *) hLeft_string;
  My_string *  pRight = (My_string *) hRight_string;
  int i = 0;
  int maxsize;

  maxsize = pLeft->size > pRight->size ? pLeft->size : pRight->size;

  for (i = 0; i < maxsize; i++)
  {
    if(i >= pLeft->size)
    {
      return -1;
    }
    
    if (i >= pRight->size)
    {
      return 1;
    }
    
    if (pRight->data[i] > pLeft->data[i])
    {
      return 1;
    }
    else if (pRight->data[i] < pLeft->data[i])
    {
      return -1;
    }
  }
  
  return 0;
}

Status my_string_push_back(MY_STRING hMy_string, char item)
{
   My_string * pString = (My_string *) hMy_string;

   if(pString->size >= pString->capacity)
   {
     if(SUCCESS !=  my_string_set_capacity(hMy_string, 2*pString->capacity))
     {
       return FAILURE;
     }
   }
   pString->data[pString->size] = item;
   pString->size++;
   return SUCCESS;
}

Status my_string_pop_back(MY_STRING hMy_string)
{
   My_string * pString = (My_string *) hMy_string;
   
   if (pString->size <= 0)
   {
     return FAILURE;
   }
   pString->data[pString->size-1] = '\0';
   pString->size--;

   return SUCCESS;
}

Status my_string_concat(MY_STRING hResult, MY_STRING hAppend)
{
  My_string * pResult = (My_string *) hResult;
  My_string * pAppend = (My_string *) hAppend;
  int concatCapacity;
  int i;

  
  concatCapacity = pResult->capacity + pAppend->capacity;
  
  if(SUCCESS != my_string_set_capacity(pResult, concatCapacity))
  {
    return FAILURE;
  }

  for (i = 0; i <= (pAppend->size - 1); i++)
  {
    pResult->data[pResult->size] = pAppend->data[i];
    pResult->size++;
  }
  
  return SUCCESS;
}

Boolean my_string_empty(MY_STRING hMy_string)
{
  My_string * pString = (My_string *) hMy_string;

  if (0 == pString->size)
  {
    return TRUE;
  }
  
  return FALSE;
}

char* my_string_at(MY_STRING hMy_string, int index)
{
   My_string * pString = (My_string *) hMy_string;
   if (index < 0 || index >= pString->size)
   {
     return NULL;
   }
   return pString->data + index;
}

char* my_string_c_str(MY_STRING hMy_string)
{
  My_string * pString = (My_string *) hMy_string;

  if ((pString->size) >= pString->capacity) //changed from size+1
  {
    if(SUCCESS !=  my_string_set_capacity(hMy_string, 1+pString->capacity))
     {
       return NULL;
     }
  }
  pString->data[pString->size] = '\0';

  return pString->data;
}

Status get_word_key_value(MY_STRING current_word_family, MY_STRING new_key, MY_STRING word, char guess)
{
    My_string * pNew = (My_string*) new_key;
    My_string * pCurr = (My_string*) current_word_family;
    My_string * pWord = (My_string*) word;
    int i;
    char* curr;
    char* letter;

    if (pNew->capacity < pCurr->capacity)
    {
	if(!my_string_set_capacity(pNew, pCurr->capacity))
	{
	    fprintf(stderr, "Error assigning new_key in get_word_key_value");
	    return FAILURE;
	}
    }
    
    if(pNew->size < pCurr->size)
    {
	for(i = pNew->size; i < pCurr->size; i++)
	{
	    pNew->data[pNew->size] = '-';
	    pNew->size++;
	}
    }
    
    for(i = 0; i < pNew->size; i++)
    {
	curr = my_string_at(pCurr, i);
	letter = my_string_at(pWord, i);\
	if (*curr == '-')
	{
	    if (*letter == guess)
	    {
		pNew->data[i] = guess;
	    }
	}
	else
	{
	    pNew->data[i] = pCurr->data[i];
	}
    }
    return SUCCESS;
}

void my_string_clear_key(MY_STRING key)
{
    My_string *pKey = (My_string*) key;
    int i;
    
    for(i = 0; i < pKey->size; i++)
    {
	pKey->data[i] = '-';
    }
}

//use to check key for hyphen (for checking whether user has won)
int key_has_hyphen(MY_STRING key)
{
    My_string *pKey = (My_string*) key;
    int i;
    int hyphen;

    hyphen = 0;
    
    for(i = 0; i < pKey->size; i++)
    {
	if(pKey->data[i] == '-')
	{
	    hyphen = 1;
	}
    }

    return hyphen;
}


void my_string_destroy(Item* pItem)
{
  Item * ppItem = (Item *) *pItem;
  My_string* pString = (My_string*) ppItem;

  free(pString->data);
  free(pString);
  *pItem = NULL;
}
