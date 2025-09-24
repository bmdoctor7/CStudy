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


typedef struct DuLNode{
    ElemType data;
    int length;
    struct DuLNode *prior;
    struct DuLNode *next;
}DuLNode,*DuLinkList;


//返回无重复结点双向链表
DuLinkList delDuplicateDuLNodes(DuLinkList head){
    if (!head) return NULL;          // 传入非法
    DuLinkList outer = head->next;   // 第一个真实数据结点(待比较的点/基准点)
    while (outer) {
        DuLinkList inner = outer->next;
        //往后找重复结点
        while (inner) {
            DuLinkList nextInner = inner->next; // 先缓存，防止被删后丢失
            if (inner->data == outer->data) {
                // 删除 inner
                if (inner->prior) inner->prior->next = inner->next; // 修改前驱的后置
                if (inner->next)  inner->next->prior = inner->prior; // 修改后继的前驱
                free(inner);  // 或 delete inner; 视创建方式
            }
            inner = nextInner;
        }
        outer = outer->next;
    }
    return head;
}

//将双向链表逆置
void reverseDuLinkList(DuLinkList &head) {
    if (!head || !head->next) return; // 空链表或只有头结点

    DuLinkList current = head->next; // 第一个真实数据结点
    DuLinkList prev = NULL;           // 前驱结点初始化为 NULL

    while (current) {
        DuLinkList nextNode = current->next; // 先缓存后继结点
        current->next = prev;                 // 反转当前结点的 next 指针
        current->prior = nextNode;            // 反转当前结点的 prior 指针
        prev = current;                       // 移动前驱指针到当前结点
        current = nextNode;                   // 移动到下一个结点
    }

    head->next = prev; // 更新头结点的 next 指向新的第一个结点
    if (prev) prev->prior = head; // 新的第一个结点的 prior 指向头结点
}

//将双向循环链表逆置
void reverseDuCirLinkList(DuLinkList &head) {
    if (!head || !head->next || head->next == head) return; // 空链表或只有头结点或只有一个数据结点

    DuLinkList current = head->next; // 第一个真实数据结点
    DuLinkList prev = head;           // 前驱结点初始化为头结点
    DuLinkList first = current;       // 记录第一个数据结点，最后需要连接

    do {
        DuLinkList nextNode = current->next; // 先缓存后继结点
        current->next = prev;                 // 反转当前结点的 next 指针
        current->prior = nextNode;            // 反转当前结点的 prior 指针
        prev = current;                       // 移动前驱指针到当前结点
        current = nextNode;                   // 移动到下一个结点
    } while (current != head);               // 循环直到回到头结点

    head->next = prev;          // 更新头结点的 next 指向新的第一个结点
    first->prior = head;        // 原第一个结点的 prior 指向头结点
    head->prior = first;        // 头结点的 prior 指向新的最后一个结点
}