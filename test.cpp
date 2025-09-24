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

typedef struct {
    ElemType* elem; 
    int length;
    int size;
    int increment;
} SqList;

char Compare(SqList A, SqList B) {
    //判断是否有空表
    bool isAempty;
    bool isBempty;

    if(A.length<B.length){
        for(int i = 0; i < A.length; i++) {
            if(A.elem[i] != B.elem[i]) {
                isAempty = false;
            }
        }
    }
    else{
        for(int i = 0; i < B.length; i++) {
            if(B.elem[i] != A.elem[i]) {
                isBempty = false;
            }
        }
    }
}

//test second commit