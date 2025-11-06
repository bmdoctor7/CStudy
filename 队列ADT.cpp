#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <math.h>
using namespace std;
#define TRUE 1
#define FALSE 0
#define ERROR 0
#define OK 1
#define OVERFLOW -1
typedef int Status;
typedef BiTree ElemType;
typedef char TElemType;
typedef int KeyType;
#define MAXSIZE 20


typedef struct BiTNode{
    TElemType data;
    BiTNode *lchild,*rchild;
}*BiTree,BiTNode;

typedef struct LQNode{
    ElemType data;
    struct LQNode *next;
}*QueuePtr, LQNode;

typedef struct {
    QueuePtr front; //队头指针
    QueuePtr rear; //队尾指针
} LQueue;

//初始化链队列
Status InitQueue_LQ(LQueue &Q) {
    Q.front = Q.rear = (QueuePtr)malloc(sizeof(LQNode));
    if (!Q.front) exit(OVERFLOW);
    Q.front->next = NULL;
    return OK;
}

//判断队列空
Status QueueEmpty_LQ(LQueue Q) {
    return Q.front == Q.rear;
}

//插入元素到队尾（入队）
Status EnQueue_LQ(LQueue &Q, ElemType e) {
    QueuePtr p = (QueuePtr)malloc(sizeof(LQNode));
    if (!p) exit(OVERFLOW);
    p->data = e;
    p->next = NULL;
    Q.rear->next = p;
    Q.rear = p;
    return OK;
}

//删除队头元素（出队）
Status DeQueue_LQ(LQueue &Q, ElemType &e) {
    if (Q.front == Q.rear) {
        return ERROR; //队列空
    }
    QueuePtr p = Q.front->next;
    e = p->data;
    Q.front->next = p->next;
    if (Q.rear == p) { //删除的是队尾元素
        Q.rear = Q.front;
    }
    free(p);
    return OK;
}

//获取队头元素
Status GetHead_LQ(LQueue Q, ElemType &e) {
    if (Q.front == Q.rear) {
        return ERROR; //队列空
    }
    QueuePtr p = Q.front->next;
    e = p->data;
    return OK;
}












