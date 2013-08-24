
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os.h"

/*
 * This is a very slow linked-list implementation
 * Please modify to tree-based implementation
 */
struct node
{
	void* obj;
	struct node *left;
	struct node *right;
  struct node *parent;
  struct node *next;
  struct node *pre;
  size_t level, nChild;
	/*int dummyNode;*/
};

struct os_t
{
	struct node* head;
  struct os_inner_t *it;
	size_t objSize, size, cap;
	int (*cmp)(const void*, const void*);
	int dynamic;
};


/*Iterrator Support DS*/
/*
 * This structure is used to maintain current iterator state
 */
struct os_inner_t
{
	struct node* node;
  size_t objSize;
};


struct os_t *osAlloc()
{
	return malloc(sizeof(struct os_t));
}

struct node *newNode(size_t objSize, void *pObj, struct node *pParent)
{
  struct node *new = malloc(sizeof(struct node));
  new->parent = pParent;
  new->level = 1;
  new->nChild = 0;
  new->obj = malloc(objSize);
  memcpy(new->obj, pObj, objSize);
  new->left = new->right = NULL;
  new->pre = new->next = NULL;
  return new;
}

int osInit(struct os_t *pThis, size_t objSize, size_t cap,
		int (*cmp)(const void*, const void*))
{
 	if(cap != 0){
    pThis->cap = cap;
    pThis->dynamic = 0;
  }
  else
  {
    pThis->cap = 4;
    pThis->dynamic = 1;
  }
  pThis->head = NULL;
  pThis->it = malloc(sizeof(struct os_inner_t));
  pThis->it->node = pThis->head;
  pThis->it->objSize = objSize;
  pThis->objSize = objSize;
  pThis->cmp = cmp;
  pThis->size = 0;

	return __DS__OS__NORMAL__;
}
int nodeFree(struct node *cur)
{
  if (cur->left != NULL)
    nodeFree(cur->left);
  else if (cur->right != NULL)
    nodeFree(cur->right);
  else
  {
    free(cur->obj);
    free(cur);
  }
  return 0;
}
int osFree(struct os_t *pThis)
{
	struct node* cur = pThis->head;
  nodeFree(cur);
	free(pThis);
	return __DS__OS__NORMAL__;
}

size_t osSize(struct os_t *pThis)
{
	return pThis->size;
}

size_t osCap(struct os_t *pThis)
{
	return pThis->cap;
}

/*int osSetCap(struct os_t *pThis, size_t cap)*/
/*{*/
	/*pThis->cap = cap;*/
	/*return __DS__OS__NORMAL__;*/
/*}*/

size_t osObjSize(struct os_t *pThis)
{
	return pThis->objSize;
}

int osEmpty(struct os_t *pThis)
{
	if(pThis->head->next==NULL)//skip dummy node
		return __DS__OS__EMPTY__;
	return __DS__OS__NOT_EMPTY__;
}
/*Rotate*/
void R_Rotate(struct node *cur)
{

}
void L_Rotate(struct node *cur)
{

}
void RL_Rotate(struct node *cur)
{

}
void LR_Rotate(struct node *cur)
{

}
/*Buggy Handler*/
int buggy(struct node *cur, int type)
{
  /*type 1 means right > left*/
  /*type 2 means left > right*/
  if (type == 1)
  {
    if (cur->right->left == NULL)
      L_Rotate(cur->right);
    else if (cur->right->right == NULL)
      RL_Rotate(cur->right->left);
    if (cur->right->right->level > cur->right->left->level)
    {
      L_Rotate(cur->right);
    }
    else
    {
      RL_Rotate(cur->right->left);
    }
  }
  else
  {
    if (cur->left->left == NULL)
      LR_Rotate(cur->left->right);
    else if (cur->left->right == NULL)
      R_Rotate(cur->left);
    if (cur->left->left->level > cur->left->right->level)
    {
      R_Rotate(cur->left);
    }
    else
    {
      LR_Rotate(cur->left->right);
    }
  }
  return 0;
}
/*Function used to support iterator*/
int osGetByIt(osit it, void *pRetObj)
{
	if(it==NULL)return __DS__OS__INVALID_ITOR__;
	memcpy(pRetObj,it->node->obj,it->objSize);
	return __DS__OS__NORMAL__;
}

