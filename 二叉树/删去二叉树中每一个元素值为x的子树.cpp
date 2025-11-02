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

typedef struct BiTNode{
    ElemType data;
    struct BiTNode *lchild,*rchild;
}*BiTree,BiTNode;




void ReleaseNode(BiTree &t)
{
    if (!t) return;
    ReleaseNode(t->lchild);
    ReleaseNode(t->rchild);
    free(t);         // 若用的是 malloc/calloc/realloc 分配
    t = NULL;
}
void ReleaseX(BiTree &bt, char x)
{  // Add your code here
    
    BiTree p=NULL;
    if (bt == NULL)
      return;
    if(bt->data==x)
    {
        p = bt;
        bt = NULL;
        ReleaseNode(p);
    }
    if(bt!=NULL)
        ReleaseX(bt->lchild, x);
    if(bt!=NULL)
        ReleaseX(bt->rchild, x);
    return;
}