#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "avl.h"
#include "vector.h"

struct avl
{
    NODE *root;
    int id;
};

typedef struct avl avl;

static int getHeight(NODE *pNode)
{
    if(!pNode)
    {
	return 0;
    }
    return pNode->height;
}

static int getBalance(NODE *pNode)
{
    if (!pNode)
    {
	return 0;
    }
    return getHeight(pNode->left) - getHeight(pNode->right);
}

static int max(int a, int b)
{
    if(a > b)
    {
	return a;
    }
    return b;
}

NODE *rightRotation(NODE* pNodey)
{
    NODE *pNodex = pNodey->left;
    NODE *T2 = pNodex->right;

    pNodex->right = pNodey;
    pNodey->left = T2;

    pNodey->height = max(getHeight(pNodey->left), getHeight(pNodey->right))+1;
    pNodex->height = max(getHeight(pNodex->left), getHeight(pNodex->right))+1;

    return pNodex;
}

NODE* leftRotation(NODE* pNodex)
{
    NODE * pNodey = pNodex->right;
    NODE *T2 = pNodey->left;

    pNodey->left = pNodex;
    pNodex->right = T2;

    pNodex->height = max(getHeight(pNodex->left), getHeight(pNodex->right))+1;
    pNodey->height = max(getHeight(pNodey->left), getHeight(pNodey->right))+1;

    return pNodey;
}

static void inorder (NODE *pNode, void f(NODE*))
{
    if (!pNode)
	return;

    inorder(pNode->left, f);
    f(pNode);
    inorder(pNode->right, f);
}

static void postorder(NODE *pNode, void f(NODE *))
{
    if(!pNode)
    {
	return;
    }
    postorder(pNode->left, f);
    postorder(pNode->right, f);
    f(pNode);
}

static NODE *new_node(avl *pAvl, MY_STRING key)
{
    MY_STRING *pString;
    NODE *pn = (NODE*)malloc(sizeof(NODE));
    if(pn != NULL)
    {
	pn->word = vector_init_default();
	pString = my_string_init_default();
	my_string_assignment((Item*) &pString, (Item*) key);
	pn->key = pString;
	pn->id = ++(pAvl->id);
	pn->height = 1;
	pn->left = NULL;
	pn->right = NULL;
    }
    return pn;
}

static void free_node(NODE *pNode)
{
    my_string_destroy((Item*) &pNode->key);
    vector_destroy((Item*) &pNode->word);
    free(pNode);
}

static void max_vector(NODE *pNode, NODE **pMax, NODE* root, int* size)
{
    if(!pNode)
	return;

    if(vector_get_size(pNode->word) > *size)
    {
	*pMax = pNode;
	*size = vector_get_size(pNode->word);
    }

    max_vector(pNode->left, pMax, root, size);
    max_vector(pNode->right, pMax, root, size);
}


NODE* avl_max(AVL hAvl)
{
    avl* pAvl = (avl*) hAvl;
    int* max_size = (int*)malloc(sizeof(int));
    NODE *pNode = pAvl->root;
    NODE *pMax = NULL;
    NODE **pTemp = (NODE**)malloc(sizeof(NODE**));
    if (!pTemp)
    {
        fprintf(stderr, "error allocating NODE** in avl_max_vector\n");
	return NULL;
    }
    if(!max_size)
    {
	fprintf(stderr, "error allocating int in avl_max_vector\n");
	return NULL;
    }
    *max_size = 0;

    
    max_vector(pNode, pTemp, pAvl->root, max_size);
    pMax = *pTemp;
    free(max_size);
    free(pTemp);
    return pMax;
}

MY_STRING avl_max_key(AVL hAvl)
{
    avl* pAvl = (avl*) hAvl;
    NODE* pMax;
    pMax = avl_max(pAvl);
    return pMax->key;
}

int avl_max_size(AVL hAvl)
{
    avl* pAvl = (avl*) hAvl;
    NODE* pMax;
    int max;
    pMax = avl_max(pAvl);
    max = vector_get_size(pMax->word);
    return max;
}

void avl_copy_max_vector(AVL hAvl, VECTOR hVector)
{
    avl* pAvl = (avl*) hAvl;
    NODE* pMax;
    pMax = avl_max(pAvl);
    vector_copy(hVector, pMax->word);
}

AVL avl_init_default(void)
{
    avl *pAvl;
    pAvl = (avl*)malloc(sizeof(avl));

    if(pAvl == NULL)
    {
	fprintf(stderr, "Error allocating avl in avl_init_default\n");
	return NULL;
    }

    pAvl->root = NULL;
    pAvl->id = 0;

    return pAvl;
}

NODE *avl_find(AVL hAvl, MY_STRING key)
{
    avl *pAvl = (avl*)hAvl;
    NODE *pNode = pAvl->root;
    MY_STRING *pKey = (MY_STRING*)key;
    
    while(pNode)
    {
	if(my_string_compare(pKey, pNode->key) < 0)
	{
	    pNode = pNode->left;
	}
	else if(my_string_compare(pKey, pNode->key) > 0)
	{
	    pNode = pNode->right;
	}
	else
	{
	    return pNode;
	}
    }
    return NULL;
}


