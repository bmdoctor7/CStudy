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


typedef struct PTNode{
    TElemType data;
    int parent;
}PTNode;

typedef struct{
    PTNode *nodes;
    int r,nodeNum; //根的位置和当前结点数
}PTree;

// 计算双亲表示法树的深度
int PTreeDepth(PTree T) 
{   // Add your code here

    if (T.nodeNum == 0) { // 空树深度为0
        return 0;
    }
    int maxDepth = 0;
    for (int i = 0; i < T.nodeNum; i++) { // 遍历每个节点
        int currentDepth = 1;
        int parent = T.nodes[i].parent;
        while (parent != -1) { // 沿双亲链向上回溯，直到根节点
            currentDepth++;
            parent = T.nodes[parent].parent;
        }
        if (currentDepth > maxDepth) { // 更新最大深度
            maxDepth = currentDepth;
        }
    }
    return maxDepth;
}







