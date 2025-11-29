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

typedef struct {
    ElemType* elem; 
    int length;
    int size;
    int increment;
} SqList;
typedef struct BiTNode{
    ElemType data;
    struct BiTNode *lchild,*rchild;
}*BiTree,BiTNode;

typedef enum{ATOM,LIST} ElemTag;
typedef struct GLNode{
    ElemTag tag;
    union{
        ElemType atom;
        struct{
            struct GLNode* hp;
            struct GLNode* tp;
        }ptr;
    }un;
}*GList;

int GListDepth(GList ls) 
{ 
    if (!ls) return 1;
	if (ls->tag == ATOM)return 0;

    return GListDepth(ls->un.ptr.hp) + 1 > GListDepth(ls->un.ptr.tp) 
    ? GListDepth(ls->un.ptr.hp) + 1 
    : GListDepth(ls->un.ptr.tp);
}


