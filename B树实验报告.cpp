#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
#define TRUE 1
#define FALSE 0
#define ERROR 0
#define OK 1
#define OVERFLOW -1
#define MAXSIZE 20

#define M 3  // B树的阶数（最多有 M 个孩子，最多有 M-1 个关键字；插入时允许临时溢出到 M 个关键字后再分裂）

typedef int Status;
typedef int KeyType;

#pragma region B树存储结构
/* B树节点类型定义 */
typedef struct BTNode {
    int keynum;                     //节点中关键字个数
    struct BTNode *parent;          //指向双亲节点
    KeyType key[M + 1];             // 关键字向量，1..keynum 有效，允许临时使用 keynum=M 溢出
    struct BTNode *ptr[M + 1];      // 子树指针向量，0..keynum 有效（孩子个数=关键字数+1）
} BTNode, *BTree;

/* 查找结果类型 */
typedef struct {
    BTNode *pt;     // 指向找到的节点
    int i;          // 在节点中的关键字序号
    int tag;        // 1:查找成功，0:查找失败
} Result;


#pragma endregion

#pragma region B树的基本操作
// 初始化B树
Status InitBTree(BTree *T) {
    *T = NULL;
    return OK;
}

// 销毁B树
Status DestroyBTree(BTree *T) {
    int i;
    if (*T != NULL) {
        for (i = 0; i <= (*T)->keynum; i++) {
            DestroyBTree(&((*T)->ptr[i]));
        }
        free(*T);
        *T = NULL;
    }
    return OK;
}

/* 创建一个空结点并初始化 */
static BTNode* NewNode() {
    BTNode *p = (BTNode*)malloc(sizeof(BTNode));
    if (!p) return NULL;
    p->keynum = 0;
    p->parent = NULL;
    for (int i = 0; i <= M; ++i) p->ptr[i] = NULL;
    return p;
}
#pragma endregion





#pragma region B树的查找操作
/* ---------------------- B树查找操作 ---------------------- */
/* 在节点 p 内查找关键字 K 的位置：返回 i，使得 K 应插入在 i 和 i+1 之间
   即 key[1..i] < K <= key[i+1..]，i 取值范围 [0, p->keynum] */
static int SearchInNode(BTNode *p, KeyType K) {
    int i = 1;
    while (i <= p->keynum && K > p->key[i]) i++;
    return i; // 若 i<keynum 且 K==key[i+1] 则为命中
}

/* 在整棵 B 树中查找 K。若成功，返回 {pt, i, 1}；
   若失败，返回 {最后一次访问的结点, 应插入位置 i, 0} */
Result SearchBTree(BTree T, KeyType K) {
    Result r{NULL, 0, 0};
    BTNode *p = T;
    BTNode *q = NULL; //记录 p 的双亲，若查找失败则返回K应插入的位置
    int i = 0;
    while (p) {
        i = SearchInNode(p, K);
        if (i <= p->keynum && K == p->key[i]) { //找到关键字
            r.pt = p; r.i = i; r.tag = 1; return r;
        }
        q = p;
        p = p->ptr[i-1]; //不断向下查找
    }
    r.pt = q; r.i = i; r.tag = 0; // 未找到，返回插入位置
    return r;
}
#pragma endregion





#pragma region B树的插入操作
/* 在结点 p 中第 i 与 i+1 个关键字之间插入新关键字 K 及其右子树指针 ap。
   i 由 SearchInNode 得到（K 应插入到 i 与 i+1 之间），ap 可为空（叶子插入）。*/
static void InsertKey(BTNode *p, int i, KeyType K, BTNode *ap) {
    // 右移腾位置（keys: i+1..keynum -> i+2..keynum+1；ptr 同步 i+1..keynum -> i+2..keynum+1）
    for (int j = p->keynum; j >= i; --j) {
        p->key[j + 1] = p->key[j];
        p->ptr[j + 1] = p->ptr[j];
    }
    p->key[i] = K;
    p->ptr[i] = ap;
    if (ap) ap->parent = p;
    p->keynum++;
}

/* 将结点 p 在第 s 个关键字处分裂为左右两个结点：
   左（保留在 p 中）含 key[1..s-1]；右（返回 ap）含原 key[s+1..原keynum]；
   （相比教材新增）上升关键字为 upKey=key[s]。*/
static void Split(BTree &p, int s, KeyType &upKey, BTree &ap) {
    upKey = p->key[s];
    BTNode *q = NewNode();
    q->parent = p->parent;
    // 右半部分搬迁到 q
    q->keynum = p->keynum - s;

    //右半部分的第一个孩子指针
    q->ptr[0] = p->ptr[s];
    if (q->ptr[0]) q->ptr[0]->parent = q;
    // 右半部分剩下的关键字和孩子指针
    for (int j = 1; j <= q->keynum; ++j) {
        q->key[j] = p->key[s + j];
        q->ptr[j] = p->ptr[s + j];
        if (q->ptr[j]) q->ptr[j]->parent = q;
    }
    // 左半部分缩短
    p->keynum = s - 1;
    // 返回新右结点
    ap = q;
}

