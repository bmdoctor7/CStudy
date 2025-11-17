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
#define MAXSIZE 20


typedef struct{
    int *parent; //双亲位置表
    int n;
}MFSet;

//递归查找
int Find(MFSet S, int x) 
{ 
    if (S.parent[x] >= S.n) {
        return -1; // 无效输入
    }
    if (S.parent[x] == x) {// 根节点指向自身的情况
    //  S.parent[x] < 0 也可以视为根节点
        return x; // x 是根
    }
    // 递归查找根，并进行路径压缩
    S.parent[x] = Find(S, S.parent[x]);
    return S.parent[x];
}

//非递归查找
int FindNonRec(MFSet S, int x) 
{ 
    if (S.parent[x] >= S.n) {
        return -1; // 无效输入
    }
    int root = x;

    // 找到根节点（不断向上查找）
    while (S.parent[root] != root) {
    //     S.parent[root] != -1 也可以视为判断条件
        root = S.parent[root];
    }
    // 路径压缩（将路径上的所有节点直接连接到根节点）
    int current = x;
    while (current != root) {
        int parent = S.parent[current];
        S.parent[current] = root;
        current = parent;
    }
    return root;
}


void Union(MFSet &S, int x, int y) 
{ 
    int rootX = Find(S, x);
    int rootY = Find(S, y);
    //按树的大小合并
    if (rootX != rootY) {
        if(S.parent[rootX]<S.parent[rootY]){ // 越小代表树越大（负数）
            S.parent[rootX] += S.parent[rootY];
            S.parent[rootY] = rootX;
        } else {
            S.parent[rootY] += S.parent[rootX];
            S.parent[rootX] = rootY;
        }
    }

    // //按树的高度合并
    // if(rootX != rootY) {
    //     if(S.parent[rootX] < S.parent[rootY]) { // 越小代表树越高（负数）
    //         S.parent[rootY] = rootX;
    //     } else if(S.parent[rootX] > S.parent[rootY]) {
    //         S.parent[rootX] = rootY;
    //     }else {
    //         S.parent[rootX] = rootY; // 任意选择一个作为新根
    //         S.parent[rootY]--; // 增加新根的高度
    //     }
    // }
    
}


