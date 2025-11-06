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
typedef char TElemType;
typedef int KeyType;
#define MAXSIZE 20

typedef struct {
    BiTNode* ptr; // 指向树结点的指针
    int tag;       // 访问标志：0 表示左子树未访问，1 表示左子树已访问
} SElemType;

typedef struct BiTNode{
    TElemType data;
    BiTNode *lchild,*rchild;
}*BiTree,BiTNode;

typedef struct {
    SElemType* elem; //存放栈元素的数组
    SElemType* top; //栈顶元素的下一个位置
    int size; //栈的当前容量
    int increment; //栈空间分配增量
} SqStack2;


//初始化栈
Status InitStack_Sq2(SqStack2 &S) { 
    S.size = 20;
    S.increment = 10;
    S.elem = (SElemType*)malloc(S.size*sizeof(SElemType));
    S.top = S.elem;
    return OK;
}
//判断栈空
Status StackEmpty_Sq2(SqStack2 S) { 
    return S.top<=S.elem;
}
//入栈
Status Push_Sq2(SqStack2 &S, SElemType e) { 
    SElemType *p;
    if(S.top>=S.elem+S.size){
        p=(SElemType *)realloc(S.elem,(S.size+S.increment)*sizeof(SElemType));
        if(!p) return ERROR;
        S.elem = p;
        S.top = S.elem+S.size;
        S.size+=S.increment;
    }
    *S.top++=e;
    return OK;
}
//出栈
Status Pop_Sq2(SqStack2 &S, SElemType &e) { 
    if (S.top<=S.elem) {
        return ERROR;
    }
    e=*(--S.top);
    return OK;
}
//取栈顶元素
Status GetTop_Sq2(SqStack2 S, SElemType &e) { 
    if (S.top<=S.elem) {
        return ERROR;
    }
    e=*(S.top-1);
    return OK;
}




// 非递归：找到从 root 到值为 target 的一条路径，保存在 path 中
// path 结果：栈底 = root，栈顶 = target
static Status FindPath_Sq(BiTree root, TElemType target, SqStack2 &path) {
    SqStack2 S, T;
    InitStack_Sq2(S);
    InitStack_Sq2(T);
    InitStack_Sq2(path);

    BiTree p = root;
    while (p || !StackEmpty_Sq2(S)) {
        while (p) {                        // 沿左路入栈
            SElemType e; e.ptr = p; e.tag = 0;
            Push_Sq2(S, e);
            p = p->lchild;
        }

        //最左下角结点
        SElemType top;
        GetTop_Sq2(S, top);

        if (top.tag == 0) {                // 首次到该结点
            Pop_Sq2(S, top);
            top.tag = 1;
            Push_Sq2(S, top);

            if (top.ptr->data == target) { // 命中：复制 S 为 path（保持 root 在底、target 在顶）
                SElemType x;
                while (!StackEmpty_Sq2(S)) { Pop_Sq2(S, x); Push_Sq2(T, x); }
                while (!StackEmpty_Sq2(T)) { Pop_Sq2(T, x); Push_Sq2(path, x); }
                return TRUE;
            }
            p = top.ptr->rchild;           // 转向右子树
        } else {                           // 回溯
            Pop_Sq2(S, top);
            p = NULL;
        }
    }
    return FALSE;                          // 未找到
}

// 根据两条从 root 到各结点的路径求“最近公共祖先（不含结点自身）”
// 输入 path1/path2：栈底 = root，栈顶 = 目标
static BiTree LCA_FromTwoPaths(SqStack2 &path1, SqStack2 &path2) {
    // 翻转到 r1/r2：栈顶 = root，向下依次到目标
    SqStack2 r1, r2;
    InitStack_Sq2(r1);
    InitStack_Sq2(r2);

    SElemType e;
    while (!StackEmpty_Sq2(path1)) { Pop_Sq2(path1, e); Push_Sq2(r1, e); }
    while (!StackEmpty_Sq2(path2)) { Pop_Sq2(path2, e); Push_Sq2(r2, e); }

    BiTree prevSame = NULL;                // 上一个公共祖先
    BiTree lastSame = NULL;                // 当前公共祖先
    SElemType a, b;

    while (!StackEmpty_Sq2(r1) && !StackEmpty_Sq2(r2)) {
        GetTop_Sq2(r1, a);
        GetTop_Sq2(r2, b);
        if (a.ptr == b.ptr) {
            prevSame = lastSame;
            lastSame = a.ptr;
            Pop_Sq2(r1, a);
            Pop_Sq2(r2, b);
        } else {
            break;
        }
    }

    // 若其中一条路径已比对完，说明 lastSame 恰是某个目标自身
    // 题意需要“真祖先”，返回其上一层公共祖先；若不存在则返回 NULL
    if (StackEmpty_Sq2(r1) || StackEmpty_Sq2(r2)) {
        return prevSame;                   // 可能为 NULL（显示成 ‘#’）
    }
    return lastSame;                       // 正常情形的最近公共（真）祖先
}

// 题目所需主函数：非递归 + 顺序栈接口；若任一目标不存在则返回 NULL
BiTree CommAncestor(BiTree root, TElemType c1, TElemType c2) {
    if (!root) return NULL;

    SqStack2 p1, p2;
    if (FindPath_Sq(root, c1, p1) == FALSE) return NULL;
    if (FindPath_Sq(root, c2, p2) == FALSE) return NULL;

    return LCA_FromTwoPaths(p1, p2);
}