/* 当根节点分裂后，创建新根(分裂时的上升元素)并连接左右孩子 
（相比教材新增）上升关键字为 upKey*/
static void NewRoot(BTree &T, KeyType upKey, BTNode *left, BTNode *right) {
    BTNode *r = NewNode();
    r->keynum = 1;
    r->key[1] = upKey;
    r->ptr[0] = left;  if (left)  left->parent  = r;
    r->ptr[1] = right; if (right) right->parent = r;
    r->parent = NULL;
    T = r;
}

/* 向 B 树中插入关键字 K。若已存在则不插入，返回 OK。*/
Status InsertBTree(BTree &T, KeyType K) {
    if (T == NULL) { // 空树，直接成为根
        T = NewNode();
        T->keynum = 1; T->key[1] = K; T->ptr[0] = T->ptr[1] = NULL; T->parent = NULL;
        return OK;
    }

    Result r = SearchBTree(T, K);
    if (r.tag == 1) return OK; // 已存在，忽略

    BTNode *p = r.pt; // 插入位置所在结点
    int i = r.i;      // 应插入在 i 与 i+1 之间

    BTNode *ap = NULL; // 分裂结点分裂后的右子树
    KeyType x = K;

    // 自底向上插入+分裂
    while (true) {
        InsertKey(p, i, x, ap);
        if (p->keynum < M) break; // 未溢出，完成

        // 溢出：分裂并将中键上升到父结点
        int s = (M + 1) / 2;      // 上升位置（ceil(M/2)）
        KeyType up;
        Split(p, s, up, ap);      // p 成为左结点，ap 为右结点，up 为上升关键字

        if (p->parent) {
            // 在父结点中找到插入 up 的位置
            BTNode *parent = p->parent;
            i = SearchInNode(parent, up);
            p = parent;// 继续向上
            x = up;
            // 继续循环，把 up 插入父结点，并把 ap 作为其右子树
        } else {
            // p 是根，需创建新根(分裂时的上升元素)
            NewRoot(T, up, p, ap);
            return OK;
        }
    }

    return OK;
}

#pragma endregion

#pragma region B树的创建操作
/* 创建一颗含有 n 个关键字的 m 阶 B 树（此实现受编译期常量 M 限制）
   - 若传入 m 与编译期常量 M 不符，将提示并按 M 构建。
   - 从标准输入读取 n 个整数作为关键字，按顺序插入构建 B 树。
*/
void CreatBTree(BTree &T,int n,int m) {
    if (n <= 0) {
        // 空树
        InitBTree(&T);
        return;
    }

    if (m != M) {
        fprintf(stderr, "[Hint] m=%d 与编译期阶数 M=%d 不一致，按 M 构建。\n", m, M);
    }

    if (T) {
        // 清理已有树，避免内存泄漏
        DestroyBTree(&T);
    }
    InitBTree(&T);

    for (int i = 0; i < n; ++i) {
        KeyType k;
        if (scanf("%d", &k) != 1) {
            fprintf(stderr, "[Warn] 第 %d 个关键字读取失败，提前结束构建。\n", i + 1);
            break;
        }
        InsertBTree(T, k);
    }
}
#pragma endregion


#pragma region B树的删除操作
static inline int MinKeys() { 
    return (M + 1) / 2 - 1; 
} // 非根结点的最少关键字数


/* 从结点 p 中删除第 i 个关键字（1<=i<=keynum），并左移收拢，同时移动相应指针 */
static void RemoveKeyAt(BTNode *p, int i) {
    for (int j = i; j < p->keynum; ++j) {
        p->key[j] = p->key[j + 1];
        p->ptr[j] = p->ptr[j + 1];
    }
    // 最后一个指针也需要前移一位
    p->ptr[p->keynum] = p->ptr[p->keynum + 1];
    p->keynum--;
}

/* 返回 child 在 parent->ptr[] 中的下标 k（0..parent->keynum）*/
static int ChildIndex(BTNode *parent, BTNode *child) {
    for (int k = 0; k <= parent->keynum; ++k){ 
        if (parent->ptr[k] == child) 
            return k;
    }
    return -1;
}

