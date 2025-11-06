#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include "队列ADT.cpp"
using namespace std;
#define TRUE 1
#define FALSE 0
#define ERROR 0
#define OK 1
#define OVERFLOW -1
typedef int Status;
typedef char TElemType;
#define MAXSIZE 20

typedef struct BiTNode{
    TElemType data;
    BiTNode *lchild,*rchild;
}*BiTree,BiTNode;


//计算二叉树结点z的个数
int LevelSum(BiTree T){
    if (T == NULL) {
        return 0;
    }
    LQueue Q;
    InitQueue_LQ(Q);
    EnQueue_LQ(Q, T);
    int sum = 0;
    ElemType e;
    while (!QueueEmpty_LQ(Q)) {
        DeQueue_LQ(Q, e);
        sum += 1;
        if (e->lchild != NULL) {
            EnQueue_LQ(Q, e->lchild);
        }
        if (e->rchild != NULL) {
            EnQueue_LQ(Q, e->rchild);
        }
    }
    return sum;

}