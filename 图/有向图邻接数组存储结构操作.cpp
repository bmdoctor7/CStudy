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

typedef int VRType;
typedef char InfoType;
typedef char VexType;
typedef enum {DG, DN, UDG, UDN} GraphKind;
//有向图，有向网，无向图，无向网

typedef struct {
    VRType adj; //顶点关系类型
    InfoType* info; //该弧相关信息指针
}ArcCell;

typedef struct{
    ArcCell arcs[MAXSIZE][MAXSIZE]; //关系数组
    VexType vexs[MAXSIZE];  //图的顶点数组
    int n,e;  //图的当前顶点数和弧数
    GraphKind kind; //图的种类
}MGraph; // 邻接数组类型

typedef struct{
    VexType v,w;
    int inf;
}ArcInfo; //边的信息



Status InitGraph(MGraph &G,GraphKind kind, int n)
{
    G.kind = kind;
    G.n = n;
    G.e = 0;
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
        {
            if(i==j)
                G.arcs[i][j].adj = 0;
            else
            {
                if(kind==UDG || kind==DG)
                    G.arcs[i][j].adj = 0; //无边
                else
                    G.arcs[i][j].adj = INFINITY; //无边
            }
            G.arcs[i][j].info = NULL;
        }
    return OK;
}

int LocateVex(MGraph G, VexType v)
{
    for(int i=0;i<G.n;i++)
        if(G.vexs[i]==v)
            return i;
    return -1;
}

//构造有向图G
Status CreateDG(MGraph &G, VexType *vexs, int n,ArcInfo *arcs, int e) 
{  
    InitGraph(G,DG,n);
    G.n = n;
    G.e = e;
    for(int i=0;i<n;i++)
        G.vexs[i] = vexs[i];
    for(int k=0;k<e;k++)
    {
        int i = arcs[k].v - 65;
        int j = arcs[k].w - 65;
        G.arcs[i][j].adj = 1; //有边
    }
    return OK;
}

//求相对于顶点k的当前邻接m顶点的下一个邻接顶点的位置
int NextAdjVex(MGraph G, int k, int m) 
{   // Add your code here
    if (k < 0 || k >= G.n || m < 0 || m >= G.n) {
        return -1; // 顶点位置不合法
    }
    
    // 从m+1位置开始查找k的下一个邻接顶点
    for (int j = m + 1; j < G.n; j++) {
        // 根据图类型判断邻接关系
        switch (G.kind) {
            case DG: // 有向图
            case UDG: // 无向图
                if (G.arcs[k][j].adj != 0) {
                    return j;
                }
                break;
                
            case DN: // 有向网
            case UDN: // 无向网
                if (G.arcs[k][j].adj != INFINITY) {
                    return j;
                }
                break;
        }
    }
    
    return -1; // 没有找到下一个邻接顶点
}


//在图G中设置弧<v,w>的信息
Status SetArc(MGraph &G, VexType v, VexType w, ArcCell info) 
{  
    int i = v-65;
    int j = w-65;
    if (G.vexs[i] == -1 || G.vexs[j] == -1) {
        return ERROR; // 顶点不存在
    }
    //检查弧是否自环
    if (i == j) {
        return ERROR;
    }

    //从v到w的弧
    G.arcs[i][j] = info;
    
    // 如果是无向图或无向网，还需要设置对称的边
    if (G.kind == UDG || G.kind == UDN) {
        G.arcs[j][i] = info;
    }
    
    return OK;
}










