#include<cstdio>
#include<iostream>
#include<cstdlib>
using namespace std;
#define TRUE 1
#define FALSE 0
#define ERROR 0
#define OK 1
#define OVERFLOW -1
typedef int Status;


typedef struct LSNode{
    int data;
    LSNode* next;
}LSNode,*LStack;

//初始化栈
void InitStack(LStack &S){
    S = NULL;
}

//清除栈
void DestroyStack(LStack &S){
    LStack p;
    while(S){
        p=S;
        S=S->next;
        free(p);
    }
}

//判断栈空
Status isStackEmpty(LStack S){
    return S==NULL ? TRUE : FALSE;
}

//入栈
Status Push(LStack &S,int e)
{
    LSNode *t = (LSNode*)malloc(sizeof(LSNode));
    if(t==NULL) exit(OVERFLOW);
    t->data = e;
    t->next = S; S = t;
    return OK;
}

//出栈
Status Pop(LStack &S,int &e){
    if(isStackEmpty(S)) return ERROR;
    LSNode *p = S;//释放原来的栈顶空间
    e = S->data;
    S = S->next;
    free(p);
    return OK;
}

//取栈顶元素
Status GetTop(LStack S,int &e){
    if(isStackEmpty(S)) return ERROR;
    e=S->data;
    return OK;
}

