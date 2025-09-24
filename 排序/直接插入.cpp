#include <stdio.h>
#include <stdlib.h>
using namespace std;
typedef int KeyType;

typedef struct{
    KeyType key;
}RcdType;

typedef struct{
    RcdType *rcd; //存储空间基址
    int length; //当前长度
    int listsize; //当前分配的存储容量
}RcdSqList;


//直接插入排序移动元素的次数
int conflictsOfInsertSort(RcdSqList* L){
    int i,j,
    count=0;
    for(i=1;i<L->length;++i){
        if(L->rcd[i+1].key<L->rcd[i].key){
            L->rcd[0]=L->rcd[i]; //哨兵
            j = i+1;
            do(j--,L->rcd[j+1]=L->rcd[j],++count);//元素后移
            while(L->rcd[0].key<L->rcd[j-1].key);
            L->rcd[j]=L->rcd[0];
        }
    }
}

//传统插入排序
void InsertSort(int a[], int n){
    for(int i=1;i<n;++i){ //直接从第二个数开始比较
        //把a[i]插入到前面已经排序的序列中
        if(a[i]<a[i-1]){  //比前面元素小才需要插入
            int temp=a[i];
            int j=i-1;    //从前一个元素开始寻找插入位置
            do{
                a[j+1]=a[j]; //将大的元素后移
                j--;
            }while(j>0 && temp<a[j]);
            a[j+1]=temp; //插入到正确位置
        }
    }
}





//直接插入排序不同监视哨兵
void InsertSort(RcdSqList &L){
    int i,j;
    for(int i=0;i<L.length;++i){
        if(L.rcd[i+1].key<L.rcd[i].key){
            L.rcd[L.length+1] = L.rcd[i+1]; //哨兵
            j = i+1;
            do{
                j--;
                L.rcd[j+1] = L.rcd[j];
            }while(L.rcd[L.length+1].key<L.rcd[j-1].key);
            L.rcd[j] = L.rcd[L.length+1];
        }
    }
    return;
}