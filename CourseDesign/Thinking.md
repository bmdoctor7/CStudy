
# 结构组成
- B树以书号为关键值，而每本书包含诸多信息，所以在B树的基本结点结构中新增一个指向图书信息结构体的指针，并且该结构体在B树的基本操作中始终与关键字的操作保持一致且数量也与关键字一致（书号与书籍一一对应）。而每本书的借阅信息也与本身强相关，所以在图书信息结构体中内嵌一个借阅信息的结构体指针（一本书可被多人借阅），至此整个系统的基本数据结构就全定义完了

//关键操作
## 查找
- 后续操作均要使用，最先实现
- 循环逻辑：不断在应插入位置的子树向下寻找

## 插入
- 分裂操作：
  - 结点中间关键字作为上升的关键字缓存，左右分裂为两个结点（指针和书籍信息的操作要保持一致，同时原结点要调整分裂走的关键字和指针）
  - 插入时多一个图书信息的参数，实现在书号已存在仅添加库存
- 循环：不断自底向上，直到溢出的上升关键字插入父亲后不溢出

## 删除
### 辅助函数：
  - RemoveKeyAt —— 单个结点删除关键字
  - ChildIndex —— 查找指定节点在父节点的下标
### Borrow —— 向兄弟借（够借时）
  - 实际上是父亲的桥键下移，兄弟的邻近关键字上升（左兄弟最大，右兄弟最小）桥键 —— 当前节点在父亲孩子节点中的次序所对应的关键字次序
  - 注意：左兄弟的最大孩子要成为p的最左孩子
  - 向右兄弟相反
### 与兄弟合并（不够借时）
  - 总体与向兄弟借类似，只是借的关键字变成了整个右兄弟，所以要循环迁移右兄弟的所有指针的关键字，图书信息，同时在所有操作后删除父中的桥与右兄弟指针
### 删除操作
- 非叶节点要用前继节点或后继节点替换
- 与插入类似，替换前驱删除节点后，再循环不断自底向上判断，直到没有下溢节点

## 辅助函数
- FindBorrowRecord —— 与单链表查找类似
- RemoveBorrowRecord —— 与单链表删除类似
- TraverseInOrder —— 左下角开始递归遍历
  - ListBooksInOrder —— 加入输出图书信息逻辑
  - CollectBooksByAuthor —— 加入了收集对应著者的图书信息逻辑
- ReadInt —— 带验证的输入读取
# C++ 中cin/cout 与 printf/scanf 的区别
## 1. 类型安全
- cin/cout：类型安全，编译器自动识别数据类型
```
cpp
int x; double y;
cin >> x >> y;  // 自动处理不同类型
cout << x << " " << y;
```
- printf/scanf：需要格式说明符，类型不安全
```
int x; double y;
scanf("%d %lf", &x, &y);  // 需明确指定格式
printf("%d %.2f", x, y);  // 类型不匹配会导致运行时错误
```
## 2. 语法风格
- cin/cout：使用流操作符 >> 和 <<
```
cout << "值: " << value << endl;
cin >> a >> b;
```
- printf/scanf：使用函数调用和格式字符串

```
printf("值: %d\n", value);
scanf("%d %d", &a, &b);
```
## 3. 性能差异
- printf/scanf：通常更快，直接系统调用
- cin/cout：默认与C标准库同步，较慢
```
cpp
// 可关闭同步提升速度
ios_base::sync_with_stdio(false);
cin.tie(NULL);
```
## 4. 格式控制
- printf：格式化输出更灵活
```
printf("%08d\n", 123);      // 00000123
printf("%.2f\n", 3.14159);  // 3.14
cout：需要操纵器（manipulator）
```
```
cout << setw(8) << setfill('0') << 123 << endl;
cout << fixed << setprecision(2) << 3.14159 << endl;
```
## 5. 错误处理
- cin：提供流状态检查
```
if (!(cin >> x)) {
    // 处理输入错误
    cin.clear();  // 清除错误状态
}
```
- scanf：返回值检查
```
if (scanf("%d", &x) != 1) {
    // 处理输入错误
}
```
## 6. 扩展性
- cin/cout：支持自定义类型的重载
```
struct Point { int x, y; };
ostream& operator<<(ostream& os, const Point& p) {
    return os << "(" << p.x << "," << p.y << ")";
}
```
- printf/scanf：只能处理内置类型

## 7. 内存安全
- cin/cout：自动处理缓冲区，更安全
- scanf：容易发生缓冲区溢出

