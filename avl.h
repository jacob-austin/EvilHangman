/** Associative Array w/ AVL **/
#ifndef AVL_H
#define AVL_H

#include <stdlib.h>
#include "vector.h"
#include "my_string.h"
#include "status.h"

typedef void* AVL;

typedef struct node NODE;

struct node
{
    VECTOR *word;
    MY_STRING *key;
    int id;
    int height;
    NODE *left;
    NODE *right;
};

AVL avl_init_default(void);

void avl_insert(AVL hAvl, VECTOR word, MY_STRING key);

void avl_delete(AVL hAvl, MY_STRING key);

NODE *avl_find(AVL hAvl, MY_STRING key);

NODE* avl_max(AVL hAvl);

MY_STRING avl_max_key(AVL hAvl);

void avl_copy_max_vector(AVL hAvl, VECTOR hVector);

int avl_max_size(AVL hAvl);

NODE *avl_root(AVL hAvl);

void avl_inorder(AVL hAvl, void f(NODE *));

void avl_postorder(AVL hAvl, void f(NODE *));

void avl_print(AVL hAvl);

void avl_destroy(AVL* phAvl);

#endif
