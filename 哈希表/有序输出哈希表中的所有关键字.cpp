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

typedef char StrKeyType[20];
typedef struct {
    StrKeyType key; // 关键字
    int tag;        // 标记：0空 1有效 -1删除
    void *otherinfo; // 其他信息
} RcdType2;

typedef struct{
    RcdType2* rcd; // 存储记录的数组
    int size;       // 当前分配的存储容量
    int count;     // 当前记录数
} HashTable;


void PrintKeys(HashTable ht, void(*print)(StrKeyType)){
/* 请调用形参中的print函数输出关键字 */
    int a, b;
    //按首字母从A到Z顺序输出
    for (int c = 'A'; c < ('Z'+1); c++) {
        //正常情况下a即为对应字母开头关键字所在地址（哈希函数为关键字首字母的序号）
        a = (c - 'A') % ht.size;
        b = a;
        while (ht.rcd[a].tag != 0) {
            if (ht.rcd[a].tag == 1 && ht.rcd[a].key[0] == c) {
                print(ht.rcd[a].key);
            }
            //处理冲突，线性探测
            a = (a + 1) % ht.size;

            //若无空槽导致绕回起点则防止死循环退出
            if (a == b) {
                break;
            }
        }
    }
    return;
}


