#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "utility.h"
#include "os.h"

#define DOUBLE_NEG_INF (-1.0/0.0)
#define SHORT_NEG_INF SHRT_MIN
#define DOUBLE_INF (1.0/0.0) 
#define SHORT_INF SHRT_MAX

typedef struct os_t os;

int doubleGT(const void *a, const void *b)
{
	double da=*(double*)a;
	double db=*(double*)b;
	if(da==db)return 0;
	return (da>db)?1:-1;
}
int shortGT(const void *a, const void *b)
{
	short da=*(short*)a;
	short db=*(short*)b;
	if(da==db)return 0;
	return (da>db)?1:-1;
}

int main()
{
    os *p = NULL;
	osit it = NULL;

    size_t cap, tSize;
    int type;
    void *data = malloc(sizeof(double)*10);
	void *data2 = malloc(sizeof(double)*10);
    double double_neg_inf = DOUBLE_NEG_INF;
    short short_neg_inf = SHORT_NEG_INF;
	
    while(1){
        char cmd[10240] = {0};
        int ret;
        if(p == NULL){
            printf("The os needs to be initialized.\n");
            printf("Init step 1. Type: d for double, others for short.\n");
            scanf("%s", cmd);
            type = (cmd[0] == 'd')?__DS__DOUBLE__:__DS__SHORT__;
			printf("Init step 2. Capacity: \n");
			scanf("%zu", &cap);
            p = osAlloc();
            if(p == NULL)
                ret = __DS__OS__OUT_OF_MEM__;
            else{
                if(type == __DS__DOUBLE__)
                    ret = osInit(p, sizeof(double), cap, doubleGT);
                else
                    ret = osInit(p, sizeof(short), cap, shortGT);
            }
                
            if(ret != __DS__OS__NORMAL__){
                printf("Not enough memory.\n");
                if(p != NULL)
                    osFree(p);
                p = NULL;
            }
            else{
                printf("Initialization done.\n");
            }
		}else{
            int choice = 0;
            printf("size/capacity: %zu/%zu\n", osSize(p), osCap(p));
            printf("Valid operation: 1)insert, 2)delete, 3)find, 4)get lower bound\n");
            printf("                 5)get upper bound, 6)find-K, 7)pre, 8)next\n");
			printf("                 9)get item by it 10)empty, 11)free 12)quit\n");
	
            while(choice <= 0 || choice > 12){
                scanf("%s", cmd);
                sscanf(cmd, "%d", &choice);
            } 
            if(choice == 1){//insert
                printf("input a %s value:", (type == __DS__DOUBLE__)?"double":"short"); 
                getData(type, data);
                ret = osInsert(p, data);
                if(ret==__DS__OS__FULL__)printf("Set is full!\n");
				else if(ret==__DS__OS__OBJ_EXIST__)printf("Element already exists!\n");
				else if(ret==__DS__OS__OUT_OF_MEM__)printf("Run out of memory\n");
            }
            else if(choice == 2){//delete
				printf("input a %s value:", (type == __DS__DOUBLE__)?"double":"short");
				getData(type, data);
                ret = osDelete(p, data);
                if(ret==__DS__OS__OBJ_NOT_EXIST__){
                    printf("Element is not in the set.\n");
                } 
                else
					printf("Delete OK\n");
            }
            else if(choice == 3){//find
                printf("input a %s value:", (type == __DS__DOUBLE__)?"double":"short"); 
                getData(type, data);
                ret = osFindIt(p, data, &it);
                if(ret == __DS__OS__OBJ_EXIST__){
                    printf("The element ");
					printData(type, data);
					printf(" is in the set.\n");
			  	}
			 	else{
					printf("The element ");
					printData(type, data);
					printf(" is not in the set.\n");
			 	}
            } 
            else if(choice == 4){//lower bo und
                printf("input the lower bound:");
                getData(type, data2);
				ret = osLowerIt(p, data2, &it);
        
                 if(ret == __DS__OS__NORMAL__ ){
                    printf("The result is ");
                    printData(type, data);
                    printf(".\n");
                 }
                else
                    printf("No element found\n");
            }
            else if( choice == 5){//upper bound
                printf("input the upper bound:");
                getData(type, data2);
                ret = osUpperIt(p, data2, &it);
                if(ret == __DS__OS__NORMAL__){
                    printf("The result is ");
                    printData(type, data);
                    printf(".\n");
                   }
                else
                    printf("No element found\n");
            }   
			else if(choice == 6){
				int k ;
				printf("input the oder in set:");
				scanf("%zu", &k);
				ret = osFindKthObj(p, k, data);
			 	if(ret == __DS__OS__NORMAL__){
					printf("The result key is ");
					printData( type, data);
					printf(".\n");
			 	}
				else 
					printf("No element found\n");

			}  
			else if(choice == 7) {//pre
				it = osPrevIt(it);
				if(it==NULL)
					printf("No prev element found\n");
				else
					printf("Prev OK!\n");
			}
			else if(choice == 8){//next
				it = osNextIt(it);
				if(it==NULL)
					printf("No next element found\n");
				else
					printf("Next OK!\n");
			} 
			else if(choice == 9){//get item by it
				ret =  osGetByIt(it, data);
				if(ret == __DS__OS__NORMAL__){
					printf("The result is ");
					printData(type, data);
					printf(".\n");
				}else{
					printf("Invalid Iterator\n");
				}
			} 
            else if(choice == 10){//empty
                if(osEmpty(p)==__DS__OS__EMPTY__)
                    printf("The os is empty.\n");
                else
                    printf("The os is not empty.\n");
            }    
            else if(choice == 11){
                osFree(p);
                p = NULL;
            }    
            else if(choice == 12)
                break;
        } 
    } 
}

