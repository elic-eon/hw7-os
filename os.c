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
  new->nChild = 1;
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
  /*pThis->it = malloc(sizeof(struct os_inner_t));*/
  /*pThis->it->node = pThis->head;*/
  /*pThis->it->objSize = objSize;*/
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
	if(pThis->size == 0)//skip dummy node
		return __DS__OS__EMPTY__;
	return __DS__OS__NOT_EMPTY__;
}
/*Rotate*/
struct node * R_Rotate(struct node *cur)
{
  if (cur->parent->parent == NULL) ;
  else if (cur->parent->parent->left == cur->parent)
    cur->parent->parent->left = cur;
  else
    cur->parent->parent->right = cur;
  cur->parent->left = cur->right;
  if (cur->right)
    cur->right->parent = cur->parent;
  cur->right = cur->parent;
  cur->parent = cur->parent->parent;
  cur->right->parent = cur;
  /*cur->right->level -= (cur->right->level > 2) ? 2 : (cur->right->level == 2) ? 1 : 0;*/
  /*
  cur->level = (cur->left == NULL) ? cur->right->level+1 :
  (cur->right->level > cur->left->level) ? cur->right->level+1 : cur->left->level+1;*/
  if (cur->right->left == NULL && cur->right->right == NULL)
    cur->right->level = 1;
  else if (cur->right->right == NULL)
  {
    cur->right->level = cur->right->left->level+1;
  }
  else if (cur->right->left == NULL)
  {
    cur->right->level = cur->right->right->level+1;
  }
  else
  {
    cur->right->level = (cur->right->right->level > cur->right->left->level) ? cur->right->right->level+1: cur->right->left->level+1;
  }
  if (cur->left == NULL)
  {
    cur->level = cur->right->level+1;
  }
  else
  {
    cur->level = (cur->right->level > cur->left->level) ? cur->right->level+1: cur->left->level+1;
  }
  cur->nChild += (cur->right->right == NULL)? 1 : (cur->right->right->nChild + 1);
  cur->right->nChild -= (cur->left == NULL) ? 1 : (cur->left->nChild + 1);
  return cur;
}
struct node * L_Rotate(struct node *cur)
{
  if (cur->parent->parent == NULL) ;
  else if (cur->parent->parent->right == cur->parent)
    cur->parent->parent->right = cur;
  else
    cur->parent->parent->left = cur;
  cur->parent->right = cur->left;
  if (cur->left)
    cur->left->parent = cur->parent;
  cur->left = cur->parent;
  cur->parent = cur->parent->parent;
  cur->left->parent = cur;
  /*cur->left->level -= (cur->left->level > 2 ) ? 2 : (cur->left->level == 2) ? 1 : 0;*/
  /*cur->level = (cur->right == NULL) ? cur->left->level+1 :
    ((cur->right->level > cur->left->level) ? (cur->right->level+1) : (cur->left->level+1));*/
  if (cur->left->left == NULL && cur->left->right == NULL)
    cur->left->level = 1;
  else if (cur->left->right == NULL)
  {
    cur->left->level = cur->left->left->level+1;
  }
  else if (cur->left->left == NULL)
  {
    cur->left->level = cur->left->right->level+1;
  }
  else
  {
    cur->left->level = (cur->left->right->level > cur->left->left->level) ? cur->left->right->level+1: cur->left->left->level+1;
  }
  if (cur->right == NULL)
  {
    cur->level = cur->left->level+1;
  }
  else
  {
    cur->level = (cur->right->level > cur->left->level) ? cur->right->level+1: cur->left->level+1;
  }
  cur->nChild += (cur->left->left == NULL) ? 1 : (cur->left->left->nChild + 1);
  cur->left->nChild -= (cur->right == NULL) ? 1 : (cur->right->nChild + 1);
  return cur;
}
struct node * RL_Rotate(struct node *cur)
{
  cur = R_Rotate(cur);
  cur = L_Rotate(cur);
  return cur;
}
struct node * LR_Rotate(struct node *cur)
{
  cur = L_Rotate(cur);
  cur = R_Rotate(cur);
  return cur;
}
/*Buggy Handler*/
struct node * buggy(struct node *cur, int type)
{
  /*type 1 means right > left*/
  /*type 2 means left > right*/
  if (type == 1)
  {
    if (cur->right->left == NULL)
      return L_Rotate(cur->right);
    else if (cur->right->right == NULL)
      return RL_Rotate(cur->right->left);
    else if (cur->right->right->level > cur->right->left->level)
      return L_Rotate(cur->right);
    else
      return RL_Rotate(cur->right->left);
  }
  else
  {
    if (cur->left->left == NULL)
      return LR_Rotate(cur->left->right);
    else if (cur->left->right == NULL)
      return R_Rotate(cur->left);
    else if (cur->left->left->level > cur->left->right->level)
      return R_Rotate(cur->left);
    else
      return LR_Rotate(cur->left->right);
  }
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
      pThis->cap *= 2;
    else
		  return __DS__OS__FULL__;
  if (pThis->size == 0)
  {
    pThis->head = newNode(pThis->objSize, pObj, NULL);
    pThis->size++;
    return __DS__OS__NORMAL__;
  }
	struct node* cur = pThis->head;
  while (1)
  {
    if (pThis->cmp(cur->obj, pObj) == 0)
      return __DS__OS__OBJ_EXIST__;
    else if (pThis->cmp(cur->obj, pObj) < 0)
    {
      if (cur->right == NULL)
      {
        cur->right = newNode(pThis->objSize, pObj, cur);
        if (cur->next == NULL)
        {
          cur->next = cur->right;
          cur->right->pre = cur;
        }
        else
        {
          cur->right->pre = cur;
          cur->right->next = cur->next;
          cur->next->pre = cur->right;
          cur->next = cur->right;
        }
        cur = cur->right;
        break;
      }
      else
        cur = cur->right;
    }
    else
    {
      if (cur->left == NULL)
      {
        cur->left = newNode(pThis->objSize, pObj, cur);
        if (cur->pre == NULL)
        {
          cur->pre = cur->left;
          cur->left->next = cur;
        }
        else
        {
          cur->left->next = cur;
          cur->left->pre = cur->pre;
          cur->pre->next = cur->left;
          cur->pre = cur->left;
        }
        cur = cur->left;
        break;
      }
      else
      {
        cur = cur->left;
      }
    }
  }
  /*update level*/
  while (1)
  {
    if (cur->left == NULL && cur->right == NULL)
      cur->level = 1;
    else if (cur->right == NULL)
    {
      cur->level = cur->left->level+1;
    }
    else if (cur->left == NULL)
    {
      cur->level = cur->right->level+1;
    }
    else
    {
      cur->level = (cur->right->level > cur->left->level) ? cur->right->level+1: cur->left->level+1;
    }
    if (cur->left == NULL || cur->right == NULL)
    {
      if (cur->left == NULL && cur->right == NULL)
        ;
      else if (cur->left == NULL)
      {
        if (cur->right->level == 2)
        {
          if (cur == pThis->head)
            pThis->head = buggy(cur, 1);
          else
            buggy(cur, 1);
          break;
        }
        /*left = 0 right = 2*/
      }
      else if (cur->right == NULL)
      {
        if (cur->left->level == 2)
        {
          if (cur == pThis->head)
            pThis->head = buggy(cur, 2);
          else
            buggy(cur, 2);
          break;
        }
        /*left = 2 right = NULL*/
      }
    }
    else if (cur->right->level - cur->left->level == 2)
    {
      if (cur == pThis->head)
      {
        pThis->head = buggy(cur, 1);
      }
      else
        buggy(cur, 1);
      /*right > left*/
      break;
    }
    else if (cur->right->level - cur->left->level == -2)
    {
      if (cur == pThis->head)
      {
        pThis->head = buggy(cur, 2);
      }
      else
        buggy(cur, 2);
      /*right < left*/
      break;
    }
    if (cur == pThis->head)
      break;
    cur = cur->parent;
  }
	pThis->size++;
	return __DS__OS__NORMAL__;
}

