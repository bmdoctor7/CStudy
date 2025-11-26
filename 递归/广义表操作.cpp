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


typedef enum{ATOM,LIST} ElemTag;
typedef struct GLNode{
    ElemTag tag;
    union{
        ElemType atom;
        struct{
            struct GLNode* hp;//表头
            struct GLNode* tp;//表尾
        }ptr;
    }un;
}*GList;

//求广义表深度
int GListDepth(GList ls) 
{ 
    if (!ls) return 1;//空表深度为1
	if (ls->tag == ATOM)return 0;//原子深度为0


    //取表头和表尾中较大的深度
    return GListDepth(ls->un.ptr.hp) + 1 > GListDepth(ls->un.ptr.tp) 
    ? GListDepth(ls->un.ptr.hp) + 1 
    : GListDepth(ls->un.ptr.tp);
}

//判断两个广义表是否相等
Status Equal(GList A, GList B) 
{  
    //两个空表相等
    if (!A && !B) return TRUE;
    
    //都不为空且类型相同
    else if (A && B && A->tag == B->tag) {

        //都是原子则比较值
        if (!A->tag && !B->tag)  return A->un.atom == B->un.atom;

        //都是表则递归比较表头和表尾
        if (Equal(A->un.ptr.hp, B->un.ptr.hp) && Equal(A->un.ptr.tp, B->un.ptr.tp))return 1;
    }
    else return FALSE;
}

void OutAtom(GList A, int layer, void(*Out2)(char, int)) 
{ // Add your code here
    //空表直接返回
    if (!A) {
        return;
    }
    //原子直接输出
    if (!A->tag) {
        Out2(A->un.atom, layer);
        return;
    }
    //有子表则递归进入子表
    if (A->un.ptr.hp) {
        OutAtom(A->un.ptr.hp, layer + 1, Out2);
    }
    //尾表不为空则递归继续输出
    if (A->un.ptr.tp) {
        OutAtom(A->un.ptr.tp, layer, Out2);
    }
}