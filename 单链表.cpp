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
typedef char ElemType;
typedef int KeyType;
#define MAXSIZE 20


typedef struct LNode{
    ElemType data;
    struct LNode *next;
}LNode,*LinkList;

//判空
Status ListEmpty_L(LinkList L) 
{   
    if (L->next == NULL) {
        return TRUE;
    }
    return FALSE;
}

//销毁
Status DestroyList_L(LinkList &L) 
{   
    L = NULL;
	free(L);
	return OK;
}

//清空
Status ClearList_L(LinkList &L)
{   
    if (L == NULL) {
        return ERROR;
    }
    if (L->next == NULL) {
        return OK;
    }
    L->next = NULL;
    return OK;
}

//求长度
int ListLength_L(LinkList L) 
{   
    if(L==NULL) return -1;
    int count=0;
    while (L->next!=NULL) {
        count++;
        L = L->next;
    }
    return count;
}

//插入
Status Insert_L(LinkList L, int i, ElemType e) 
{   
    if (L == NULL || i < 1) {
        return ERROR;
    }
    LNode *p = L;
    int j = 0;
    while (p != NULL && j < i - 1) {
        p = p->next;
        j++;
    }
    if (p == NULL) {
        return ERROR;
    }
    LNode *s = (LNode *)malloc(sizeof(LNode));
    s->data = e;
    s->next = p->next;
    p->next = s;
    return OK;
}

//删除第i个元素
Status Delete_L(LinkList L, int i, ElemType &e) 
{   
    if (L == NULL || i < 1) {
        return ERROR;
    }
    LNode *p = L;
    int j = 0;
    while (p->next != NULL && j < i - 1) {
        p = p->next;
        j++;
    }
    if (p->next == NULL) {
        return ERROR;
    }
    LNode *q = p->next;
    e = q->data;
    p->next = q->next;
    free(q);
    return OK;
}



//删除第i个元素起的所有元素
Status Cut_L(LinkList L, int i)
{   // Add your code here
    if(L==NULL || i<1)return ERROR;

    LNode* p = L;
    int j=0;
    while (p!= NULL && j<i-1) {
        p = p->next;
        j++;
    }

    if(p == NULL)return ERROR;
    //注意只有先判断了p本身是否为空，才能判断p->next
    if (p->next ==NULL)return ERROR;
    p->next = NULL;
    return OK;
}


//删除第i个元素起的所有元素，并将这些元素存入新链表Li
Status Split_L(LinkList L, LinkList &Li, int i){
    if (L == NULL || i < 1) {
        return ERROR;
    }
    Li = (LinkList)malloc(sizeof(LNode)); //新链表的头结点
    Li->next = NULL;
    LNode *p = L;
    int j = 0;
    while (p != NULL && j < i - 1) {
        p = p->next;
        j++;
    }
    if (p == NULL) {
        return ERROR;
    }
    Li->next = p->next;
    p->next = NULL;
    return OK;
}


//删除所有值为x的元素，返回删除个数
Status DeleteX_L(LinkList L, ElemType x)  
{   // Add your code here
    int count=0;
    LinkList p=L;
    LinkList a;
    if(p==NULL) return ERROR;
    while (p->next!=NULL) {
        if(p->next->data ==x){
            count++;
            a = p->next;
            if(p->next->next!=NULL)
                p->next = p->next->next;
            else{
                p->next =NULL;
                return count;
            }
            free(a);
        }
        if(p->next->data != x)
            p = p->next;
    }
    return count;
}