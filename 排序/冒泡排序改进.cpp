#include <stdio.h>
#include <stdlib.h>
using namespace std;
typedef int KeyType;
#define MAXSIZE 20

typedef struct{
    KeyType key;
}RcdType;

typedef struct{
    RcdType *rcd; //存储空间基址
    int length; //当前长度
}RcdSqList2;

void BubbleSort(RcdSqList2 &L){
    if (L.length <= 1) return;

    int last = L.length;      // 当前需要比较的最右端（闭区间右端）
    while (last > 1) {
        int change = 1;       // 本趟最后一次交换的位置（下一趟只需到这里）
        bool swapped = false; // 可选：用于已完全有序时快速结束
        for (int j = 1; j < last; ++j) {
            if (GT(L.rcd[j], L.rcd[j+1])) {  // 比后一位大
                Swap(L.rcd[j], L.rcd[j+1]);  // 交换
                change = j;
                swapped = true;
            }
        }
        if (!swapped) break;  // 已有序
        last = change;        // 缩短下一趟扫描范围
    }
}

// 比较函数
bool GT(const RcdType &a, const RcdType &b) {
    return a.key > b.key;
}
// 交换函数
void Swap(RcdType &a, RcdType &b) {
    RcdType temp = a;
    a = b;
    b = temp;
}