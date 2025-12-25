#include <iostream>
#include <string>
#include <cstring>
using namespace std;

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
