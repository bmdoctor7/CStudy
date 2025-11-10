#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include "队列/队列ADT.cpp"
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


//层次遍历二叉树
void LevelOrder(BiTree bt, char* ss){
    if (bt == NULL) {
        return;
    }
    LQueue Q;
    InitQueue_LQ(Q);
    EnQueue_LQ(Q, bt);
    int index = 0;
    ElemType e;
    while (!QueueEmpty_LQ(Q)) {
        DeQueue_LQ(Q, e);
        ss[index++] = e->data; //访问结点
        if (e->lchild != NULL) {
            EnQueue_LQ(Q, e->lchild);
        }
        if (e->rchild != NULL) {
            EnQueue_LQ(Q, e->rchild);
        }
    }
    ss[index] = '\0'; //字符串结束标志
}



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

//判别给定二叉树是否为完全二叉树
Status CompleteBiTree(BiTree bt){
    if (bt == NULL) return TRUE; // 空树是完全二叉树
    
    LQueue Q;
    InitQueue_LQ(Q); // 初始化队列
    
    EnQueue_LQ(Q, bt); // 根节点入队
    int meetIncomplete = 0; // 标记是否遇到第一个不完整的节点
    
    while (!QueueEmpty_LQ(Q)) {
        BiTree current;
        DeQueue_LQ(Q, current); // 出队当前节点
        
        // 检查左孩子
        if (current->lchild != NULL) {
            // 如果之前已经遇到过不完整的节点，但当前节点有左孩子，则不是完全二叉树
            if (meetIncomplete) return FALSE;
            EnQueue_LQ(Q, current->lchild);
        } else {
            // 遇到第一个没有左孩子的节点
            meetIncomplete = 1;
        }
        
        // 检查右孩子
        if (current->rchild != NULL) {
            // 如果左孩子为空但右孩子不为空，或者之前已经遇到过不完整的节点，则不是完全二叉树
            if (current->lchild == NULL || meetIncomplete) return FALSE;
            EnQueue_LQ(Q, current->rchild);
        } else {
            // 遇到第一个没有右孩子的节点
            meetIncomplete = 1;
        }
    }
    return TRUE;
}

//复制一颗二叉树(非递归)
void CopyBiTree(BiTree T, BiTree &TT){
    if (T == NULL) {
        TT = NULL;
        return;
    }
    LQueue Q1, Q2;
    InitQueue_LQ(Q1);
    InitQueue_LQ(Q2);
    
    TT = (BiTree)malloc(sizeof(BiTNode));
    TT->data = T->data;
    TT->lchild = NULL;
    TT->rchild = NULL;
    
    EnQueue_LQ(Q1, T);
    EnQueue_LQ(Q2, TT);
    
    ElemType e1, e2;
    while (!QueueEmpty_LQ(Q1)) {
        DeQueue_LQ(Q1, e1);
        DeQueue_LQ(Q2, e2);
        
        // 复制左子树
        if (e1->lchild != NULL) {
            BiTree leftChild = (BiTree)malloc(sizeof(BiTNode));
            leftChild->data = e1->lchild->data;
            leftChild->lchild = NULL;
            leftChild->rchild = NULL;
            e2->lchild = leftChild;
            
            EnQueue_LQ(Q1, e1->lchild);
            EnQueue_LQ(Q2, leftChild);
        }
        
        // 复制右子树
        if (e1->rchild != NULL) {
            BiTree rightChild = (BiTree)malloc(sizeof(BiTNode));
            rightChild->data = e1->rchild->data;
            rightChild->lchild = NULL;
            rightChild->rchild = NULL;
            e2->rchild = rightChild;
            
            EnQueue_LQ(Q1, e1->rchild);
            EnQueue_LQ(Q2, rightChild);
        }
    }


}
//复制一颗二叉树(递归)
void CopyBiTreeRecursive(BiTree T, BiTree &TT){
    if (T == NULL) {
        TT = NULL;
    } else {
        TT = (BiTree)malloc(sizeof(BiTNode));
        TT->data = T->data;
        CopyBiTreeRecursive(T->lchild, TT->lchild);
        CopyBiTreeRecursive(T->rchild, TT->rchild);
    }
}