/* 向左兄弟借：使用 parent->key[k]（k>=1）作为桥，把左兄弟的最大键上移，桥键下移到 p 的最前面 */
static bool BorrowFromLeft(BTNode *parent, int k, BTNode *p) {
    BTNode *L = parent->ptr[k - 1];//左兄弟结点
    if (!L || L->keynum <= MinKeys()) return false;
    // p 的键、指针整体右移一位
    for (int j = p->keynum; j >= 1; --j) p->key[j + 1] = p->key[j];
    for (int j = p->keynum; j >= 0; --j) p->ptr[j + 1] = p->ptr[j];
    // 桥键下移到 p，左兄弟的最大孩子成为 p 的最左孩子
    p->key[1] = parent->key[k];
    p->ptr[0] = L->ptr[L->keynum]; 
    if (p->ptr[0]) p->ptr[0]->parent = p;
    p->keynum++;
    // 左兄弟最大键上移为桥键
    parent->key[k] = L->key[L->keynum];
    L->keynum--;
    return true;
}

/* 向右兄弟借：使用 parent->key[k+1] 作为桥，把右兄弟的最小键上移，桥键下移到 p 的末尾 */
static bool BorrowFromRight(BTNode *parent, int k, BTNode *p) {
    BTNode *R = parent->ptr[k + 1];
    if (!R || R->keynum <= MinKeys()) return false;
    // 桥键下移到 p 末尾
    p->key[p->keynum + 1] = parent->key[k + 1];
    p->ptr[p->keynum + 1] = R->ptr[0]; if (p->ptr[p->keynum + 1]) p->ptr[p->keynum + 1]->parent = p;
    p->keynum++;
    // 右兄弟最小键上移为桥键，右兄弟整体左移
    parent->key[k + 1] = R->key[1];
    for (int j = 1; j < R->keynum; ++j) R->key[j] = R->key[j + 1];
    for (int j = 0; j < R->keynum; ++j) R->ptr[j] = R->ptr[j + 1];
    R->keynum--;
    return true;
}

/* 与右兄弟合并：将 parent->key[k+1] 下移到 p（位于其末尾），再把右兄弟所有键和孩子搬到 p，随后删除父中的桥与右指针 */
static void MergeWithRight(BTree &T, BTNode *parent, int k, BTNode *p) {
    BTNode *R = parent->ptr[k + 1];
    int base = p->keynum;
    // 桥键下移到 p
    p->key[base + 1] = parent->key[k + 1];
    p->ptr[base + 1] = R->ptr[0]; if (p->ptr[base + 1]) p->ptr[base + 1]->parent = p;
    // 右兄弟内容依次接到 p
    for (int j = 1; j <= R->keynum; ++j) {
        p->key[base + 1 + j] = R->key[j];
        p->ptr[base + 1 + j] = R->ptr[j];
        if (p->ptr[base + 1 + j]) p->ptr[base + 1 + j]->parent = p;
    }
    p->keynum = base + 1 + R->keynum;
    // 删除父中的桥与右兄弟指针
    for (int j = k + 1; j < parent->keynum; ++j) parent->key[j] = parent->key[j + 1];
    for (int j = k + 1; j < parent->keynum; ++j) parent->ptr[j] = parent->ptr[j + 1];
    parent->ptr[parent->keynum] = parent->ptr[parent->keynum + 1];
    parent->keynum--;
    free(R);

    // 若父为根且空，提升 p 为新根
    if (parent == T && parent->keynum == 0) {
        p->parent = NULL; T = p; free(parent);
    }
}

/* 删除关键字 K */
Status DeleteBTree(BTree &T, KeyType K) {
    if (!T) return OK;
    Result r = SearchBTree(T, K);
    if (r.tag == 0) return OK; // 不存在，视为完成

    BTNode *p = r.pt; // 关键字位置所在结点
    int i = r.i; // p->key[i] == K(删除p->key[i])

    // 若非叶子：用前驱替换，转化为在叶子删除
    if (p->ptr[i - 1] != NULL) {
        BTNode *q = p->ptr[i - 1];
        while (q->ptr[q->keynum] != NULL){ 
            q = q->ptr[q->keynum];
        }//左子树的最右下节点（左子树的最大值）
        p->key[i] = q->key[q->keynum];
        p = q; i = q->keynum; // 后续在叶子 q 删除最后一个关键字
    } else if (p->ptr[i] != NULL) {
        // 理论上不会只存在一个方向，但为健壮性加上用后继替换
        BTNode *q = p->ptr[i];
        while (q->ptr[0] != NULL) q = q->ptr[0];
        p->key[i] = q->key[1];
        p = q; i = 1;
    }

    // 现在在叶子 p 删除第 i 个关键字
    RemoveKeyAt(p, i);

    // 自底向上修复下溢
    while (true) {
        if (p == T) {
            // 根的特判：若根空且有孩子，则降高
            if (p->keynum == 0) {
                BTree child = p->ptr[0];
                if (child) { child->parent = NULL; T = child; free(p); }
                else { /* 空树 */ }
            }
            break;
        }

        if (p->keynum >= MinKeys()) break; // 不下溢

        BTNode *parent = p->parent;
        int k = ChildIndex(parent, p);
        // 先尝试向左/右兄弟借
        if (k > 0 && BorrowFromLeft(parent, k, p)) break;
        if (k < parent->keynum && BorrowFromRight(parent, k, p)) break;

        // 借不了则与兄弟合并（优先与右兄弟合并，否则与左兄弟合并）
        if (k < parent->keynum) {
            MergeWithRight(T, parent, k, p);
            // 合并后可能导致父节点下溢，继续向上检查
            p = parent;
        } else {
            // 与左兄弟合并：把合并目标设为左兄弟，k-1
            BTNode *left = parent->ptr[k - 1];
            MergeWithRight(T, parent, k - 1, left);
            p = parent;
        }
    }

    return OK;
}

