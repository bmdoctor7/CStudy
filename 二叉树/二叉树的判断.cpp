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
typedef char TElemType;
#define MAXSIZE 20

typedef struct BiTNode{
    TElemType data;
    BiTNode *lchild,*rchild;
}*BiTree,BiTNode;

// 判断两棵二叉树是否相等
Status BTEqual(BiTree T1, BiTree T2) {
    if (T1 == NULL && T2 == NULL) {
        return TRUE; // 两棵树都为空，视为相等
    }
    if (T1 == NULL || T2 == NULL) {
        return FALSE; // 一棵树为空，另一棵不为空，视为不相等
    }
    // 比较当前结点的值，并递归比较左右子树
    return (T1->data == T2->data) &&
           BTEqual(T1->lchild, T2->lchild) &&
           BTEqual(T1->rchild, T2->rchild);
}

//判别给定二叉树是否为小根二叉树
Status SmallBiTree(BiTree T){
    if (T == NULL) {
        return TRUE; // 空树视为小根二叉树
    }
    if (T->lchild != NULL) {
        if (T->lchild->data < T->data) {
            return FALSE; // 左子结点值小于父结点值，不满足小根堆性质
        }
    }
    if (T->rchild != NULL) {
        if (T->rchild->data < T->data) {
            return FALSE; // 右子结点值小于父结点值，不满足小根堆性质
        }
    }
    // 递归检查左子树和右子树
    return SmallBiTree(T->lchild) && SmallBiTree(T->rchild);
}

//判别给定二叉树是否为正则二叉树
Status RegularBiTree(BiTree T){
    if (T == NULL) {
        return TRUE; // 空树视为正则二叉树
    }
    if ((T->lchild == NULL && T->rchild != NULL) ||
        (T->lchild != NULL && T->rchild == NULL)) {
        return FALSE; // 只有一个子结点，不满足正则二叉树性质
    }
    // 递归检查左子树和右子树
    return RegularBiTree(T->lchild) && RegularBiTree(T->rchild);
}
















