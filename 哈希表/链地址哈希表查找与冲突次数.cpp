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

typedef int keyType;
typedef int status;
typedef struct {
    KeyType key;
}RecordType, RcdType;

typedef struct Node {
    RcdType r;
    struct Node* next;
}Node;

typedef struct LHashTable {
    Node** rcd;
    int size;//哈希表的容量
    int count;//当前表中含有的记录个数
    int(*hash)(KeyType key,int hashsize);//函数指针变量，选取的哈希函数
}LHashTable;


int countConflics(LHashTable H){
    int conflicts = 0;
    for (int i = 0; i < H.size; i++) {
        Node* p = H.rcd[i];
        if (p != NULL) {
            p = p->next; // 跳过头结点
            while (p) {
                conflicts++;
                p = p->next;
            }
        }
    }
    return conflicts;
}


Node* searchLHash(LHashTable H, KeyType key, int &c){
    c = -1;// 计数器，初始为-1（因为第一个结点不算入）
    int idx = H.hash(key, H.size);
    Node* p = H.rcd[idx];
    while (p) {
        c++;
        if (p->r.key == key) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}
