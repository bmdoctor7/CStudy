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




// 计算二叉树中度为1/2的结点个数
void Degree1(BiTree T, int &count) {
    if (T == NULL) {
        return;
    }
    if ((T->lchild != NULL && T->rchild == NULL) ||
        (T->lchild == NULL && T->rchild != NULL)) {
        count++; // 当前结点度为1
    }
    Degree1(T->lchild, count); // 递归遍历左子树
    Degree1(T->rchild, count); // 递归遍历右子树
}
void Degree2(BiTree T, int &count) {
    if (T == NULL) {
        return;
    }
    if (T->lchild != NULL && T->rchild != NULL) {
        count++; // 当前结点度为2
    }
    Degree2(T->lchild, count); // 递归遍历左子树
    Degree2(T->rchild, count); // 递归遍历右子树
}
//分支结点总数
int BranchNodes(BiTree T) {
    int count = 0;
    Degree1(T, count);
    Degree2(T, count);
    return count;
}


// 在二叉树 T 中查找是否存在值为 x 的结点
Status SearchX(BiTree T, TElemType x){
    if (T == NULL) {
        return FALSE;
    }
    if (T->data == x) {
        return TRUE;
    }
    return SearchX(T->lchild, x) || SearchX(T->rchild, x);
}
// 在二叉树 T 中定位值为 x 的结点，返回该结点指针
Status LocateX(BiTree T, TElemType x, BiTree &result) {
    if (T == NULL) {
        result = NULL;
        return FALSE;
    }
    if (T->data == x) {
        result = T;
        return TRUE;
    }
    return LocateX(T->rchild, x, result) || LocateX(T->lchild, x, result);
}


// 统计以 t 为根的子树结点总数（与结点值无关）
int CountAll(BiTree t) {
    if (t == NULL) return 0;
    return 1 + CountAll(t->lchild) + CountAll(t->rchild);
}
//求以x结点为根的子树的结点总数
int xSum(BiTree T, TElemType x) {
    if (T == NULL) return 0;

    if (T->data == x) {
        return CountAll(T);           // 找到后改为纯计数
    }
    int leftResult = xSum(T->lchild, x);
    if (leftResult != 0) return leftResult;  // 左边找到了就直接返回
    return xSum(T->rchild, x);               // 否则去右边找
}


//返回值为x的结点所在的层次，无x则返回-1
int NodeLevel(BiTree T, TElemType x){
    if (T == NULL) {
        return -1; // 树为空，返回-1表示未找到
    }
    if (T->data == x) {
        return 1; // 找到结点，返回层次1
    }
    int level;
    level = NodeLevel(T->lchild, x); // 在左子树中查找
    if (level != -1) {
        return level + 1; // 在左子树中找到，层次加1返回
    }
    level = NodeLevel(T->rchild, x); // 在右子树中查找
    if (level != -1) {
        return level + 1; // 在右子树中找到，层次加1返回
    }
    return -1; // 未找到结点，返回-1
}

//返回值为data的结点的双亲结点的data值，若无则返回NULL
TElemType findParent(BiTree T, TElemType data, BiTNode* parent = NULL) {
    if (T == NULL) {
        return '\0'; // 树为空，返回NULL表示未找到
    }
    if (T->data == data) {
        return parent ? parent->data : '\0'; // 找到结点，返回父结点的data值
    }
    TElemType leftResult = findParent(T->lchild, data, T); // 在左子树中查找
    if (leftResult != '\0') {
        return leftResult; // 在左子树中找到，直接返回
    }
    return findParent(T->rchild, data, T); // 在右子树中查找
}

//将二叉树中所有结点的左右字数交换
void ExchangeSubTree(BiTree &T){
    if (T == NULL) {
        return;
    }
    // 交换当前结点的左右子树
    BiTree temp = T->lchild;
    T->lchild = T->rchild;
    T->rchild = temp;

    // 递归交换左子树和右子树
    ExchangeSubTree(T->lchild);
    ExchangeSubTree(T->rchild);
}
