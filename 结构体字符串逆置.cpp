#include <stdio.h>
#include <stdlib.h>
#define Status int
#define OK 1
#define ERROR 0


typedef char ElemType;

typedef struct {
    ElemType* elem; //存放一个字符串
    int length; //字符串的长度
} strSequence;

strSequence* reverseStr(strSequence* strSeq) {
    if (strSeq == NULL || strSeq->elem == NULL) {
        return NULL;
    }
    
    // 分配新结构体的内存
    strSequence* newSeq = (strSequence*)malloc(sizeof(strSequence));
    if (newSeq == NULL) {
        return NULL; // 内存分配失败
    }
    
    // 分配新字符串的内存（长度+1用于存放结束符）
    newSeq->length = strSeq->length;
    newSeq->elem = (ElemType*)malloc((newSeq->length + 1) * sizeof(ElemType));
    if (newSeq->elem == NULL) {
        free(newSeq);
        return NULL; // 内存分配失败
    }
    
    // 逆序复制字符串
    for (int i = 0; i < strSeq->length; i++) {
        newSeq->elem[i] = strSeq->elem[strSeq->length - 1 - i];
    }
    newSeq->elem[newSeq->length] = '\0'; // 添加字符串结束符
    
    return newSeq;
}

// 
// strSequence* reverseStr(strSequence* strSeq) {
//     strSequence temp; // 局部变量在栈上分配
//     // ... 初始化temp
//     return &temp; // 错误！temp在函数结束时被销毁
// }
// 直接返回结构体（而不是指针）：
// 需要复制整个结构体（性能开销）
// 结构体内的elem指针仍然指向动态内存，需要调用者管理
// 不符合题目要求的返回指针的接口