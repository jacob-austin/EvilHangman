#ifndef VECTOR_H
#define VECTOR_H

#include "status.h"
#include "generic.h"
#include "boolean.h"
#include "my_string.h"

typedef void* VECTOR;

VECTOR vector_init_default(void);

int vector_get_size(VECTOR hVector);
int vector_get_capacity(VECTOR hVector);

Status vector_push_back(VECTOR hVector, Item* pItem);

Status vector_pop_back(VECTOR hVector);

MY_STRING* vector_at(VECTOR hVector, int index);

Status vector_set_capacity(VECTOR hVector, int capacity);

Boolean vector_empty(VECTOR hVector);

/* takes empty left vector and a valid right vector and copies right vector to left vector*/
void vector_copy(VECTOR copyTo, VECTOR copyFrom);

void vector_destroy(Item* pItem);

#endif
