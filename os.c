
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
	void* object;
	struct node* next;
	struct node* pre;
	int dummyNode;
 };

struct os_t
 {   
	struct node* head;
	size_t objSize,  size, cap;
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

int osInit(struct os_t *pThis, size_t objSize, size_t cap,
		int (*cmp)(const void*, const void*))
{  
 	if(cap != 0){
		pThis->head = NULL;
		pThis->objSize = objSize;
		pThis->cmp = cmp;
		pThis->cap = cap;
		pThis->dynamic = 0;
		pThis->size = 0;

		struct node* dummy = (struct node*)malloc(sizeof(struct node));
		dummy->pre = dummy->next = NULL;
		dummy->dummyNode = 1;
		dummy->object = calloc(sizeof(pThis->objSize),0);
		pThis->head = dummy;
		if(dummy==NULL || dummy->object==NULL)
			return __DS__OS__OUT_OF_MEM__;
	}
	return __DS__OS__NORMAL__;
}

int osFree(struct os_t *pThis)
{   
	struct node* cur = pThis->head;
	while(cur!=NULL)
   	{
		struct node*next = cur->next;
		free(cur);
		cur=next;
	}
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

int osSetCap(struct os_t *pThis, size_t cap)
{
	pThis->cap = cap;
	return __DS__OS__NORMAL__;
}

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

/*Function used to support iterator*/
int osGetByIt(osit it, void *pRetObj)
{
	if(it==NULL)return __DS__OS__INVALID_ITOR__;
	memcpy(pRetObj,it->node->object,it->objSize);
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
		return __DS__OS__FULL__;
	struct node* cur = pThis->head;
	while(1)
 	{ 
		if(pThis->cmp(pObj,cur->object)==0 && cur->dummyNode==0)
 	 	{
			return __DS__OS__OBJ_EXIST__;
	 	}
		if(cur->next==NULL)
			break;
		cur = cur->next;
	} 
	struct node* tmp = (struct node*)malloc(sizeof(struct node));
	tmp->object = malloc(sizeof(pThis->objSize));
	//if(tmp->object == NULL)
	//	return __DS__OS__OUT_OF_MEM__;
	tmp->next = NULL;
	tmp->pre = cur;
	tmp->dummyNode = 0;
	memcpy(tmp->object,pObj,pThis->objSize);
	cur->next = tmp;
	pThis->size ++;
	return __DS__OS__NORMAL__;
}

int osDelete(struct os_t *pThis, void *pObj)
{ 
	struct node* cur = pThis->head;
	while(cur!=NULL)
 	{
		if(pThis->cmp(pObj,cur->object)==0 && cur->dummyNode==0)
		{
			if(cur->next!=NULL)cur->next->pre = cur->pre;
			cur->pre->next = cur->next;
			struct node* tmp = cur->next;
			free(cur);
			pThis->size --;
			return __DS__OS__NORMAL__;
 		}
		cur = cur->next;
	}
	return __DS__OS__OBJ_NOT_EXIST__;
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
