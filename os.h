#include <stdlib.h>
#ifndef __ds__os__header__
#define __ds__os__header__

#define __DS__OS__NORMAL__        0x00000000
#define __DS__OS__FULL__          0x00000001
#define __DS__OS__EMPTY__         0x00000002
#define __DS__OS__NOT_EMPTY__     0x00000004
#define __DS__OS__OUT_OF_MEM__    0x00000008
#define __DS__OS__OBJ_EXIST__     0x00000010
#define __DS__OS__OBJ_NOT_EXIST__ 0x00000020
#define __DS__OS__INVALID_ITOR__  0x00000040

struct os_t;
/* A simple set contain ordered elements
 * Support customized comparison
 * If it is OOM(out of memory) in the memory allocation,
 * Please return __DS__OS__OUT_OF_MEM__ for each function which supports to 
 * return __DS__OS__NORMAL__ when normal execution. 
 */

struct os_t *osAlloc();
/* Allocate memory for os. Return NULL if not enough memory */

int osInit(struct os_t *pThis, size_t objSize, size_t cap,
           int (*cmp)(const void*, const void*));
/* Initialize pThis: set the size of object as objSize
 * and the capacity as cap.
 * the comparison function as cmp: 
 *     cmp(objA, objB) returns -1 if keyA < keyB
 *     cmp(objA, objB) returns 0 if keyA = keyB
 *     cmp(objA, objB) returns 1 if keyA > keyB
 * If cmp(objA, objB)==0 then objA and objB are identical.
 * After the initialization, there's nothing. So size is 0.
 * Return __DS__OS__NORMAL__ for normal execution.
 * Bonus1: Using dynamic capacity when cap=0.
 */

int osFree(struct os_t *pThis);
/*  Release the memory. 
 * Deep release: You should release the memory allocated by any operation.
 * Return __DS__OS__NORMAL__ for normal execution.
 */

size_t osSize(struct os_t *pThis);
/* Return number of elements in the order set */

size_t osCap(struct os_t *pThis);
/* Return current capacity */

size_t osObjSize(struct os_t *pThis);
/* Return the size of obj in the order set */

int osEmpty(struct os_t *pThis);
/* Return zero if order set is not empty */

int osInsert(struct os_t *pThis, void *pObj);
/* Insert a new object into order set pThis
 * Input: pThis, pointer to the ordered set
 *        pObj, pointer to the object
 * Output: __DS__OS__NORMAL__ for normal execution & *pObj was not in *pThis
 *         __DS__OS__OBJ_EXIST__  if *pObj is already in *pThis
 *         __DS__OS__OUT_OF_MEM__ for memory unavailable
 * Note 1: osInsert should not change the set represented by *pThis if *pObj was in *pThis
 * Note 2: osInsert should be implement in O(log n)
 */

int osDelete(struct os_t *pThis, void *pObj);
/* Delete a object in the set 
 * Input: pThis, pointer to order set
 *        pObj, pointer to object
 * Output: __DS__OS__NORMAL__ for normal execution & *pObj was in *pThis
 *         __DS__OS__OBJ_NOT_EXIST__ if *pObj was not in *pThis
 * Note 1: osDelete should not change the set represented by *pThis if *pObj was not in *pThis
 * Note 2: osDelete should be implement in O(log n)
 */

int osFind(struct os_t *pThis, void *pObj);
/* Check if certain object is in the set
 * Input: pThis, pointer to the ordered set
 *        pObj, pointer to the object
 * Output: __DS__OS__OBJ_NOT_EXIST__ if *pObj is not in *pThis
 *         __DS__OS__OBJ_EXIST__ if *pObj is in *pThis
 * Note that osFind should be implement in O(log n)
 */

int osLowerBound(struct os_t *pThis, void *pObj, void *pRetObj);
/* Find the greatest object in *pThis which is no greater than *pObj
 * Input: pThis, pointer to order set
 *        pObj, pointer to the object
 *        pRetObj, poiter to the result object
 * Output: __DS__OS__NORMAL__ for normal execution
 *         __DS__OS__OBJ_NOT_EXIST__ if no object found
 * Note 1: osLowerBound puts *pObj into *pRetObj if *pObj is in *pThis
 * Note 2: osLowerBound should be implement in O(log n)
 */

