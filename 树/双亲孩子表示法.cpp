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


typedef struct ChildNode{
    int childIndex;
    struct ChildNode* nextChild;
}ChildNode; //孩子结点

typedef struct {
    TElemType data;
    int parent;
    ChildNode* firstChild; //指向第一个孩子结点
}PCTreeNode;

typedef struct {
    PCTreeNode *nodes;
    int r, n; //根的位置和当前结点数
}PCree;


// 计算孩子表示法树的深度
int PCTreeDepth(PCree T) 
{   // Add your code here

    if(T.n==0) return 0;
    if(T.nodes == NULL)return 0;

    int maxDepth = 0;
    
    // 遍历所有节点
    for (int i = 0; i < T.n; i++) {
        // 检查是否为叶节点
        if (T.nodes[i].firstChild == NULL) {
            // 当前叶节点的深度
            int depth = 0;
            int current = i;
            
            // 向上遍历到根节点
            while (current != -1) { 
                depth++;
                current = T.nodes[current].parent;
            }
            
       
            if (depth > maxDepth) {
                maxDepth = depth;
            }
        }
    }
    
    return maxDepth;
}
