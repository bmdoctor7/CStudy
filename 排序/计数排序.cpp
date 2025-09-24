#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
using namespace std;
typedef int KeyType;
#define MAXSIZE 20

typedef struct{
    KeyType key;
}RcdType;

typedef struct{
    RcdType rcd[MAXSIZE+1];
    int length; //当前长度
}RcdSqList2;


void CountSort(RcdSqList2 &L){
    if (L.length <= 1) return;

    //找到最大最小值
    KeyType maxVal = L.rcd[1].key;
    KeyType minVal = L.rcd[1].key;
    for (int i = 2; i <= L.length; ++i) {
        maxVal = max(L.rcd[i].key, maxVal);
        minVal = min(L.rcd[i].key, minVal);
    }
    int c[MAXSIZE+1] = {0};
}