#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <math.h>
using namespace std;
///状态定义
#define TRUE 1
#define FALSE 0
#define ERROR 0
#define OK 1
#define OVERFLOW -1
#define MAXSIZE 20
typedef int Status;
typedef char ElemType;
typedef int KeyType;

void ShellSort(int a[] ,int n){
    for(int gap=n/2;gap>0;gap/=2) //增量逐步减小
        for(int i=gap;i<n;i++){ //直接从每组的第二个数开始排序
            if(a[i]<a[i-gap]){ //每一组的两个数比较
                int j=i-gap;
                int temp=a[i]; //待插入元素
                a[i]=a[i-gap];
                while(j>=0 && temp<a[j]){ //寻找插入位置
                    a[j+gap]=a[j]; //大的元素后移
                    j-=gap; //继续向前寻找
                }
                a[j+gap]=temp; //小数向前插入
            }
        }
}
