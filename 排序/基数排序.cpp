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

typedef struct{
    KeyType *key;
}KeysRcdType;

typedef struct{
    KeysRcdType *rcd;
    int length; //当前长度
    int size;   //顺序表容量
    int digitNum; //关键字位数
    int radix; //基数(十进制、二进制......)
}KeysSqList;

typedef struct Node{
    KeysRcdType data;
    struct Node *next;
}LinkNode;

//尾插法创建单链表
LinkNode* CreateListR(int n){
    LinkNode *p=NULL;//头指针
    LinkNode *r=NULL;//尾指针
    LinkNode *temp=NULL;//临时指针
    for(int i=0;i<n;i++){
        temp=(LinkNode*)malloc(sizeof(LinkNode));
        scanf("%d",&temp->data.key);
        temp->next=NULL;
        if(p==NULL) p=r=temp;//创建第一个结点
        else{
            r->next=temp;
            r=temp;
        }
    }
    r->next=NULL;//尾结点指针域置空
    return p;
}


#define radix 10 //基数
#define digit 3 //关键字位数
void RadixSort(LinkNode *&p){
    //构建并初始化桶
    LinkNode *head[radix],*tail[radix];
    LinkNode *r=NULL;//排序后构造的链表尾指针
    for(int i=0;i<radix;i++){
        head[i]=tail[i]=NULL;
    }

    
    //按关键字的每一位进行排序
    for(int d=1;d<=digit;d++){
        int b=1; //基数的d-1次方
        //分配
        while(p!=NULL){
            int r = (*(p->data.key)/b)%radix; //计算关键字的第d位数字
            if(head[r]==NULL) {   //桶空
                head[r]=tail[r]=p;
            } else {
                tail[r]->next=p;
                tail[r]=p;
            }
            p=p->next;
        }
        b*=radix; //基数的d次方

        //收集:将每个桶的元素取出
        for(int i=0;i<radix;i++){
            if(head[i]!=NULL){ //桶不空
                if(p==NULL){
                    p=head[i]; //第一个非空桶
                    r=tail[i]; //更新尾指针
                } 
                else{
                    r->next=head[i]; //链接到后面
                    r=tail[i]; //更新尾指针
                }
                head[i]=tail[i]=NULL; //清空桶
            }
            r->next=NULL; //尾结点指针域置空
        }
    }
}









