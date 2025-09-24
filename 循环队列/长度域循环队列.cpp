#include <stdio.h>
#include <stdlib.h>
#define Status int
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
typedef char ElemType;
#define MAXQSIZE 100 //最大队列长度

typedef struct {
    ElemType* elem; //存放栈元素的数组
    int length; //头指针
    int rear; //尾指针
    int maxSize; //队列的最大容量
} CLenQueue;



Status EnCQueue(CLenQueue &Q, ElemType x) { 
    // Add your code here
    if (Q.length == MAXQSIZE) // 队满
        return ERROR;
    if (Q.length == 0) {
        // 队列为空，直接放在0位置
        Q.rear = 0;
        Q.elem[Q.rear] = x;
    } else {
        Q.rear = (Q.rear + 1) % MAXQSIZE;
        Q.elem[Q.rear] = x;
    }
    Q.length++;
    return OK;

}

Status DeCQueue(CLenQueue &Q, ElemType &x){
   // Add your code here
    if(Q.length==0)return ERROR;
    
    int front =(Q.rear - Q.length + MAXQSIZE+1)%MAXQSIZE;

    x = Q.elem[front];
    Q.length--;
    return OK;
}