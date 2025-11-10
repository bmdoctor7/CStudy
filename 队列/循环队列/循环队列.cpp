#include <stdio.h>
#include <stdlib.h>
#define Status int
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
typedef char ElemType;

typedef struct {
    ElemType* elem; //存放栈元素的数组
    int front; //头指针
    int rear; //尾指针
    int maxSize; //队列的最大容量
} SqQueue;



int QueueLength_Sq(SqQueue Q) { 
    return (Q.rear-Q.front+Q.maxSize)%Q.maxSize;//+max再%max保证非负
}