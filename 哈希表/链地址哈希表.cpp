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

typedef char HKeyType;
typedef struct HNode{
    HKeyType data;
    struct HNode *next;
}*HLink;
typedef struct{
    HLink *rcd;
    int size;   // 哈希表的最大长度
    int count;  // 当前关键字个数
}ChainHashTab;

int Hash(ChainHashTab H, HKeyType k) {
    return (k - 'A') % H.size; // 简单的除留余数法
}

Status Collision (ChainHashTab H, HLink &p){
    //求得下一个探查地址p
    if(p && p->next){
        p = p->next;
        return OK;
    }
    else return ERROR;
}

int BuildHashTab(ChainHashTab &H, int n, HKeyType es[]){
    H.count = 0;
    for (int i = 0; i < n; ++i) {
        int idx = Hash(H, es[i]);
        HLink p = H.rcd[idx];
        bool dup = false;
        while (p) {              // 查重
            if (p->data == es[i]) { dup = true; break; }
            p = p->next;
        }
        if (dup) continue;

        // 头插
        HLink node = (HLink)malloc(sizeof(HNode));
        node->data = es[i];
        node->next = H.rcd[idx];
        H.rcd[idx] = node;
        ++H.count;
    }
    return OK;
}