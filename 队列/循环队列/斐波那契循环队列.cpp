#include <stdio.h>
#include <stdlib.h>
#define Status int
#define OK 1
#define ERROR 0
typedef long ElemType;


typedef struct {
    ElemType* base; //存放栈元素的数组
    int front; //头指针
    int rear; //尾指针
    int maxSize; //队列的最大容量
} SqQueue;

long Fib(int k, int n){
	if (n < k - 1) return 0;
    if (n == k - 1) return 1;
    
    SqQueue Q;
    Q.maxSize = k + 1;
    Q.base = (ElemType*)malloc(Q.maxSize * sizeof(ElemType));
    Q.front = Q.rear = 0;
    
    long sum = 0;
    
    // 初始化队列
    for (int i = 0; i < k - 1; i++) {
        Q.base[Q.rear] = 0;
        Q.rear = (Q.rear + 1) % Q.maxSize;
    }
    Q.base[Q.rear] = 1;
    sum = 1;
    Q.rear = (Q.rear + 1) % Q.maxSize;
    
    for (int i = k; i <= n; i++) {
        // 新的斐波那契数就是当前运行总和
        long new_fib = sum;
        
        // 出队最旧的元素
        long oldest = Q.base[Q.front];
        sum -= oldest;
        Q.front = (Q.front + 1) % Q.maxSize;
        
        // 入队新的斐波那契数
        Q.base[Q.rear] = new_fib;
        sum += new_fib;
        Q.rear = (Q.rear + 1) % Q.maxSize;
    }
    
    long result = Q.base[(Q.rear - 1 + Q.maxSize) % Q.maxSize];
    free(Q.base);
    return result;
}

int main()
{

	return 0;
}