static NODE *avl_ins(avl *pAvl, NODE *pNode, MY_STRING key, MY_STRING cur_word)
{
    int balance;

    if (pNode == NULL)
    {
	NODE *pNew;
	pNew = new_node(pAvl, key);
	if(pNew == NULL)
	{
	    fprintf(stderr, "avl_ins failed to allocate new node\n");
	    exit(1);
	}
	vector_push_back(pNew->word, (Item*) cur_word);
	return pNew;
    }

    if (my_string_compare(key, pNode->key) < 0)
    {
	pNode->left = avl_ins(pAvl, pNode->left, key, cur_word);
    }
    else if (my_string_compare(key, pNode->key) > 0)
    {
	pNode->right = avl_ins(pAvl, pNode->right, key, cur_word);
    }
    else
    {
	vector_push_back(pNode->word, (Item*) cur_word);
	return pNode;
    }

    pNode->height = 1 + max(getHeight(pNode->left), getHeight(pNode->right));
    balance = getBalance(pNode);

    //if node is unbalanced...

    //left/left cas
    if(balance > 1 && (my_string_compare(key, pNode->left->key) < 0))
	return rightRotation(pNode);

    //right/right case
    if (balance <-1 && (my_string_compare(key, pNode->right->key) > 0))
	return leftRotation(pNode);

    //left/right case
    if (balance > 1 && (my_string_compare(key, pNode->left->key) > 0))
    {
	pNode->left = leftRotation(pNode->left); //turn to left/left
	return rightRotation(pNode);
    }

    //right/left case
    if (balance < -1 && (my_string_compare(key, pNode->right->key) < 0))
    {
	pNode->right = rightRotation(pNode->right); //turn to right/right
	return leftRotation(pNode);
    }

    return pNode;
}

void avl_insert(AVL hAvl, MY_STRING key, MY_STRING cur_word)
{
    avl *pAvl = (avl*)hAvl;
    
    pAvl->root = avl_ins(pAvl, pAvl->root, key, cur_word);
}


void avl_delete(AVL hAvl, MY_STRING key)
{
    avl *pAvl = (avl*)hAvl;
    NODE *pNode = pAvl->root;
    NODE *pPrev = NULL;
    NODE *pMin = NULL;
    NODE **ppMin = NULL;
    NODE **pptemp = &pAvl->root;

    if (pNode == NULL)
	return;

    while (pNode)
    {
	pPrev = pNode;

	if (my_string_compare(key, pNode->key) < 0)
	{
	    pptemp = &pNode->left;
	    pNode = pNode->left;
	}
	else if(my_string_compare(key, pNode->key) > 0)
	{
	    pptemp = &pNode->right;
	    pNode = pNode->right;
	}
	else
	    break;
    }

    if (!pNode)
	return;

    if (pNode->left == NULL && pNode->right ==NULL) //leaf node
    {
	*pptemp = NULL;
	free_node(pNode);
	return;
    }

    if (pNode->left == NULL) //right only
    {
	*pptemp = pNode->right;
	free_node(pNode);
	return;
    }

    if (pNode->right == NULL) //left only
    {
	*pptemp = pNode->left;
	free_node(pNode);
	return;
    }

    pMin = pNode->right;
    ppMin = &pNode->right;
    while(pMin->left)
    {
	ppMin = &pMin->left;
	pMin = pMin->left;
    }
    pMin->left = pNode->left;

    (*pptemp)->left = NULL;
    (*ppMin) = NULL;
    *pptemp = pMin;

    if(pMin->right)
    {
	NODE *px;
	NODE *phold = pMin->right;
	MY_STRING x = phold->key;

	pMin->right = pNode->right;
	px = pMin;
	while(px)
	{
	    pPrev = px;
	    if (my_string_compare(x, px->key) < 0)
	    {
		px = px->left;
	    }
	    else
	    {
		px = px->right;
	    }
	}
	if (my_string_compare(x, pPrev->key) < 0)
	{
	    pPrev->left = phold;
	}
	else
	{
	    pPrev->right = phold;
	}
    }
    else
	pMin->right = pNode->right;

    free(pNode);
}

NODE *avl_root(AVL hAvl)
{
    avl *pAvl = (avl*)hAvl;
    return pAvl->root;
}

void avl_inorder(AVL hAvl, void f(NODE *))
{
    avl *pAvl = (avl*)hAvl;
    NODE *pNode = pAvl->root;
    inorder(pNode, f);
}

void avl_postorder(AVL hAvl, void f(NODE *))
{
    avl *pAvl = (avl*)hAvl;
    NODE *pNode = pAvl->root;
    postorder(pNode, f);
}

static void print_node(NODE *pNode)
{
    printf("%s : [%d]\n", my_string_c_str(pNode->key), vector_get_size(pNode->word));
}

void avl_print(AVL hAvl)
{
    avl_inorder(hAvl, print_node);
}

void avl_destroy(AVL* phAvl)
{
    avl *pAvl = (avl*)*phAvl;

    avl_postorder(*phAvl, free_node);
    free(pAvl);
    *phAvl = NULL;
}