```
char str[10];
scanf("%s", str);  // 可能溢出
scanf("%9s", str); // 需手动限制
```
## 8. 推荐使用场景
- cin/cout：
C++项目
需要类型安全
处理自定义类型
简单的输入输出
- printf/scanf：
需要高性能I/O
复杂格式化输出
与C代码混合
精确控制输出格式


# C++中vector与普通数组的区别
## 1. 内存管理方式
- 普通数组

```
// 静态数组（栈分配）
int arr1[5];  // 栈上，大小固定

// 动态数组（堆分配）
int* arr2 = new int[10];  // 堆上，需要手动释放
delete[] arr2;
```
- vector
```
#include <vector>
using namespace std;

vector<int> vec(5);  // 自动内存管理
vec.resize(10);      // 自动重新分配内存
// 超出作用域自动释放
```
> 关键区别：vector自动管理内存，数组需要手动管理。

## 2. 大小灵活性
固定 vs 动态
```
// 数组 - 固定大小
int arr[5];
// arr[5] = 10;  // 越界，危险！

// vector - 动态大小
vector<int> vec = {1, 2, 3};
vec.push_back(4);    // 自动扩展：1,2,3,4
vec.pop_back();      // 自动缩小：1,2,3
vec.resize(10);      // 扩展到10个元素
```
## 3. 性能特点
| 特性 | 数组 | vector |
|:--:|:--:|:--:|
内存分配|	一次分配|	可能多次重新分配 |
访问速度|	直接，最快|	间接，稍慢（但差异极小）|
插入/删除|	困难，需手动移动|	末尾O(1)，中间O(n)|
内存连续性|	保证连续|	保证连续

## 4. 初始化方式
```
// 数组初始化
int arr1[3] = {1, 2, 3};       // 正确
int arr2[] = {1, 2, 3};        // 自动推导大小
int arr3[5] = {1, 2, 3};       // 其余为0
// int arr4[3] = {};           // C++11起支持

// vector初始化
vector<int> v1 = {1, 2, 3};    // 初始化列表
vector<int> v2(5, 0);          // 5个0
vector<int> v3(10);            // 10个默认值
vector<int> v4(v1);            // 拷贝构造
```
## 5. 函数参数传递
```
// 数组传递（退化为指针）
void processArray(int arr[], int size) {  // arr实际是指针
    // 无法知道数组大小，必须传递size参数
}

// vector传递
void processVector(vector<int>& vec) {  // 引用传递
    // 可以直接使用 vec.size() 获取大小
    // 修改会影响原vector
}

void processVectorConst(const vector<int>& vec) {
    // 只读访问，不会修改原vector
}
```
## 6. 边界检查
```
// 数组 - 无边界检查
int arr[5] = {1, 2, 3, 4, 5};
cout << arr[10];  // 未定义行为，可能崩溃

// vector - 可选择边界检查
vector<int> vec = {1, 2, 3, 4, 5};
cout << vec[10];  // 未定义行为，可能崩溃
cout << vec.at(10);  // 抛出 std::out_of_range 异常
```
## 7. STL算法兼容性
```
#include <algorithm>
#include <vector>

int arr[] = {3, 1, 4, 1, 5};
vector<int> vec = {3, 1, 4, 1, 5};

// 两者都可用STL算法
sort(arr, arr + 5);
sort(vec.begin(), vec.end());

// 但vector更方便
reverse(vec.begin(), vec.end());
auto it = find(vec.begin(), vec.end(), 4);

// 数组需要手动计算大小
reverse(arr, arr + sizeof(arr)/sizeof(arr[0]));
```
## 8. 迭代器支持
```
int arr[5] = {1, 2, 3, 4, 5};
vector<int> vec = {1, 2, 3, 4, 5};

// 数组的"迭代器"（指针）
for (int* p = arr; p < arr + 5; ++p) {
    cout << *p << " ";
}

// vector的真正迭代器
for (auto it = vec.begin(); it != vec.end(); ++it) {
    cout << *it << " ";
}

// 范围for循环（C++11起都支持）
for (int x : arr) cout << x << " ";
for (int x : vec) cout << x << " ";
```
## 9. 内存重新分配策略
```
vector<int> vec;
cout << "初始容量: " << vec.capacity() << endl;  // 0

for (int i = 0; i < 100; ++i) {
    vec.push_back(i);
    // vector会以2倍（或1.5倍）策略增长
    // 每次重新分配会复制所有元素到新内存
}

// 可以预先分配
vector<int> vec2;
vec2.reserve(100);  // 预先分配100个元素空间
// 之后添加100个元素不会重新分配
```
## 10. 实际应用对比示例
### 示例1：动态数据处理
cpp
// 数组方式（繁琐）
int* readNumbersArray(int& size) {
    int capacity = 10;
    int* arr = new int[capacity];
    size = 0;
    int num;
    
    while (cin >> num) {
        if (size >= capacity) {
            // 手动重新分配
            capacity *= 2;
            int* newArr = new int[capacity];
            for (int i = 0; i < size; ++i) {
                newArr[i] = arr[i];
            }
            delete[] arr;
            arr = newArr;
        }
        arr[size++] = num;
    }
    
    return arr;  // 调用者必须记得delete[]
}

