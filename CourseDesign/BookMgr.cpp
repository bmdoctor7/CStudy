#include "BTreeADT.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>
using namespace std;


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


