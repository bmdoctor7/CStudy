
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