int osUpperBound(struct os_t *pThis, void *pObj, void *pRetObj);
/* Find the smallest object in *pThis which is no smaller than *pObj
 * Input: pThis, pointer to order set
 *        pObj, pointer to the object
 *        pRetObj, poiter to the result object
 * Output: __DS__OS__NORMAL__ for normal execution
 *         __DS__OS__OBJ_NOT_EXIST__ if no object found
 * Note 1: osUpperBound puts *pObj into *pRetObj if *pObj is in *pThis
 * Note 2: osUpperBound should be implement in O(log n)
 */

/* Bonus 2: iterator support! complete the following structures and 6 functions
 * Goal: Get previous/next iterator in O(1) time */

struct os_inner_t;
/* the inner cell stores an element and its structural information */

typedef struct os_inner_t *osit;
/* Ordered set iterator: a pointer to os_inner_t */

int osFreeIt(osit *pRetIt);
/* Release the memory of iterator. 
 * Deep release: You should release the memory allocated by any operation.
 * Return __DS__OS__NORMAL__ for normal execution.
 */

int osFindIt(struct os_t *pThis, void *pObj, osit *pRetIt);
/* Check if certain object is in the set
 * Input: pThis, pointer to the ordered set
 *        pObj, pointer to the object
 *        *pRetIt, poinrter to the iterator storing return value
 * Output: __DS__OS__OBJ_NOT_EXIST__ if *pObj is not in *pThis
 *         __DS__OS__OBJ_EXIST__ if *pObj is in *pThis
 * Note 1: osFindIt should be implement in O(log n)
 * Note 2: *pRetIt should be NULL if *pObj is not in *pThis
 *         otherwise *pRetIt should be the iterator indicating the inner
 *		   cell which contains *pObj
 */

int osLowerIt(struct os_t *pThis, void *pObj, osit *pRetIt);
/* Find the iterator to greatest object in *pThis which is no greater than *pObj
 * Input: pThis, pointer to order set
 *        pObj, pointer to the object
 *        pRetIt, poiter to the iterator indicating the result
 * Output: __DS__OS__NORMAL__ for normal execution
 *         __DS__OS__OBJ_NOT_EXIST__ if no object found
 * Note 1: *pRetIt should be the iterator indicating lower bound of *pObj
 *         *pRetIt should be NULL if *pObj has no lower bound.
 * Note 2: osLowerIt should be implement in O(log n)
 */

int osUpperIt(struct os_t *pThis, void *pObj, osit *pRetIt);
/* Find the iterator to smallest object in *pThis which is no smaller than *pObj
 * Input: pThis, pointer to order set
 *        pObj, pointer to the object
 *        pRetIt, poiter to the iterator indicating the result
 * Output: __DS__OS__NORMAL__ for normal execution
 *         __DS__OS__OBJ_NOT_EXIST__ if no object found
 * Note 1: *pRetIt should be the iterator indicating upper bound of *pObj
 *         *pRetIt should be NULL if *pObj has no upper bound.
 * Note 2: osUpperIt should be implement in O(log n)
 */

int osGetByIt(osit it, void *pRetObj);
/* Get element by iterator
 * Input: pThis, pointer to the ordered set
 *        pRetObj, pointer to the object storing result
 * Output: __DS__OS__INVALID_ITOR__ if it is invalid
 *         __DS__OS__NORMAL__ for normal execution
 * Note that osGetByIt should be implement in O(1)
 */

osit osNextIt(osit it);
/* Get the iterator of the next element of the one indicated by it
 * Input: it, iterator
 * Output: NULL if it is invalid or it indicates the last element
 * Note that osPrevIt should be implement in O(1)
 */

osit osPrevIt(osit it);
/* Get the iterator of the previous element of the one indicated by it
 * Input: it, iterator
 * Output: NULL if it is invalid or it indicates the first element
 * Note that osPrevIt should be implement in O(1)
 */

int osFindKthObj(struct os_t *pThis, int k, void *pRetObj);
/* Bonus 3 
 * Random Access k-th smallest object in the set
 * Input: pThis, pointer to order set
 *        pRetObj, poiter to the result object
 *        itor, pointer to pointer of iterator
 * Output: __DS__OS__NORMAL__ for normal execution
 *         __DS__OS__OBJ_NOT_EXIST__ if no object found
 * Note this function must be implement in O(logn)
 */
#endif
