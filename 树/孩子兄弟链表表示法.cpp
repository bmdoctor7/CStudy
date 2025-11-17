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
typedef int KeyType;
#define MAXSIZE 20

typedef struct CSTNode{
    TElemType data;
    struct CSTNode* firstChild; //指向第一个孩子结点
    struct CSTNode* nextSibling; //指向下一个兄弟结点
}CSTNode, *CSTree;

// 计算树的度
int Degree(CSTree T) 
{   
    if (T == NULL) {
        return 0;  // 空树度为0
    }
    // 当前节点的度
    int currentDegree = 0;
    CSTNode *child = T->firstChild;
    while (child != NULL) {
        currentDegree++;
        child = child->nextSibling;
    }
    
    // 第一个孩子的度
    int childDegree = Degree(T->firstChild);
    
    // 兄弟树的度
    int siblingDegree = Degree(T->nextSibling);
    
    // 三者中的最大值
    int maxDegree = currentDegree;
    if (childDegree > maxDegree) {
        maxDegree = childDegree;
    }
    if (siblingDegree > maxDegree) {
        maxDegree = siblingDegree;
    }
    
    return maxDegree;
}

// 计算树的深度
int TreeDepth(CSTree T) 
{   
    if (T == NULL) {
        return 0;  // 空树深度为0
    }
    
    int maxChildDepth = 0;
    CSTNode *child = T->firstChild;
    while (child != NULL) {
        int childDepth = TreeDepth(child);
        if (childDepth > maxChildDepth) {
            maxChildDepth = childDepth;
        }
        child = child->nextSibling;
    }
    
    return maxChildDepth + 1; // 当前节点深度为子树最大深度加1
}