#pragma endregion



/* 中序遍历输出（用于调试）：按关键字递增输出所有键 */
void TraverseInOrder(BTree T) {
    if (!T) return;
    for (int i = 0; i < T->keynum; ++i) {
        TraverseInOrder(T->ptr[i]);
        printf("%d ", T->key[i + 1]);
    }
    TraverseInOrder(T->ptr[T->keynum]);
}

/* 层序打印（简单结构查看）*/
void PrintByLevel(BTree T) {
    if (!T) { printf("<empty>\n"); return; }
    // 简易队列（最多 MAXSIZE*10 结点）
    BTNode* q[1024]; int h=0,t=0; q[t++]=T;
    while (h<t) {
        int sz = t-h;
        for (int k=0;k<sz;++k) {
            BTNode* p=q[h++];
            printf("[");
            for (int i=1;i<=p->keynum;++i) {
                printf("%d%s", p->key[i], (i<p->keynum?",":""));
            }
            printf("] ");
            for (int i=0;i<=p->keynum;++i) if (p->ptr[i]) q[t++]=p->ptr[i];
        }
        printf("\n");
    }
}

// 演示基本操作
int main() {
    BTree T;
    InitBTree(&T);
    srand(static_cast<unsigned>(time(NULL)));

    const int initCount = 8;
    printf("初始化B树，随机插入 %d 个关键字。\n", initCount);
    for (int i = 0; i < initCount; ++i) {
        int key = rand() % 90 + 10; // 生成两位数，便于观察
        InsertBTree(T, key);
    }

    int select = -1;
    do {
        printf("\n当前B树层序遍历：\n");
        PrintByLevel(T);
        printf("中序关键字序列：");
        TraverseInOrder(T);
        printf("\n\n");

        printf("select 1 插入关键字 Insert()\n");
        printf("select 2 删除关键字 Delete()\n");
        printf("select 3 查找关键字 Search()\n");
        printf("select 0 退出测试\n");
        printf("input your select: ");

        if (scanf("%d", &select) != 1) {
            printf("输入错误，结束测试。\n");
            break;
        }

        switch (select) {
            case 1: {
                int key;
                printf("请输入要插入的关键字: ");
                if (scanf("%d", &key) == 1) {
                    InsertBTree(T, key);
                    printf("已插入 %d。\n", key);
                } else {
                    printf("输入无效，中止操作。\n");
                    DestroyBTree(&T);
                    return 0;
                }
                break;
            }
            case 2: {
                int key;
                printf("请输入要删除的关键字: ");
                if (scanf("%d", &key) == 1) {
                    Result r = SearchBTree(T, key);
                    if (r.tag) {
                        DeleteBTree(T, key);
                        printf("已删除 %d。\n", key);
                    } else {
                        printf("关键字 %d 不存在，无需删除。\n", key);
                    }
                } else {
                    printf("输入无效，中止操作。\n");
                    DestroyBTree(&T);
                    return 0;
                }
                break;
            }
            case 3: {
                int key;
                printf("请输入要查找的关键字: ");
                if (scanf("%d", &key) == 1) {
                    Result r = SearchBTree(T, key);
                    if (r.tag) {
                        printf("关键字 %d 已找到，所在结点含 %d 个关键字,节点的关键字序号为 %d\n", key, r.pt->keynum, r.i);
                    } else if (r.pt) {
                        printf("未找到 %d，应插入到某结点的第 %d 个位置。\n", key, r.i);
                    } else {
                        printf("树为空，尚未插入关键字。\n");
                    }
                } else {
                    printf("输入无效，中止操作。\n");
                    DestroyBTree(&T);
                    return 0;
                }
                break;
            }
            case 0:
                printf("退出测试。\n");
                break;
            default:
                printf("无效的菜单选项，请重新输入。\n");
                break;
        }

    } while (select != 0);

    DestroyBTree(&T);
    return 0;
}


