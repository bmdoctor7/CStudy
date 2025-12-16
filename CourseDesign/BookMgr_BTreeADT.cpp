#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>
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

/* 借阅记录结构 */
typedef struct BorrowRecord {
    int card_no;           // 借阅证号
    int return_date;       // 归还期限（简化：天数）
    struct BorrowRecord *next;  // 下一借阅记录
} BorrowRecord;

/* 图书信息结构 */
typedef struct BookInfo {
    char name[100];        // 书名
    char author[100];      // 作者
    int current_count;     // 现存量
    int total_count;       // 总库存量
    BorrowRecord *borrow_list;  // 借阅记录链表
} BookInfo;

/* 辅助内存管理与工具函数 */
static void FreeBorrowList(BorrowRecord *head) {
    while (head) {
        BorrowRecord *next = head->next;
        free(head);
        head = next;
    }
}

static void FreeBookInfo(BookInfo *info) {
    if (!info) return;
    FreeBorrowList(info->borrow_list);
    free(info);
}

static BorrowRecord* CreateBorrowRecord(int cardNo, int returnDate) {
    BorrowRecord *record = static_cast<BorrowRecord*>(malloc(sizeof(BorrowRecord)));
    if (!record) return nullptr;
    record->card_no = cardNo;
    record->return_date = returnDate;
    record->next = nullptr;
    return record;
}

static BorrowRecord* FindBorrowRecord(BorrowRecord *head, int cardNo) {
    while (head) {
        if (head->card_no == cardNo) return head;
        head = head->next;
    }
    return nullptr;
}

