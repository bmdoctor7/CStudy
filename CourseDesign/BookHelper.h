#include <iostream>
#include <string>
#include <cstring>
using namespace std;

// 删除整个借阅记录链表
static void FreeBorrowList(BorrowRecord *head) {
    while (head) {
        BorrowRecord *next = head->next;
        free(head);
        head = next;
    }
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

// 创建新的图书信息结构
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
// 释放图书信息结构及其借阅记录链表
static void FreeBookInfo(BookInfo *info) {
    if (!info) return;
    FreeBorrowList(info->borrow_list);
    free(info);
}

// 创建新的借阅记录
static BorrowRecord* CreateBorrowRecord(int cardNo, int returnDate) {
    BorrowRecord *record = static_cast<BorrowRecord*>(malloc(sizeof(BorrowRecord)));
    if (!record) return nullptr;
    record->card_no = cardNo;
    record->return_date = returnDate;
    record->next = nullptr;
    return record;
}
// 查找借阅记录
static BorrowRecord* FindBorrowRecord(BorrowRecord *head, int cardNo) {
    while (head) {
        if (head->card_no == cardNo) return head;
        head = head->next;
    }
    return nullptr;
}
// 移除借阅记录
static bool RemoveBorrowRecord(BookInfo *info, int cardNo) {
    if (!info) return false;
    BorrowRecord *prev = nullptr;
    BorrowRecord *curr = info->borrow_list;
    while (curr) {
        if (curr->card_no == cardNo) {
            if (prev) {
                prev->next = curr->next;//匹配，删除节点
            } else {//第一条记录就匹配
                info->borrow_list = curr->next;//更新头指针
            }
            free(curr);
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

// 打印借阅记录表格
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

// 打印某一本图书详细信息
static void PrintBookDetail(int bookId, const BookInfo *info) {
    if (!info) return;
    cout << "书号: " << bookId << '\n';
    cout << "  书名: " << info->name << '\n';
    cout << "  作者: " << info->author << '\n';
    cout << "  可借/总量: " << info->current_count << "/" << info->total_count << '\n';
    PrintBorrowRecords(info->borrow_list);
}