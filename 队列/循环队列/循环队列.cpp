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


Status InitQueue(SqQueue &Q, int maxSize) { 
    Q.elem = (ElemType*)malloc(maxSize * sizeof(ElemType));
    if (!Q.elem) return ERROR;
    Q.front = 0;
    Q.rear = 0;
    Q.maxSize = maxSize;
    return OK;
}

int QueueLength_Sq(SqQueue Q) { 
    return (Q.rear-Q.front+Q.maxSize)%Q.maxSize;//+max再%max保证非负
}

//入队
Status EnQueue(SqQueue &Q, ElemType x) { 
    if ((Q.rear + 1) % Q.maxSize == Q.front) // 队满
        return ERROR;
    Q.elem[Q.rear] = x;
    Q.rear = (Q.rear + 1) % Q.maxSize;
    return OK;
}

//出队
Status DeQueue(SqQueue &Q, ElemType &x) { 
    if (Q.front == Q.rear) // 队空
        return ERROR;
    x = Q.elem[Q.front];
    Q.front = (Q.front + 1) % Q.maxSize;
    return OK;
}