osit osNextIt(osit it)
{
	if(it->node->next==NULL)return NULL;
	osit ret = malloc(sizeof(struct os_inner_t));
	ret->node = it->node->next;
	ret->objSize = it->objSize;
	return ret;
}


osit osPrevIt(osit it)
{
	if(it->node->pre==NULL)return NULL;
	osit ret = malloc(sizeof(struct os_inner_t));
	ret->node = it->node->pre;
	ret->objSize = it->objSize;
	return ret;
}

/*Public Operator*/
int osInsert(struct os_t *pThis, void *pObj)
{
	if(pThis->size == pThis->cap)
    if (pThis->dynamic)
    {
      pThis->cap *= 2;
    }
    else
		  return __DS__OS__FULL__;
  if (pThis->size == 0)
  {
    pThis->head = newNode(pThis->objSize, pObj, NULL);
    pThis->size++;
  }
	struct node* cur = pThis->head;
  while (1)
  {
    if (pThis->cmp(cur->obj, pObj) == 0)
      return __DS__OS__OBJ_EXIST__;
    else if (pThis->cmp(cur-obj, pObj) > 0)
    {
      if (cur->right == NULL)
      {
        cur->right = newNode(pThis->objSize, pObj, cur);
        cur = cur->right;
        break;
      }
      else
      {
        cur = cur->right;
      }
    }
    else
    {
      if (cur->left == NULL)
      {
        cur->left = newNode(pThis->objSize, pObj, cur);
        cur = cur->left;
        break;
      }
      else
      {
        cur = cur->left;
      }
    }
  }
  /*update level and nChild*/
  struct node *temp = cur;
  while (temp != head)
  {
    temp->parent->levle++;
    temp->parent->nChild++;
  }
  while (1)
  {
    if (cur == pThis->head)
      break;
    if (cur->left == NULL || cur->right == NULL)
    {
      if (cur->left == NULL && cur->right == NULL)
        ;
      if (cur->right->level == 2)
      {
        buggy(cur, 1);
        /*left = 0 right = 2*/
        break;
      }
      else if (cur->left->level == 2)
      {
        buggy(cur, 2);
        /*left = 2 right = NULL*/
        break;
      }
    }
    else if (cur->right->level - cur->left->level == 2)
    {
      buggy(cur, 1);
      /*right > left*/
      break;
    }
    else if (cur->right->level - cur->left->level == -2)
    {
      buggy(cur, 2);
      /*right < left*/
      break;
    }
    cur = cur->parent;
  }
	pThis->size++;
	return __DS__OS__NORMAL__;
}

int osDelete(struct os_t *pThis, void *pObj)
{
	struct node* cur = pThis->head;
  /*find obj*/
  while (1)
  {
    if (pThis->cmp(pObj, cur->obj) == 0)
      break;
    else if (pThis->cmp(pObj, cur->obj) > 0)
    {
      if (cur->right == NULL)
        return __DS__OS__OBJ_NOY_EXIST__;
      else
        cur = cur->right;
    }
    else
    {
      if (cur->left == NULL)
        return __DS__OS__OBJ_NOT_EXIST__;
      else
        cur = cur->left;
    }
  }
  /*delete obj*/
  pThis->it
  /*balance*/
}

int osFind(struct os_t *pThis, void *pObj)
{
	struct node* cur = pThis->head;
	while(cur!=NULL)
 	{
		if(pThis->cmp(pObj,cur->object)==0 && cur->dummyNode==0)
 		{
			return __DS__OS__OBJ_EXIST__;
		}
		cur = cur->next;
	}
	return __DS__OS__OBJ_NOT_EXIST__;
}

