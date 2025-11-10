#include<cstdio>
#include<iostream>
using namespace std;
#define TRUE 1
#define FALSE 0
#define ERROR 0
#define OK 1
#define OVERFLOW -1
typedef int Status;


typedef struct LQNode{
    int data;
    LQNode* next;
}LQNode,*LQueuePtr;

typedef struct{
    LQueuePtr front;
    LQueuePtr rear;
}LQueue;



//初始化队列
void InitQueue(LQueue &Q){
    Q.front=Q.rear=NULL;
}
//清除队列
void DestroyQueue(LQueue &Q){
    LQNode *p;
    while(Q.rear){
        p=Q.front;
        Q.rear=Q.rear->next;
        free(p);
    }
}
//判断队列空
Status isQueueEmpty(LQueue Q){
    return Q.front==NULL ? TRUE : FALSE;
}

//入队
Status EnQueue(LQueue &Q,int e){
    LQNode *t=(LQNode*)malloc(sizeof(LQNode));
    if(t==NULL) exit(OVERFLOW);
    t->data=e; t->next=NULL;
    if(Q.front==NULL) Q.front=t;
    else Q.rear->next=t;
    Q.rear=t;
    return OK;
}


int QueueLength_LQ(LQueue Q){
    int len=0;
    LQNode *p=Q.front;
    while(p){
        len++;
        p=p->next;
    }
    return len;
}

/*
//循环链队列
//关键：rear->next == head;  循环链表！

//初始化
Status InitCLQueue(CLQueue &rear) 
{   
    rear = (CLQueue)malloc (sizeof(*rear));
     rear ->next = rear; //头结点和尾节点指向相同
} 

//入队
Status EnCLQueue(CLQueue &rear, ElemType x)
{   

    CLQueue tmp = (CLQueue)malloc (sizeof(CLQNode));
    if(!tmp) return ERROR;
    
    tmp->data = x;

    CLQueue head = rear->next;
    //插入：rear -> tmp -> head
    tmp->next = head;
    rear -> next = tmp;
    rear = tmp; //tmp为新的尾节点
    return OK;

}

//出队
Status DeCLQueue(CLQueue &rear, ElemType &x)
{    

    if(rear->next == rear) return ERROR; //队列为空（头尾指向相同）

    CLQueue head = rear->next;

    CLQueue first = head->next; // 首元素结点
    x = first->data;
    head->next = first->next;

    if (first == rear) {
        // 删除的是唯一的那个元素结点，此时恢复为空队列：rear 回到头结点
        rear = head;
    }
    free(first);
    return OK;
}

*/