static bool RemoveBorrowRecord(BookInfo *info, int cardNo) {
    if (!info) return false;
    BorrowRecord *prev = nullptr;
    BorrowRecord *curr = info->borrow_list;
    while (curr) {
        if (curr->card_no == cardNo) {
            if (prev) {
                prev->next = curr->next;
            } else {
                info->borrow_list = curr->next;
            }
            free(curr);
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

static BookInfo* CreateBookInfo(const string &name, const string &author, int count) {
    BookInfo *info = static_cast<BookInfo*>(malloc(sizeof(BookInfo)));
    if (!info) return nullptr;
    memset(info, 0, sizeof(BookInfo));
    strncpy(info->name, name.c_str(), sizeof(info->name) - 1);
    strncpy(info->author, author.c_str(), sizeof(info->author) - 1);
    info->total_count = count;
    info->current_count = count;
    info->borrow_list = nullptr;
    return info;
}

static void PrintBorrowRecords(const BorrowRecord *head) {
    if (!head) {
        cout << "  借阅记录: 无" << '\n';
        return;
    }
    cout << "  借阅记录:" << '\n';
    while (head) {
        cout << "    借阅证号: " << head->card_no
             << ", 归还期限(天): " << head->return_date << '\n';
        head = head->next;
    }
}


#pragma region B树存储结构
/* B树节点类型定义 */
typedef struct BTNode {
    int keynum;                     //节点中关键字个数
    struct BTNode *parent;          //指向双亲节点
    KeyType key[M + 1];             // 关键字向量，1..keynum 有效，允许临时使用 keynum=M 溢出
    struct BTNode *ptr[M + 1];      // 子树指针向量，0..keynum 有效（孩子个数=关键字数+1）
    BookInfo *bookInfo[M + 1];      // 图书信息指针
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
        for (i = 1; i <= (*T)->keynum; ++i) {
            FreeBookInfo((*T)->bookInfo[i]);
            (*T)->bookInfo[i] = nullptr;
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
    for (int i = 0; i <= M; ++i) {
        p->ptr[i] = NULL;
        p->bookInfo[i] = nullptr; // 初始化图书信息指针
    }
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

/* 在整棵 B 树中查找 K。若成功，返回 {pt, i, 1}；(pt 为关键字所在的结点，i 为关键字序号)
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
static void InsertKey(BTNode *p, int i, KeyType K, BTNode *ap,BookInfo *book = nullptr) {
    // 右移腾位置（keys: i+1..keynum -> i+2..keynum+1；ptr 同步 i+1..keynum -> i+2..keynum+1）
    for (int j = p->keynum; j >= i; --j) {
        p->key[j + 1] = p->key[j];
        p->ptr[j + 1] = p->ptr[j];

        p->bookInfo[j + 1] = p->bookInfo[j];//额外维护图书信息指针
    }
    p->key[i] = K;
    p->ptr[i] = ap;

    p->bookInfo[i] = book;

    if (ap) ap->parent = p;
    p->keynum++;
}

/* 将结点 p 在第 s 个关键字处分裂为左右两个结点：
   左（保留在 p 中）含 key[1..s-1]；右（返回 ap）含原 key[s+1..原keynum]；*/
static void Split(BTree &p, int s, KeyType &upKey, BTree &ap,BookInfo* &upBookInfo) {
    const int oldKeyNum = p->keynum;
    upKey = p->key[s];
    upBookInfo = p->bookInfo[s];
    BTNode *q = NewNode();
    q->parent = p->parent;
    q->keynum = oldKeyNum - s;

    q->ptr[0] = p->ptr[s];
    if (q->ptr[0]) q->ptr[0]->parent = q;
    for (int j = 1; j <= q->keynum; ++j) {
        q->key[j] = p->key[s + j];
        q->bookInfo[j] = p->bookInfo[s + j];
        q->ptr[j] = p->ptr[s + j];
        if (q->ptr[j]) q->ptr[j]->parent = q;
    }

    p->keynum = s - 1;
    p->bookInfo[s] = nullptr;
    for (int j = s; j <= oldKeyNum; ++j) {
        p->key[j] = 0;
        p->bookInfo[j] = nullptr;
        p->ptr[j] = NULL;
    }
    for (int j = p->keynum + 1; j <= M; ++j) {
        p->ptr[j] = NULL;
        p->bookInfo[j] = nullptr;
    }

    ap = q;
}

/* 当根节点分裂后，创建新根(分裂时的上升元素)并连接左右孩子 
（相比教材新增）上升关键字为 upKey*/
static void NewRoot(BTree &T, KeyType upKey, BookInfo* upBookInfo, BTNode *left, BTNode *right) {
    BTNode *r = NewNode();
    r->keynum = 1;
    r->key[1] = upKey;
    r->bookInfo[1] = upBookInfo;
    r->ptr[0] = left;  if (left)  left->parent  = r;
    r->ptr[1] = right; if (right) right->parent = r;
    r->parent = NULL;
    T = r;
}

/* 向 B 树中插入关键字 K。若已存在则修改对应书籍信息，返回 OK。*/
Status InsertBTree(BTree &T, KeyType K,BookInfo *book = nullptr) {
    if (T == NULL) { // 空树，直接成为根
        T = NewNode();
        T->keynum = 1; T->key[1] = K; T->ptr[0] = T->ptr[1] = NULL; T->parent = NULL;
        T->bookInfo[1] = book;
        return OK;
    }

    Result r = SearchBTree(T, K);

    // 书号已存在
    if (r.tag == 1){
        if (book && r.pt->bookInfo[r.i]) {
            r.pt->bookInfo[r.i]->total_count += book->total_count;
            r.pt->bookInfo[r.i]->current_count += book->total_count;
            FreeBookInfo(book);
        } else if (book) {
            // 仅在节点尚未绑定信息时接管新数据
            r.pt->bookInfo[r.i] = book;
        }
        return OK;
    } 

    BTNode *p = r.pt; // 插入位置所在结点
    int i = r.i;      // 应插入在 i 与 i+1 之间

    BTNode *ap = NULL; // 分裂结点分裂后的右子树
    KeyType x = K;

    // 自底向上插入+分裂
    while (true) {
        InsertKey(p, i, x, ap, book); // 插入关键字 x 及其右子树 ap
        if (p->keynum < M) break; // 未溢出，完成

        // 溢出：分裂并将中键上升到父结点
        int s = (M + 1) / 2;      // 上升位置（ceil(M/2)）
        KeyType up;
        BookInfo *upBookInfo;  // 新增：上升的图书信息
        Split(p, s, up, ap, upBookInfo);      // p 成为左结点，ap 为右结点，up 为上升关键字

        if (p->parent) {
            // 在父结点中找到插入 up 的位置
            BTNode *parent = p->parent;
            i = SearchInNode(parent, up);
            p = parent;// 继续向上
            x = up;
            book = upBookInfo;
            // 继续循环，把 up 插入父结点，并把 ap 作为其右子树
        } else {
            // p 是根，需创建新根(分裂时的上升元素)
            NewRoot(T, up, upBookInfo, p, ap);
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
static BookInfo* RemoveKeyAt(BTNode *p, int i) {
    const int oldKeyNum = p->keynum;
    BookInfo *removed = p->bookInfo[i];
    for (int j = i; j < oldKeyNum; ++j) {
        p->key[j] = p->key[j + 1];
        p->bookInfo[j] = p->bookInfo[j + 1];
        p->ptr[j] = p->ptr[j + 1];
    }
    p->keynum--;
    p->key[p->keynum + 1] = 0;
    p->ptr[p->keynum + 1] = NULL;
    p->bookInfo[p->keynum + 1] = nullptr;
    return removed;
}

/* 返回 child 在 parent->ptr[] 中的下标 k（0..parent->keynum）*/
static int ChildIndex(BTNode *parent, BTNode *child) {
    for (int k = 0; k <= parent->keynum; ++k){ 
        if (parent->ptr[k] == child) 
            return k;
    }
    return -1;
}

/* 向左兄弟借：使用 parent->key[k]（k>=1）作为桥，把左兄弟的最大键上移，桥键下移到 p的最前面*/
/* k由ChildIndex()函数获得 —— p节点在父节点ptr指针的次序*/
static bool BorrowFromLeft(BTNode *parent, int k, BTNode *p) {
    BTNode *L = parent->ptr[k - 1];//左兄弟结点

    if (!L || L->keynum <= MinKeys()) return false; // 防止左兄弟无法借

    const int oldKeyNum = L->keynum; //左兄弟原关键字数

    // p 的键、指针整体右移一位
    for (int j = p->keynum; j >= 1; --j) {
        p->key[j + 1] = p->key[j];
        p->bookInfo[j + 1] = p->bookInfo[j];
    }
    for (int j = p->keynum; j >= 0; --j) p->ptr[j + 1] = p->ptr[j];

    // 桥键下移到 p
    p->key[1] = parent->key[k];
    p->bookInfo[1] = parent->bookInfo[k];
    //左兄弟的最大孩子成为 p 的最左孩子
    p->ptr[0] = L->ptr[oldKeyNum];
    if (p->ptr[0]) p->ptr[0]->parent = p;
    p->keynum++;
    // 左兄弟最大键上移为桥键
    parent->key[k] = L->key[oldKeyNum];
    parent->bookInfo[k] = L->bookInfo[oldKeyNum];
    L->key[oldKeyNum] = 0;
    L->bookInfo[oldKeyNum] = nullptr;
    L->ptr[oldKeyNum] = NULL;
    L->keynum--;
    L->key[L->keynum + 1] = 0;
    L->ptr[L->keynum + 1] = NULL;
    L->bookInfo[L->keynum + 1] = nullptr;
    return true;
}

/* 向右兄弟借：使用 parent->key[k+1] 作为桥，把右兄弟的最小键上移，桥键下移到 p 的末尾 */
/* k由ChildIndex()函数获得 —— p节点在父节点ptr指针的次序*/
static bool BorrowFromRight(BTNode *parent, int k, BTNode *p) {
    BTNode *R = parent->ptr[k + 1];
    if (!R || R->keynum <= MinKeys()) return false;
    // 桥键下移到 p 末尾
    p->key[p->keynum + 1] = parent->key[k + 1];
    p->bookInfo[p->keynum + 1] = parent->bookInfo[k + 1];

    // 右兄弟的最小孩子成为 p 的新末尾孩子
    p->ptr[p->keynum + 1] = R->ptr[0]; 
    if (p->ptr[p->keynum + 1]) p->ptr[p->keynum + 1]->parent = p;//维护新末尾孩子的父指针
    p->keynum++;

    // 右兄弟最小键上移为桥键
    parent->key[k + 1] = R->key[1];
    parent->bookInfo[k + 1] = R->bookInfo[1];
    //右兄弟整体左移
    const int oldKeyNum = R->keynum;
    for (int j = 1; j < oldKeyNum; ++j) {
        R->key[j] = R->key[j + 1];
        R->bookInfo[j] = R->bookInfo[j + 1];
    }
    for (int j = 0; j < oldKeyNum; ++j) R->ptr[j] = R->ptr[j + 1];
    R->key[oldKeyNum] = 0;
    R->bookInfo[oldKeyNum] = nullptr;
    R->ptr[oldKeyNum] = NULL;
    R->keynum--;
    R->ptr[R->keynum + 1] = NULL;
    R->bookInfo[R->keynum + 1] = nullptr;
    return true;
}

/* 与右兄弟合并：将 parent->key[k+1] 下移到 p（位于其末尾），再把右兄弟所有键和孩子搬到 p，随后删除父中的桥与右指针 */
/* k由ChildIndex()函数获得 —— p节点在父节点ptr指针的次序*/
static void MergeWithRight(BTree &T, BTNode *parent, int k, BTNode *p) {
    BTNode *R = parent->ptr[k + 1];
    int base = p->keynum; // p 原关键字数
    // 桥键下移到 p
    p->key[base + 1] = parent->key[k + 1];
    p->bookInfo[base + 1] = parent->bookInfo[k + 1];

    // 右兄弟的最小孩子成为 p 的新末尾孩子
    p->ptr[base + 1] = R->ptr[0]; 
    if (p->ptr[base + 1]) p->ptr[base + 1]->parent = p;//维护新末尾孩子的父指针

    // 右兄弟内容依次接到 p
    for (int j = 1; j <= R->keynum; ++j) {
        p->key[base + 1 + j] = R->key[j];
        p->bookInfo[base + 1 + j] = R->bookInfo[j];
        p->ptr[base + 1 + j] = R->ptr[j];
        if (p->ptr[base + 1 + j]) p->ptr[base + 1 + j]->parent = p;
    }
    p->keynum = base + 1 + R->keynum;

    // 删除父中的桥与右兄弟指针
    const int oldParentKeys = parent->keynum;
    for (int j = k + 1; j < oldParentKeys; ++j) {
        parent->key[j] = parent->key[j + 1];
        parent->bookInfo[j] = parent->bookInfo[j + 1];
        parent->ptr[j] = parent->ptr[j + 1];
    }
    parent->key[oldParentKeys] = 0;
    parent->bookInfo[oldParentKeys] = nullptr;
    parent->ptr[oldParentKeys] = NULL;
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

        BTNode *q = p->ptr[i - 1]; //从左子树开始找前驱

        while (q->ptr[q->keynum] != NULL){ 
            q = q->ptr[q->keynum];
        }//左子树的最右下节点（左子树的最大值）

        p->key[i] = q->key[q->keynum];
        p->bookInfo[i] = q->bookInfo[q->keynum];
        q->bookInfo[q->keynum] = nullptr;
        p = q; i = q->keynum; // 后续在叶子 q 删除最后一个关键字
    } else if (p->ptr[i] != NULL) {
        // 理论上不会只存在一个方向，但为健壮性加上用后继替换
        BTNode *q = p->ptr[i];
        while (q->ptr[0] != NULL) q = q->ptr[0];
        p->key[i] = q->key[1];
        p->bookInfo[i] = q->bookInfo[1];
        q->bookInfo[1] = nullptr;
        p = q; i = 1;
    }

    // 现在在叶子 p 删除第 i 个关键字
    BookInfo *removedInfo = RemoveKeyAt(p, i);
    if (removedInfo) FreeBookInfo(removedInfo);

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
        int k = ChildIndex(parent, p); // p 在 parent->ptr[] 中的下标
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



#pragma region 图书管理系统功能

// 从标准输入读取一个整数，带输入验证
static bool ReadInt(const char *prompt, int &value) {
    cout << prompt;
    if (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "输入无效，请重试。" << '\n';
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

// 打印某一本图书详细信息
static void PrintBookDetail(int bookId, const BookInfo *info) {
    if (!info) return;
    cout << "书号: " << bookId << '\n';
    cout << "  书名: " << info->name << '\n';
    cout << "  作者: " << info->author << '\n';
    cout << "  可借/总量: " << info->current_count << "/" << info->total_count << '\n';
    PrintBorrowRecords(info->borrow_list);
}

// 中序遍历 B 树并列出所有图书
static void ListBooksInOrder(BTree node, bool &hasData) {
    if (!node) return;
    for (int i = 0; i < node->keynum; ++i) {
        ListBooksInOrder(node->ptr[i], hasData);
        BookInfo *info = node->bookInfo[i + 1];
        if (info) {
            hasData = true;
            cout << "书号: " << node->key[i + 1]
                 << " | 书名: " << info->name
                 << " | 作者: " << info->author
                 << " | 可借/总量: " << info->current_count << "/" << info->total_count << '\n';
            if (info->borrow_list) {
                cout << "  借阅中: ";
                const BorrowRecord *curr = info->borrow_list;
                bool first = true;
                while (curr) {
                    if (!first) cout << "; ";
                    cout << "证号" << curr->card_no << "/期限" << curr->return_date << "天";
                    first = false;
                    curr = curr->next;
                }
                cout << '\n';
            }
        }
    }
    ListBooksInOrder(node->ptr[node->keynum], hasData);
}


//新增书籍/增加库存
static void AddBookMenu(BTree &T) {
    int keep = 1;
    while(keep){
        int bookId = 0;
        if (!ReadInt("请输入书号:（输入-1退出） ", bookId)) return;
        if (bookId == -1) return;
        if (bookId < 0) {
            cout << "书号必须为非负整数。" << '\n';
            continue;
        }

        Result r = SearchBTree(T, bookId);
        if (r.tag) {
            int additional = 0;
            if (!ReadInt("该图书已存在，请输入新增入库数量: ", additional)) return;
            if (additional <= 0) {
                cout << "新增数量需为正整数。" << '\n';
                return;
            }
            r.pt->bookInfo[r.i]->total_count += additional;
            r.pt->bookInfo[r.i]->current_count += additional;
            cout << "库存更新完成，当前可借数量: " << r.pt->bookInfo[r.i]->current_count << '\n';
            //return;
            cout << "是否继续添加图书？(1: 是, 0: 否): ";
            cin >> keep;
            continue;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        string name;
        string author;
        cout << "请输入书名: ";
        getline(cin, name);
        cout << "请输入作者: ";
        getline(cin, author);

        int total = 0;
        if (!ReadInt("请输入入库数量: ", total)) return;
        if (total <= 0) {
            cout << "入库数量需为正整数。" << '\n';
            continue;
        }

        BookInfo *info = CreateBookInfo(name, author, total);
        if (!info) {
            cout << "内存不足，入库失败。" << '\n';
            return;
        }

        InsertBTree(T, bookId, info);
        cout << "图书入库成功。" << '\n';
        cout << "是否继续添加图书？(1: 是, 0: 否): ";
        cin >> keep;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}


//删除图书
static void DeleteBookMenu(BTree &T) {
    int keep = 1;
    while (keep)
    {
        cout<<"当前图书列表："<<endl;
        bool has = false;
        ListBooksInOrder(T, has);
        if (!T || !has) {
            cout << "暂无图书记录。" << '\n';
            return;
        }

        int bookId = 0;
        if (!ReadInt("请输入要删除的书号:(-1退出) ", bookId)) return;
        if (bookId == -1) return;

        if (bookId < 0) {
            cout << "书号必须为非负整数。" << '\n';
            continue;
        }

        Result r = SearchBTree(T, bookId);
        if (!r.tag) {
            cout << "未找到该书号的图书。" << '\n';
            continue;
        }

        BookInfo *info = r.pt->bookInfo[r.i];
        if (info && info->borrow_list) {
            cout << "仍有借阅记录，无法删除该图书。" << '\n';
            continue;
        }

        DeleteBTree(T, bookId);
        cout << "图书已删除。" << '\n';
    }
    
}


//查找图书
static void SearchBookMenu(BTree T) {
    int keep = 1;
    while (keep){
        if (!T || T->keynum == 0) {
            cout << "暂无图书记录。" << '\n';
            return;
        }

        int bookId = 0;
        if (!ReadInt("请输入要查询的书号:(-1退出) ", bookId)) return;
        if (bookId == -1) return;
        if (bookId < 0) {
            cout << "书号必须为非负整数。" << '\n';
            continue;
        }

        Result r = SearchBTree(T, bookId);
        if (!r.tag) {
            cout << "未找到该书号的图书。" << '\n';
            continue;
        }
        PrintBookDetail(bookId, r.pt->bookInfo[r.i]);
    }
}


//借阅图书
static void BorrowBookMenu(BTree T) {
    cout<<"所有输入操作均可输入-1退出当前借阅操作"<<endl;
    int keep = 1;
    while (keep)
    {
        if (!T || T->keynum == 0) {
            cout << "暂无图书记录。" << '\n';
            return;
        }

        int bookId = 0;
        if (!ReadInt("请输入要借阅的书号: ", bookId)) continue;
        if (bookId == -1) return;
        Result r = SearchBTree(T, bookId);
        if (!r.tag) {
            cout << "未找到该书号的图书。" << '\n';
            continue;
        }
        BookInfo *info = r.pt->bookInfo[r.i];
        if (info->current_count <= 0) {
            cout << "库存不足，暂无法借出。" << '\n';
            continue;
        }

        int cardNo = 0;
        if (!ReadInt("请输入借阅证号: ", cardNo)) continue;
        if (cardNo == -1) return;
        if (FindBorrowRecord(info->borrow_list, cardNo)) {
            cout << "该借阅证已借出此书，无法重复借阅。" << '\n';
            continue;
        }

        int returnDate = 0;
        if (!ReadInt("请输入归还期限(天): ", returnDate)) continue;
        if (returnDate <= 0) {
            cout << "归还期限需为正整数。" << '\n';
            return;
        }

        BorrowRecord *record = CreateBorrowRecord(cardNo, returnDate);
        if (!record) {
            cout << "内存不足，借阅登记失败。" << '\n';
            return;
        }
        record->next = info->borrow_list;
        info->borrow_list = record;
        if (info->current_count > 0) info->current_count--;
        cout << "借阅登记成功。" << '\n';
    }
    
}

//归还图书
static void ReturnBookMenu(BTree T) {
    cout<<"所有输入操作均可输入-1退出当前归还操作"<<endl;
    int keep = 1;
    while(keep){
        if (!T || T->keynum == 0) {
            cout << "暂无图书记录。" << '\n';
            return;
        }

        int bookId = 0;
        if (!ReadInt("请输入归还的书号:", bookId)) continue;
        if (bookId == -1) return;
        Result r = SearchBTree(T, bookId);
        if (!r.tag) {
            cout << "未找到该书号的图书。" << '\n';
            continue;
        }
        BookInfo *info = r.pt->bookInfo[r.i];

        int cardNo = 0;
        if (!ReadInt("请输入借阅证号: ", cardNo)) continue;
        if (cardNo == -1) return;
        if (!RemoveBorrowRecord(info, cardNo)) {
            cout << "未找到对应的借阅信息。" << '\n';
            continue;
        }
        if (info->current_count < info->total_count) info->current_count++;
        cout << "图书归还完成。" << '\n';
    }
}

static void ListBooksMenu(BTree T) {
    bool has = false;
    ListBooksInOrder(T, has);
    if (!has) cout << "暂无图书记录。" << '\n';
}

//辅助函数：递归收集某个结点中所有符合著者的图书
static void CollectBooksByAuthor(BTree node, const string &author, vector<pair<int, BookInfo*>> &matches) {
    if (!node) return;
    for (int i = 0; i < node->keynum; ++i) {
        CollectBooksByAuthor(node->ptr[i], author, matches);
        BookInfo *info = node->bookInfo[i + 1];
        if (info && author == info->author) {
            matches.emplace_back(node->key[i + 1], info);
        }
    }
    CollectBooksByAuthor(node->ptr[node->keynum], author, matches);
}

//按著者查询图书
static void SearchByAuthorMenu(BTree T) {
    if (!T || T->keynum == 0) {
        cout << "暂无图书记录。" << '\n';
        return;
    }

    while (true) {
        cout << "请输入要查询的著者名(直接回车退出): ";
        string author;
        getline(cin, author);
        if (author.empty()) {
            cout << "已退出著者查询。" << '\n';
            return;
        }

        vector<pair<int, BookInfo*>> matches;
        CollectBooksByAuthor(T, author, matches);
        if (matches.empty()) {
            cout << "未找到该著者的图书。" << '\n';
            continue;
        }

        cout << "该著者的图书:" << '\n';
        for (const auto &entry : matches) {
            PrintBookDetail(entry.first, entry.second);
        }
    }
}

#pragma endregion


/* 中序遍历输出（用于调试）：按关键字递增输出所有键 */
void TraverseInOrder(BTree T) {
    if (!T) return;
    for (int i = 0; i < T->keynum; ++i) {
        TraverseInOrder(T->ptr[i]);
        cout << T->key[i + 1] << ' ';
    }
    TraverseInOrder(T->ptr[T->keynum]);
}

/* 凹入表形式打印 B 树结构 */
static void PrintIndented(BTree node, int depth) {
    if (!node) return;
    const string indent(depth * 4, ' ');
    cout << indent;
    for (int i = 1; i <= node->keynum; ++i) {
        cout << node->key[i];
        if (i < node->keynum) cout << ',';
    }
    cout << '\n';
    for (int i = 0; i <= node->keynum; ++i) {
        PrintIndented(node->ptr[i], depth + 1);
    }
}

void PrintByLevel(BTree T) {
    if (!T || T->keynum == 0) {
        cout << "<empty>" << '\n';
        return;
    }
    PrintIndented(T, 0);
}

// 图书管理系统入口
int main() {
    BTree T;
    InitBTree(&T);

    bool running = true;
    while (running) {
        cout << "\n====== 图书管理系统 ======" << '\n';
        cout << "1. 新书入库/增加库存" << '\n';
        cout << "2. 删除图书" << '\n';
        cout << "3. 查询图书" << '\n';
        cout << "4. 借阅图书" << '\n';
        cout << "5. 归还图书" << '\n';
        cout << "6. 展示全部图书" << '\n';
        cout << "7. 查看B树结构" << '\n';
        cout << "8. 按著者查询图书" << '\n';
        cout << "0. 退出系统" << '\n';
        cout << "请选择: ";

        int choice = -1;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入无效，请重新选择。" << '\n';
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                AddBookMenu(T);
                break;
            case 2:
                DeleteBookMenu(T);
                break;
            case 3:
                SearchBookMenu(T);
                break;
            case 4:
                BorrowBookMenu(T);
                break;
            case 5:
                ReturnBookMenu(T);
                break;
            case 6:
                ListBooksMenu(T);
                break;
            case 7:
                PrintByLevel(T);
                break;
            case 8:
                SearchByAuthorMenu(T);
                break;
            case 0:
                running = false;
                break;
            default:
                cout << "无效的菜单选项，请重新选择。" << '\n';
                break;
        }
    }

    DestroyBTree(&T);
    cout << "已退出系统。" << '\n';
    return 0;
}