int osLowerBound(struct os_t *pThis, void *lowObj, void *pRetObj)
{
	struct node* cur = pThis->head;
	struct node* pNode = NULL;
	while(cur!=NULL)
 	{
		if(	pThis->cmp(cur->object,lowObj)>=0 && cur->dummyNode==0 &&
		   (pNode==NULL || pThis->cmp(cur->object,pNode->object)<0) )
 		{
			pNode = cur;
		}
		cur = cur->next;
	}
	if(pNode==NULL) return __DS__OS__OBJ_NOT_EXIST__;
	memcpy(pRetObj, pNode->object, pThis->objSize);
	return __DS__OS__NORMAL__;
}

int osUpperBound(struct os_t *pThis, void *upObj, void *pRetObj)
{
	struct node* cur = pThis->head;
	struct node* pNode = NULL;
	while(cur!=NULL)
	{
		if( pThis->cmp(cur->object,upObj)<=0 && cur->dummyNode==0 &&
			(pNode==NULL || pThis->cmp(cur->object,pNode->object)>0) )
		{
			pNode = cur;
		}
		cur = cur->next;
	}
	if(pNode==NULL) return __DS__OS__OBJ_NOT_EXIST__;
	memcpy(pRetObj, pNode->object, pThis->objSize);
	return __DS__OS__NORMAL__;
}


//Iterator support operator
int osFindIt(struct os_t *pThis, void *pObj, osit *pRetIt)
{
	struct node* cur = pThis->head;
	while(cur!=NULL)
 	{
		if(pThis->cmp(pObj,cur->object)==0 && cur->dummyNode==0)
 		{
			osit ret = malloc(sizeof(struct os_inner_t));
			ret->node = cur;
			ret->objSize = pThis->objSize;
			*pRetIt = ret;
			return __DS__OS__OBJ_EXIST__;
		}
		cur = cur->next;
	}
	return __DS__OS__OBJ_NOT_EXIST__;
}

int osLowerIt(struct os_t *pThis, void *pObj, osit *pRetIt)
{
	struct node* cur = pThis->head;
	struct node* pNode = NULL;
	while(cur!=NULL)
 	{
		if(	pThis->cmp(cur->object,pObj)>=0 && cur->dummyNode==0 &&
		   (pNode==NULL || pThis->cmp(cur->object,pNode->object)<0) )
 		{
			pNode = cur;
		}
		cur = cur->next;
	}
	osit ret = NULL;
	*pRetIt = ret;
	if(pNode==NULL) return __DS__OS__OBJ_NOT_EXIST__;
	ret = (osit)malloc(sizeof(struct os_inner_t));
	ret->node = pNode;
    ret->objSize = pThis->objSize;
	return __DS__OS__NORMAL__;
}

int osUpperIt(struct os_t *pThis, void *pObj, osit *pRetIt)
{
	struct node* cur = pThis->head;
	struct node* pNode = NULL;
	while(cur!=NULL)
	{
		if( pThis->cmp(cur->object,pObj)<=0 && cur->dummyNode==0 &&
			(pNode==NULL || pThis->cmp(cur->object,pNode->object)>0) )
		{
			pNode = cur;
		}
		cur = cur->next;
	}
	osit ret = NULL;
	*pRetIt = ret;
	if(pNode==NULL) return __DS__OS__OBJ_NOT_EXIST__;
	ret = malloc(sizeof(struct os_inner_t));
	ret->node = cur;
	ret->objSize = pThis->objSize;
	return __DS__OS__NORMAL__;
}

int osFreeIt(osit *pRetIt)
{
	free(pRetIt);
	return __DS__OS__NORMAL__;
}

//Bonus
int osFindKthObj(struct os_t *pThis, int k, void *pRetObj)
{
	return 0;
}