// vector方式（简单）
vector<int> readNumbersVector() {
    vector<int> numbers;
    int num;
    
    while (cin >> num) {
        numbers.push_back(num);  // 自动处理重新分配
    }
    
    return numbers;  // 自动内存管理
}
### 示例2：多维结构
cpp
// 二维数组
const int ROWS = 3, COLS = 4;
int matrix1[ROWS][COLS];

// 二维vector
vector<vector<int>> matrix2(ROWS, vector<int>(COLS));

// vector更灵活
matrix2.push_back(vector<int>(COLS));  // 添加一行
matrix2[0].push_back(10);              // 在第一行添加一列
## 11. 性能对比测试
```
#include <iostream>
#include <vector>
#include <chrono>

void testArray() {
    auto start = chrono::high_resolution_clock::now();
    
    const int N = 1000000;
    int* arr = new int[N];
    for (int i = 0; i < N; ++i) arr[i] = i;
    
    delete[] arr;
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "数组耗时: " << duration.count() << "微秒" << endl;
}

void testVector() {
    auto start = chrono::high_resolution_clock::now();
    
    const int N = 1000000;
    vector<int> vec;
    vec.reserve(N);  // 关键！预分配避免重新分配
    for (int i = 0; i < N; ++i) vec.push_back(i);
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "vector耗时: " << duration.count() << "微秒" << endl;
}
```
## 12. 选择指南
- 使用数组的场景： 

性能要求极高：嵌入式系统、游戏引擎核心
固定大小的简单数据：常量查找表、小缓冲区
与C库交互：C库函数通常需要原始指针
栈分配需求：避免堆分配开销，大小确定且较小

```
// 适合用数组的例子
const float COLOR_TABLE[3] = {0.0f, 0.5f, 1.0f};
char buffer[1024];  // 固定大小缓冲区
```
- 使用vector的场景：

大小不确定或变化：读取用户输入、动态数据集
需要频繁增删：实现列表、队列等数据结构
现代C++项目：类型安全、自动内存管理
需要STL算法：排序、查找、变换等
作为类成员：自动处理拷贝、赋值、析构

```
// 适合用vector的例子
class StudentManager {
private:
    vector<Student> students;  // 自动管理内存
    
public:
    void addStudent(const Student& s) {
        students.push_back(s);
    }
    
    // 无需编写析构函数、拷贝构造函数等
};
```
>现代C++中的数组替代品
```
std::array (C++11)
cpp
#include <array>
array<int, 5> arr = {1, 2, 3, 4, 5};  // 固定大小
cout << arr.size();  // 5
sort(arr.begin(), arr.end());  // STL兼容
// 栈分配，无额外开销，但大小固定
```

## 总结对比表
特性|	普通数组|	std::vector
:--:|:--:|:--:|
内存管理	|手动|	自动
大小	|固定|	动态可变
边界检查	|无|	.at()方法有
传递参数	|退化为指针|	保持完整信息
STL兼容	|有限	|完全兼容
迭代器	|指针模拟|	完整迭代器
多维	|直接支持|	嵌套vector
性能	|最快	|接近数组（预分配后）
安全性	|较低	|较高
代码简洁性|	较低|	较高

- 最佳实践建议
  
默认使用vector：除非有明确理由，否则优先选择vector

预分配内存：如果知道大致大小，使用reserve()避免多次重新分配

避免C风格数组：在现代C++中，优先使用vector或array

与C API交互时：使用vector::data()获取原始指针

小数据且固定大小：考虑使用std::array替代原生数组
```
// 现代C++推荐写法
vector<int> processData() {
    vector<int> result;
    result.reserve(1000);  // 预分配
    
    // ... 处理数据
    
    return result;  // 移动语义，无拷贝开销
}

// 需要传递到C函数
void callCFunction(const vector<int>& data) {
    someCFunction(data.data(), data.size());
}
```
核心思想：vector提供了数组的所有功能加上自动内存管理，是现代C++中动态数组的首选