int osDelete(struct os_t *pThis, void *pObj)
{
  if (pThis->size == 0)
    return __DS__OS__OBJ_NOT_EXIST__;
	struct node* cur = pThis->head;
  if (pThis->size <= 3)
  {
    if (pThis->size == 0)
      return __DS__OS__EMPTY__;
    if (pThis->size == 1)
    {
      free(cur->obj);
      free(cur);
      pThis->size--;
      return __DS__OS__OBJ_EXIST__;
    }
    if (pThis->cmp(cur->obj, pObj) == 0)
    {
      if (cur->right)
      {
        pThis->head = cur->right;
        pThis->head->pre = cur->pre;
        pThis->head->parent = NULL;
        pThis->head->left = cur->left;
        if (cur->left)
          cur->left->parent = pThis->head;
        free(cur->obj);
        free(cur);
        pThis->size--;
        return __DS__OS__NORMAL__;
      }
      else
      {
        pThis->head = cur->left;
        pThis->head->next = cur->next;
        pThis->head->parent = NULL;
        pThis->head->right = cur->right;
        if (cur->right)
          cur->right->parent = pThis->head;
        free(cur->obj);
        free(cur);
        pThis->size--;
        return __DS__OS__NORMAL__;
      }
    }
    if (pThis->cmp(cur->obj, pObj) > 0)
    {
      if (cur->left == NULL)
        return __DS__OS__OBJ_NOT_EXIST__;
      if (pThis->cmp(cur->left->obj, pObj) != 0)
        return __DS__OS__OBJ_NOT_EXIST__;
      else
      {
        free(cur->left->obj);
        free(cur->left);
        pThis->head->left = NULL;
        pThis->head->pre = NULL;
        pThis->size--;
        return __DS__OS__NORMAL__;
      }
    }
    else
    {
      if (cur->right == NULL)
        return __DS__OS__OBJ_NOT_EXIST__;
      if (pThis->cmp(cur->right->obj, pObj) != 0)
        return __DS__OS__OBJ_NOT_EXIST__;
      else
      {
        free(cur->right->obj);
        free(cur->right);
        pThis->head->right = NULL;
        pThis->head->next = NULL;
        pThis->size--;
        return __DS__OS__NORMAL__;
      }
    }
  }
  /*find obj*/
  while (1)
  {
    if (pThis->cmp(pObj, cur->obj) == 0)
      break;
    else if (pThis->cmp(pObj, cur->obj) > 0)
    {
      if (cur->right == NULL)
        return __DS__OS__OBJ_NOT_EXIST__;
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
  struct node *re;
  struct node *temp;
  if (cur->next != NULL && cur->next->level < cur->level)
  {
    re = cur->next;
    if (cur == pThis->head)
    {
      pThis->head = re;
      pThis->head->parent = NULL;
    }
    temp = re->parent;
    if (temp == cur)
      temp = re;
    /*fix level nChild*/
    re->parent->nChild--;
    /*fix linked list*/
    if (cur->pre)
      cur->pre->next = re;
    re->pre = cur->pre;
    /*fix tree structure*/
    if (re->parent == cur)
    {
      re->left = cur->left;
      if (re->left)
        re->left->parent = re;
      re->parent = cur->parent;
      if (cur->parent)
        if (cur->parent->right == cur)
          cur->parent->right = re;
        else
          cur->parent->left = re;
    }
    else
    {
      if (re->right != NULL)
      {
        re->parent->left = re->right;
        re->right->parent = re->parent;
      }
      else
      {
        re->parent->left = NULL;
      }
      /*fix connection*/
      re->parent = cur->parent;
      re->left = cur->left;
      if (re->left != NULL)
        re->left->parent = re;
      re->right = cur->right;
      if (re->right != NULL)
        re->right->parent = re;
    }
    free(cur->obj);
    free(cur);
  }
  else if (cur->pre != NULL && cur->pre->level < cur->level)
  {
    re = cur->pre;
    if (cur == pThis->head)
    {
      pThis->head = re;
      pThis->head->parent;
    }
    temp = re->parent;
    if (temp == cur)
      temp = re;
    /*fix level nChild*/
    re->parent->nChild--;
    /*fix linked list*/
    if (cur->next)
      cur->next->pre = re;
    re->next = cur->next;
    /*fix tree structure*/
    if (re->parent == cur)
    {
      re->right = cur->right;
      if (re->right)
        re->right->parent = re;
      re->parent = cur->parent;
      if (cur->parent)
        if (cur->parent->left == cur)
          cur->parent->left = re;
        else
          cur->parent->right = re;
    }
    else
    {
      if (re->left != NULL)
      {
        re->parent->right = re->left;
        re->left->parent = re->parent;
      }
      else
      {
        re->parent->right = NULL;
      }
      /*fix connection*/
      re->parent = cur->parent;
      re->left = cur->left;
      if (re->left != NULL)
        re->left->parent = re;
      re->right = cur->right;
      if (re->right != NULL)
        re->right->parent = re;
    }
    free(cur->obj);
    free(cur);
  }
  else
  {
    temp = cur->parent;
    if (cur->pre)
      cur->pre->next = cur->next;
    if (cur->next)
      cur->next->pre = cur->pre;
    if (cur->parent == NULL)
      ;
    else if (cur->parent->right == cur)
    {
      cur->parent->right = NULL;
      if (cur->parent)
      {
        cur->parent->level = (cur->parent->left) ? cur->parent->left->level+1 : 1;
      }
    }
    else if (cur->parent->left == cur)
    {
      cur->parent->left = NULL;
      if (cur->parent)
      {
        cur->parent->level = (cur->parent->right) ? cur->parent->right->level+1 : 1;
      }
    }
    free(cur->obj);
    free(cur);
    if (pThis->size == 1)
    {
      pThis->size--;
      return __DS__OS__NORMAL__;
    }
  }
  /*balance*/
  cur = temp;
  while (1)
  {
    if (cur->left == NULL && cur->right == NULL)
      cur->level = 1;
    else if (cur->right == NULL)
    {
      cur->level = cur->left->level+1;
    }
    else if (cur->left == NULL)
    {
      cur->level = cur->right->level+1;
    }
    else
    {
      cur->level = (cur->right->level > cur->left->level) ? cur->right->level+1: cur->left->level+1;
    }
    if (cur->left == NULL || cur->right == NULL)
    {
      if (cur->left == NULL && cur->right == NULL)
        ;
      else if (cur->left == NULL)
      {
        if (cur->right->level == 2)
        {
          if (cur == pThis->head)
            pThis->head = buggy(cur, 1);
          else
            buggy(cur, 1);
        }
        /*left = 0 right = 2*/
      }
      else if (cur->right == NULL)
      {
        if (cur->left->level == 2)
        {
          if (cur == pThis->head)
            pThis->head = buggy(cur, 2);
          else
            buggy(cur, 2);
        }
        /*left = 2 right = NULL*/
      }
    }
    else if (cur->right->level - cur->left->level == 2)
    {
      if (cur == pThis->head)
        pThis->head = buggy(cur, 1);
      else
        buggy(cur, 1);
      /*right > left*/
    }
    else if (cur->left->level - cur->right->level == 2)
    {
      if (cur == pThis->head)
        pThis->head = buggy(cur, 2);
      else
        buggy(cur, 2);
      /*right < left*/
    }
    if (cur->parent == NULL)
      break;
    cur = cur->parent;
  }
  pThis->size--;
  return __DS__OS__NORMAL__;
}

int osFind(struct os_t *pThis, void *pObj)
{
  if (pThis->size == 0)
    return __DS__OS__OBJ_NOT_EXIST__;
	struct node* cur = pThis->head;
  while (1)
  {
    if (pThis->cmp(pObj, cur->obj) == 0)
    {
      return __DS__OS__OBJ_EXIST__;
    }
    else if (pThis->cmp(pObj, cur->obj) > 0)
    {
      if (cur->right == NULL)
        return __DS__OS__OBJ_NOT_EXIST__;
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
}

int osLowerBound(struct os_t *pThis, void *lowObj, void *pRetObj)
{
  if (pThis->size == 0)
    return __DS__OS__OBJ_NOT_EXIST__;
  struct node* cur = pThis->head;
  while (1)
  {
    if (pThis->cmp(lowObj, cur->obj) == 0)
    {
      if (cur->pre == NULL)
        memcpy(pRetObj, cur->obj, pThis->objSize);
      else
        memcpy(pRetObj, cur->pre->obj, pThis->objSize);
      break;
    }
    else if (pThis->cmp(lowObj, cur->obj) > 0)
    {
      if (cur->right == NULL)
        return __DS__OS__OBJ_NOT_EXIST__;
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
	return __DS__OS__NORMAL__;
}

int osUpperBound(struct os_t *pThis, void *upObj, void *pRetObj)
{
  if (pThis->size == 0)
    return __DS__OS__OBJ_NOT_EXIST__;
  struct node* cur = pThis->head;
  while (1)
  {
    if (pThis->cmp(upObj, cur->obj) == 0)
    {
      if (cur->next == NULL)
        memcpy(pRetObj, cur->obj, pThis->objSize);
      else
        memcpy(pRetObj, cur->next->obj, pThis->objSize);
      break;
    }
    else if (pThis->cmp(upObj, cur->obj) > 0)
    {
      if (cur->right == NULL)
        return __DS__OS__OBJ_NOT_EXIST__;
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
	return __DS__OS__NORMAL__;
}


//Iterator support operator
int osFindIt(struct os_t *pThis, void *pObj, osit *pRetIt)
{
	/*struct node* cur = pThis->head;*/
	/*while(cur!=NULL)*/
   /*{*/
		/*if(pThis->cmp(pObj,cur->object)==0 && cur->dummyNode==0)*/
     /*{*/
			/*osit ret = malloc(sizeof(struct os_inner_t));*/
			/*ret->node = cur;*/
			/*ret->objSize = pThis->objSize;*/
			/**pRetIt = ret;*/
			/*return __DS__OS__OBJ_EXIST__;*/
		/*}*/
		/*cur = cur->next;*/
	/*}*/
	return __DS__OS__OBJ_NOT_EXIST__;
}

int osLowerIt(struct os_t *pThis, void *pObj, osit *pRetIt)
{
	/*struct node* cur = pThis->head;*/
	/*struct node* pNode = NULL;*/
	/*while(cur!=NULL)*/
   /*{*/
		/*if(	pThis->cmp(cur->object,pObj)>=0 && cur->dummyNode==0 &&*/
			 /*(pNode==NULL || pThis->cmp(cur->object,pNode->object)<0) )*/
     /*{*/
			/*pNode = cur;*/
		/*}*/
		/*cur = cur->next;*/
	/*}*/
	/*osit ret = NULL;*/
	/**pRetIt = ret;*/
	/*if(pNode==NULL) return __DS__OS__OBJ_NOT_EXIST__;*/
	/*ret = (osit)malloc(sizeof(struct os_inner_t));*/
	/*ret->node = pNode;*/
    /*ret->objSize = pThis->objSize;*/
	return __DS__OS__NORMAL__;
}

int osUpperIt(struct os_t *pThis, void *pObj, osit *pRetIt)
{
	/*struct node* cur = pThis->head;*/
	/*struct node* pNode = NULL;*/
	/*while(cur!=NULL)*/
	/*{*/
		/*if( pThis->cmp(cur->object,pObj)<=0 && cur->dummyNode==0 &&*/
			/*(pNode==NULL || pThis->cmp(cur->object,pNode->object)>0) )*/
		/*{*/
			/*pNode = cur;*/
		/*}*/
		/*cur = cur->next;*/
	/*}*/
	/*osit ret = NULL;*/
	/**pRetIt = ret;*/
	/*if(pNode==NULL) return __DS__OS__OBJ_NOT_EXIST__;*/
	/*ret = malloc(sizeof(struct os_inner_t));*/
	/*ret->node = cur;*/
	/*ret->objSize = pThis->objSize;*/
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
