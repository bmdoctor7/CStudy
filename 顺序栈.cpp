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

typedef char ElemType;
typedef struct {
    ElemType* elem; //存放栈元素的数组
    ElemType* top; //栈顶元素的下一个位置
    int size; //栈的当前容量
    int increment; //栈空间分配增量
} SqStack2;

//初始化栈
Status InitStack_Sq2(SqStack2 &S, int size, int inc) { 
    if(size<=0 || inc<=0) return ERROR;
    S.size = size;
    S.increment = inc;
    S.elem = (ElemType*)malloc(size*sizeof(ElemType));
    S.top = S.elem;
    return OK;
}

//判断栈空
Status StackEmpty_Sq2(SqStack2 S) { 
    return S.top<=S.elem;
}

//入栈
Status Push_Sq2(SqStack2 &S, ElemType e) { 
    ElemType *p;
    if(S.top>=S.elem+S.size){
        p=(ElemType *)realloc(S.elem,(S.size+S.increment)*sizeof(ElemType));
        if(!p) return ERROR;
        S.elem = p;
        S.top = S.elem+S.size;
        S.size+=S.increment;
    }
    *S.top++=e;
    return OK;
}
//出栈
Status Pop_Sq2(SqStack2 &S, ElemType &e) { 
    if (S.top<=S.elem) {
        return ERROR;
    }
    e=*(--S.top);
    return OK;
}