#include<iostream>

#include <vector>
#include <string>
using namespace std;

#pragma region 线性表

#pragma endregion

#pragma region Stack

#pragma region 共享栈
#define MaxSize 10
struct SharedStack
{
    int data[MaxSize]; // 静态数组存放数据
    int top;
    int bom;
};
void InitStack(SharedStack& s)
{
    s.top = MaxSize;
    s.bom = -1;
}
bool Full(const SharedStack& s)
{
    if (s.top - 1 == s.bom) return true;
    else return false;
}
// 修复点 3：拆分判空逻辑，根据 flag 判断指定的栈是否为空
bool Empty(const SharedStack& s, int flag) {
    if (flag == 1) return s.top == MaxSize; // 右栈空
    if (flag == 0) return s.bom == -1;      // 左栈空
    return false; // 非法栈号
}
bool Push(SharedStack& s, int flag, int num)
{
    if (!Full(s))
    {
        if (flag == 1)
        {
            //s.top--;
            s.data[--s.top] = num;
        }
        else if (flag == 0)
        {
            s.data[++s.bom] = num;
        }
        else
        {
            return false;
        }
        return true;
    }
    else return false;
}

bool Pop(SharedStack& s, int flag, int& e) {
    // 1. 边界防护：判空（防止下溢出 Underflow）
    if (Empty(s, flag)) {
        return false; // 栈空，出栈失败
    }

    // 2. 执行出栈逻辑
    if (flag == 1) {
        // 右栈：先提取当前栈顶元素，随后指针向右退回（自增）
        e = s.data[s.top++];
    }
    else {
        // 左栈：先提取当前栈顶元素，随后指针向左退回（自减）
        e = s.data[s.bom--];
    }

    return true; // 出栈成功
}

#pragma endregion



/// <summary>
/// 顺序存储数组栈
/// </summary>
typedef struct
{
    int data[MaxSize];
    int top;
}SqStack;
void InitStack(SqStack& s)
{
    s.top = -1;
}
bool Empty(const SqStack& s)
{
    return s.top == -1;
}
bool Full(const SqStack& s)
{
    return s.top == MaxSize - 1;
}
bool Push(SqStack& s, int num)
{
    if (!Full(s))
    {
        s.data[++s.top] = num;
        return true;
    }
    else return false;
}
bool Pop(SqStack& s, int& res)
{
    if (!Empty(s))
    {
        res = s.data[s.top--];
        return true;
    }
    else return false;
}

/// <summary>
/// 无头节点链栈
/// </summary>
namespace SingleStack
{
    struct SNode
    {
        int val;
        SNode* next;
    };
    typedef struct
    {
        SNode* top;
    }SingleStack;

    bool Empty(const SingleStack& s)
    {
        return s.top == nullptr;
    }
    /*bool Full(const SingleStack& s)
    {

    }*/
    void Push(SingleStack& s, int num)
    {
        SNode* node = new SNode;
        node->val = num;
        node->next = s.top;
        s.top = node;
    }
    bool Pop(SingleStack& s, int& res)
    {
        if (Empty(s))return false;

        SNode* s1 = s.top;
        res = s1->val;
        s.top = s.top->next;
        delete s1;
        return true;
    }
    /// <summary>
    /// 一定要初始化，不然s.top 为 野指针，不属于本程序的地址空间，会严重报错！！！！
    /// </summary>
    /// <param name="s"></param>
    void InitStack(SingleStack& s)
    {
        s.top = nullptr;
    }
}
/// <summary>
/// 带头结点链栈
/// </summary>
namespace SingleStack2
{
    typedef struct SNode
    {
        int val;
        SNode* next;
    }*SingleStack;

    bool Empty(const SingleStack& s)
    {
        return s->next == nullptr;
    }
    /*bool Full(const SingleStack& s)
    {

    }*/
    void Push(SingleStack& s, int num)
    {
        SNode* node = new SNode;
        node->val = num;

        node->next = s->next;
        s->next = node;
    }
    bool Pop(SingleStack& s, int& res)
    {
        if (Empty(s))return false;

        SNode* s1 = s->next;
        res = s1->val;
        s->next = s1->next;
        delete s1;
        return true;
    }
    /// <summary>
    /// 一定要初始化，不然s.top 为 野指针，不属于本程序的地址空间，会严重报错！！！！
    /// </summary>
    /// <param name="s"></param>
    void InitStack(SingleStack& s)
    {
        s = new SNode;
        s->next = nullptr;
    }
}
namespace DoubleStack
{
    struct DNode
    {
        int val;
        DNode* pre;
        DNode* next;
    };
    typedef struct
    {
        DNode* top;
        DNode* tail;
    }DStack;

    void InitStack(DStack& s)
    {
        s.top = new DNode{ 0, nullptr, nullptr }; // 分配头结点
        s.tail = s.top;                         // 初始时栈顶(尾)即为头
    }
    bool Empty(const DStack& s)
    {
        return s.top == s.tail;
    }
    bool Push(DStack& s, int num)
    {
        DNode* node = new DNode{ num };

        //头插法
        node->pre = s.tail;
        s.tail->next = node;
        s.tail = node;
        return true;
    }
    bool Pop(DStack& s, int& res)
    {
        if (Empty(s))return false;

        DNode* p = s.tail;
        res = p->val;

        s.tail = p->pre;
        p->pre->next = nullptr;

        delete p;
        return true;

    }
}

#pragma endregion

#pragma region 排序算法

void QuickSort(int a[], int l, int r)
{
    if (l >= r)return;

    int i = l, j = r;
    while (i < j)
    {
        while (i<j && a[j]>=a[l])--j;
        while (i<j && a[i]<=a[l])++i;
        swap(a[i], a[j]);
    }
    swap(a[l], a[i]);
    QuickSort(a, l, i - 1);
    QuickSort(a, i+1, r);
}   

void MergeSort(int a[], int l, int r)
{
    if (l >= r)return;

    int m = (l + r) >> 1;

    MergeSort(a, l, m);
    MergeSort(a, m+1, r);

    int* tmp = new int[r-l+1];
    for (int i = l; i <= r; ++i)tmp[i - l] = a[i];

    int i = 0, j = m - l + 1;

    for (int k = l; k <= r; ++k)
    {
        if (i == m - l + 1) a[k] = tmp[j++];
        else if (j == r - l + 1 || tmp[j] >= tmp[i])a[k] = tmp[i++];
        else a[k] = tmp[j++];
    }
    delete[] tmp;
}

void ShellSort(int a[], int n, int step)
{
    for (int gap = step; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n ; ++i)
        {
            int t = a[i];
            int j;
            for (int j = i - gap; j >= 0 && a[j + gap] < t; j-=gap)
            {
                a[j + gap] = a[j];
            }
            a[j + gap] = t;
        }
    }
}
#pragma endregion

#pragma region 并查集UFS
// 宏定义 10 个元素
constexpr int N = 10;
int UFS[N];
// 初始化：全部置为 -1，代表10个独立的集合，规模均为 1
void Init() 
{
    for (int i = 0; i < N; ++i) UFS[i] = -1;
}
// 核心操作 1：查 (带路径压缩)
int Find(int x) 
{
    // 递归基：值为负数，说明找到了龙头老大
    if (UFS[x] < 0) return x;

    // 递归向上传递寻找根节点
    int root = Find(UFS[x]);

    // 核心考点：递归回溯阶段执行路径压缩
    // 如果当前节点的直接父亲不是根节点，则将其拍平挂到根节点下
    if (UFS[x] != root) 
    {
        UFS[x] = root; // 核心压缩赋值
    }

    return root;
}

// 核心操作 2：并 (按规模合并 Union by Size)
void Union(int x, int y) {
    int root_x = Find(x);
    int root_y = Find(y);

    if (root_x == root_y) return; // 已在同一集合

    // 注意：存的是负数规模，值越小说明绝对规模越大 (小树并入大树)
    if (UFS[root_x] <= UFS[root_y]) {
        UFS[root_x] += UFS[root_y]; // 大树合并小树规模
        UFS[root_y] = root_x;       // 小树根节点认大树根节点为父
    }
    else {
        UFS[root_y] += UFS[root_x];
        UFS[root_x] = root_y;
    }
}



// 测试代码
// // --- 辅助可视化函数 ---
//void PrintArray(const char* msg) {
//    std::cout << msg << "\n[ ";
//    for (int i = 0; i < N; ++i) {
//        if (UFS[i] >= 0) std::cout << " "; // 对齐负号
//        std::cout << UFS[i] << (i == N - 1 ? " " : ", ");
//    }
//    std::cout << "]\n";
//}
//// 递归打印单棵树的层级结构
//void PrintTreeRecursive(int node, int depth, const std::string& prefix) {
//    if (depth == 0) {
//        std::cout << "根节点 " << node << " (集合规模: " << -UFS[node] << ")\n";
//    }
//
//    // 收集该节点的所有子节点
//    std::vector<int> children;
//    for (int i = 0; i < N; ++i) {
//        if (UFS[i] == node) {
//            children.push_back(i);
//        }
//    }
//
//    // 递归打印子树
//    for (size_t i = 0; i < children.size(); ++i) {
//        int child = children[i];
//        std::cout << prefix << (i == children.size() - 1 ? "└── " : "├── ");
//        std::cout << "子节点 " << child << "\n";
//        // 继续向下查找该子节点的后代
//        PrintTreeRecursive(child, depth + 1, prefix + (i == children.size() - 1 ? "    " : "│   "));
//    }
//}
//
//// 打印整个森林（所有根节点及子树）
//void PrintForest(const char* msg) {
//    std::cout << "\n==== " << msg << " ====\n";
//    for (int i = 0; i < N; ++i) {
//        if (UFS[i] < 0) { // 负数代表该节点是当前集合的根
//            PrintTreeRecursive(i, 0, "");
//        }
//    }
//    std::cout << "========================\n";
//}
//
//// 初始化：全部置为 -1，代表10个独立的集合，规模均为 1
//void Init() {
//    for (int i = 0; i < N; ++i) UFS[i] = -1;
//    PrintArray("==== 初始化 (Initial State) ====");
//    PrintForest("初始树形状态 (10 个孤立节点)");
//}
//// 核心操作 1：查 (带路径压缩)
//int Find(int x) {
//    // 递归基：值为负数，说明找到了龙头老大
//    if (UFS[x] < 0) return x;
//
//    // 递归向上传递寻找根节点
//    int root = Find(UFS[x]);
//
//    // 核心考点：递归回溯阶段执行路径压缩
//    // 如果当前节点的直接父亲不是根节点，则将其拍平挂到根节点下
//    if (UFS[x] != root) {
//        std::cout << "  -> 触发路径压缩: 将节点 " << x
//            << " 的父节点从 " << UFS[x] << " 直接修改为根节点 " << root << "\n";
//        UFS[x] = root; // 核心压缩赋值
//        PrintArray("  -> 压缩后数组状态:");
//    }
//
//    return root;
//}
//// 核心操作 2：并 (按规模合并 Union by Size)
//void Union(int x, int y) {
//    int root_x = Find(x);
//    int root_y = Find(y);
//
//    if (root_x == root_y) return; // 已在同一集合
//
//    std::cout << "\n执行 Union(" << x << ", " << y << "): \n";
//
//    // 注意：存的是负数规模，值越小说明绝对规模越大 (小树并入大树)
//    if (UFS[root_x] <= UFS[root_y]) {
//        UFS[root_x] += UFS[root_y]; // 大树合并小树规模
//        UFS[root_y] = root_x;       // 小树根节点认大树根节点为父
//        std::cout << "  (规模判定: " << root_x << " 规模 >= " << root_y << ") 将节点 "
//            << root_y << " 挂载到 " << root_x << " 下\n";
//    }
//    else {
//        UFS[root_y] += UFS[root_x];
//        UFS[root_x] = root_y;
//        std::cout << "  (规模判定: " << root_y << " 规模 > " << root_x << ") 将节点 "
//            << root_x << " 挂载到 " << root_y << " 下\n";
//    }
//    PrintArray("  => Union完成后的状态:");
//}
#pragma endregion

#pragma region 25年最大乘积
void CalMulSum(int a[], int res[], int n)
{
    int NerMax = INT_MAX;
    int PosMax = INT_MIN;
    int PosMaxi = -1;
    int NerMaxi = -1;


    for (int i = 0; i < n; ++i)
    {

        //////!!!!!!! 
        if (NerMaxi < i || PosMaxi < i)
        {
            NerMax = INT_MAX;
            PosMax = INT_MIN;
            for (int k = i; k < n; ++k)
            {
                /////!!!!!!!

                ///////!!!!!!!
                if (NerMax > a[k])
                {
                    NerMaxi = k;
                    NerMax = a[k];
                }

                if (PosMax < a[k])
                {
                    PosMax = a[k];
                    PosMaxi = k;
                }

            }
        }

        //////!!!!!!!
        if (a[i] < 0) res[i] = a[i] * NerMax;
        else res[i] = a[i] * PosMax;
    }
    for (int i = 0; i < n; ++i)
    {
        cout << res[i] << " ";
    }
}
void CalMulSumPro(int a[], int res[], int n)
{
    int NerMax = a[n - 1];
    int PosMax = a[n - 1];
    for (int i = n - 1; i >= 0; --i)
    {

        //////!!!!!!! 
        if (NerMax > a[i]) NerMax = a[i];
        if (PosMax < a[i]) PosMax = a[i];
        //////!!!!!!!
        if (a[i] < 0) res[i] = a[i] * NerMax;
        else res[i] = a[i] * PosMax;
    }
    for (int i = 0; i < n; ++i)
    {
        cout << res[i] << " ";
    }
}
// 辅助函数：打印原始数组
void printArray(int a[], int n) {
    cout << "原数组:   ";
    for (int i = 0; i < n; ++i) {
        cout << a[i] << " ";
    }
    cout << endl;
}
#pragma endregion

#pragma region 2010算法数组循环左移p真题

void ReverseArray(int a[], int b, int e)
{
    while (b < e)
    {
        swap(a[b], a[e]);
        b++;
        e--;
    }
}
void MoveArrayPro(int a[], int n, int p)
{
    p = p % n;

    ReverseArray(a, 0, p-1);
    ReverseArray(a, p, n-1);
    ReverseArray(a, 0, n-1);
    for (int i = 0; i < n; ++i)
    {
        cout << a[i] << " ";
    }
}
//暴力解
void MoveArray(int a[], int n, int p)
{
    int* t = new int[n];
    p = p % n;
    for (int i = 0; i < n; ++i)
    {
        t[i] = a[i];
    }
    for (int i = n - p; i < n; ++i)
    {
        a[i] = t[i - n + p];
    }
    for (int i = 0; i < n - p; ++i)
    {
        a[i] = t[i + p];
    }
    for (int i = 0; i < n; ++i)
    {
        cout << a[i] << " ";
    }
}
#include <fstream>
#include <bitset>
#pragma endregion



struct TreeNode {
    int data;
    TreeNode* left, * right;
    TreeNode(int d) : data(d), left(nullptr), right(nullptr) {}
};

// 插入辅助函数
TreeNode* insert(TreeNode* root, int val) {
    if (!root) return new TreeNode(val);
    if (val < root->data)
        root->left = insert(root->left, val);
    else
        root->right = insert(root->right, val);
    return root;
}


struct TreeNode* searchBST(struct TreeNode* root, int k)
{
    if (!root)return nullptr;
    if (root->data == k)return root;
    if (k < root->data)
        return searchBST(root->left, k);
    else 
        return searchBST(root->right, k);
}

//struct TreeNode* searchBST(struct TreeNode* root, int k)
//{
//    if (!root)return nullptr;
//    if (root->data == k)return root;
//    if (k < root->data)
//    {
//        if (root->left == nullptr)
//        {
//            TreeNode* node = new TreeNode;
//            node->data = k;
//            root->left = node;
//            node->left = nullptr, node->right = nullptr;
//            return node;
//        }
//        return searchBST(root->left, k);
//    }
//        
//    else
//    {
//        if (root->right == nullptr)
//        {
//            TreeNode* node = new TreeNode;
//            node->data = k;
//            node->left = nullptr, node->right = nullptr;
//            root->right = node;
//            
//            return node;
//        }
//        
//        return searchBST(root->right, k);
//    }
//        
//}

#pragma region 串的匹配KMP算法

int match(char a[], int n, char b[], int m)
{
    for (int i = 0; i <= n - m; ++i)
    {
        int j;
        for (j = 0; j < m; ++j)
        {
            if (a[i + j] != b[j]) break;

        }
        if (j == m) return i;
    }
    return -1;
}

/**
 * 构建 next 数组（前缀表），同时打印构造过程。
 * @param a 模式串
 * @param n 模式串长度
 * @return next 数组（需调用者 delete[]）
 */
int* BuildNext(char a[], int n) {
    int prefix_len = 0;
    int* next = new int[n];
    next[0] = 0;
    int i = 1;

    cout << "===== 开始构造 next 数组 =====" << endl;
    cout << "模式串: ";
    for (int k = 0; k < n; ++k) cout << a[k] << ' ';
    cout << endl;
    cout << "i=" << 0 << "\tprefix_len=" << prefix_len
        << "\tnext[" << 0 << "]=" << next[0] << endl;

    while (i < n) {
        cout << "----------------------------------------" << endl;
        cout << "当前 i=" << i << ", 比较 a[i]='" << a[i]
            << "' 与 a[prefix_len]='" << a[prefix_len] << "'" << endl;

            if (a[i] == a[prefix_len]) {
                prefix_len++;
                next[i] = prefix_len;
                cout << "相等 -> prefix_len 增加为 " << prefix_len
                    << ", next[" << i << "]=" << next[i] << endl;
                ++i;
            }
            else {
                cout << "不相等 -> ";
                if (prefix_len == 0) {
                    next[i] = 0;
                    cout << "prefix_len 已为 0, next[" << i << "]=" << next[i] << endl;
                    ++i;
                }
                else {
                    int old = prefix_len;
                    prefix_len = next[prefix_len - 1];
                    cout << "回溯 prefix_len: " << old
                        << " -> " << prefix_len << " (next[" << old - 1 << "])" << endl;
                }
            }
    }

    cout << "===== next 数组构造完毕 =====" << endl;
    cout << "next = [";
    for (int k = 0; k < n; ++k) {
        cout << next[k];
        if (k != n - 1) cout << ", ";
    }
    cout << "]" << endl << endl;
    return next;
}

/**
 * KMP 字符串匹配，同时打印匹配过程。
 * @param a 主串
 * @param n 主串长度
 * @param b 模式串
 * @param m 模式串长度
 * @return 首次匹配的起始位置，没找到返回 -1
 */
int KMP(char a[], int n, char b[], int m) {
    if (m == 0) {
        cout << "模式串为空，直接返回 0" << endl;
        return 0;
    }

    int* next = BuildNext(b, m);

    int i = 0, j = 0;
    cout << "===== 开始 KMP 匹配 =====" << endl;
    cout << "主串: ";
    for (int k = 0; k < n; ++k) cout << a[k] << ' ';
    cout << endl;
    cout << "模式串: ";
    for (int k = 0; k < m; ++k) cout << b[k] << ' ';
    cout << endl << endl;

    while (i < n) {
        cout << "i=" << i << ", j=" << j << " -> 比较 a[" << i << "]='"
            << a[i] << "' 与 b[" << j << "]='" << b[j] << "'" << endl;

        if (a[i] == b[j]) {
            cout << "  匹配成功，两指针同时前进" << endl;
            i++;
            j++;
        }
        else if (j > 0) {
            int old_j = j;
            j = next[j - 1];
            cout << "  失配！j>0，使用 next[" << old_j - 1 << "]="
                << next[old_j - 1] << " 回退 j: " << old_j << " -> " << j << endl;
        }
        else {
            cout << "  失配且 j=0，主串指针 i 前进" << endl;
            i++;
        }

        if (j == m) {
            int start = i - j;
            cout << "\n*** 匹配成功！j==m，返回起始位置 i - j = " << start << " ***" << endl;
            delete[] next;
            return start;
        }
        cout << "  当前状态: i=" << i << ", j=" << j << endl << endl;
    }

    cout << "主串遍历完毕，未找到匹配，返回 -1" << endl;
    delete[] next;
    return -1;
}
#pragma endregion

#pragma region 2026算法题二叉树搜索真题
//res[0]初始化为INT_MAX
///递归到叶节点，自底向上运行
void Least(TreeNode* T, int K, TreeNode* res[], int& cnt, int& minAbs)
{
    if (!T) return;
    if (T->data < K)
    {
        Least(T->right, K, res, cnt, minAbs);
    }
    else if (T->data == K)
    {
        res[0] = T;
        minAbs = 0;
        cnt = 1;
        return;
    }
    else Least(T->left, K, res, cnt, minAbs);

    int abs = (T->data - K > 0) ? T->data - K : K - T->data;
    if (minAbs > (abs))
    {
        minAbs = abs;
        res[0] = T;
        cnt = 1;
    }
    else if (minAbs == (abs))
    {
        res[cnt++] = T;
    }
    else
    {
        return;
    }
}

void FindClosetNodes(TreeNode* root, int k)
{
    TreeNode* res[2];

    int cnt = 0;

    TreeNode* cur = root;
    int minDiff = INT_MAX;
    while (cur)
    {
        int diff = (cur->data > k) ? cur->data - k : k - cur->data;
        if (diff > minDiff)
        {
            minDiff = diff;
            res[0] = cur;
            cnt = 1;    
        }
        else if (diff == minDiff)
        {
            res[cnt++] = cur;
        }
        if (cur->data == k) break;
        else if (cur->data > k) cur = cur->left;
        else cur = cur->right;
    }
    // 4. 按题目要求输出结果
    for (int i = 0; i < cnt; ++i) {
        std::cout << "绝对值: " << minDiff << ", 节点关键字: " << res[i]->data << std::endl;
    }
}
#pragma endregion

#pragma region 二叉搜索树
/// <summary>
/// 线索二叉树,线索化及其遍历
/// </summary>
struct ThreadNode
{
    int ltag, rtag;
    ThreadNode* left, * right, * parent;
    int data;
};

namespace InThread
{
    /// <summary>
    /// 中序线索化
    /// </summary>
    /// <param name="root"></param>
    /// <param name="pre"></param>
    void InThread(ThreadNode* root, ThreadNode* pre)
    {
        if (!root)return;
        //先线索化左子树
        InThread(root->left, pre);
        //左子树为空, 将左指针线索化,指向前驱节点pre
        if (!root->left)
        {
            root->left = pre;
            root->ltag = 1;
        }
        //右子树为空,pre节点的后继为,  根节点   LNR 
        if (pre != nullptr && pre->right == nullptr)
        {
            pre->right = root;
            pre->rtag = 1;
        }
        pre = root;
        InThread(root->right, pre);
    }
    ThreadNode* LastNode(ThreadNode* root)
    {
        while (root->right)root = root->right;
        return root;
    }
    ThreadNode* FirstNode(ThreadNode* root)
    {
        while (root->left)root = root->left;
        return root;
    }
    ///中序前驱
    ThreadNode* PreNode(ThreadNode* root)
    {
        //左子树最右下节点
        if (root->ltag == 0) return LastNode(root->left);
        //直接返回线索节点
        else return root->left;
    }
    ///中序后继
    ThreadNode* NextNode(ThreadNode* root)
    {
        //右子树最左下节点
        if (root->rtag == 0) return FirstNode(root->right);
        //直接返回线索节点
        else return root->right;
    }
    void CreateInThread(ThreadNode* root)
    {
        ThreadNode* pre = nullptr;
        InThread(root, pre);
        if (pre && pre->right == nullptr)
        {
            pre->rtag = 1;
        }
    }
}

/// <summary>
/// 先序线索二叉树
/// </summary>
/// <param name="root"></param>
/// <param name="pre"></param>
void PreThread(ThreadNode* root, ThreadNode* pre)
{
    if (!root)return;
    if (!root->left)
    {
        root->left = pre;
        root->ltag = 1;
    }

    if (pre != nullptr && pre->right == nullptr)
    {
        pre->right = root;
        pre->rtag = 1;
    }
    pre = root;
    // 2. 只有在 left 是真实孩子时才递归左子树 (防止环形引用)
    if (root->ltag == 0) {
        PreThread(root->left, pre);
    }
    // 3. 只有在 right 是真实孩子时才递归右子树
    if (root->rtag == 0) {
        PreThread(root->right, pre);
    }
}

//先序后继
ThreadNode* NextNode(ThreadNode* root)
{
    if (root->ltag == 0) return root->left;
    else return root->right;
}
#pragma endregion

#pragma region 图的遍历算法

//邻接矩阵
struct GraphMatrix
{
    int n, e;
    int adjMatrix[MaxSize][MaxSize];
};
// ==================== 2. 邻接表实现（静态数组模拟链表） ====================
struct Edge {
    int to;       // 目标顶点
    int next;     // 同起点的下一条边在 edges 中的下标
};
// 2. 邻接表 (Adjacency List)
typedef struct ArcNode {   // 边节点
    int adjvex;            // 该弧所指向的顶点的位置
    struct ArcNode* next;  // 指向下一条弧的指针
    // int weight;         // 网的边权值，此处视无权图省略
} ArcNode;

typedef struct {           // 顶点表节点
    int data;              // 顶点信息
    ArcNode* firstarc;     // 指向第一条依附该顶点的弧的指针
} VNode, AdjList[MaxSize]; //邻接表的的顶点 即VNode数组

typedef struct {
    AdjList vertices;      // 邻接表
    int n, e;              // 顶点数 n, 边数 e
} ALGraph;               //提供邻接表的边 顶点信息

// 全局访问标记数组
bool visited[MaxSize];


// 邻接矩阵的 DFS
void DFS_M(GraphMatrix G, int cur) {
    cout<< "DFS访问 : " << cur << " "<<endl;             // 访问初始顶点 (伪代码，比如 printf)
    visited[cur] = true;     // 标记为已访问

    // 扫描邻接矩阵的第 v 行 , 先搜 该起始节点 v --> 出度 相连的节点 
    
    for (int i = 0; i < G.n; ++i)
    {
        if (G.adjMatrix[cur][i] == 1 && !visited[i]) DFS_M(G, i);
    }
    //cout << "退栈前，逆拓扑序列：DFS访问 : " << cur << " " << endl;
}

// 邻接表 DFS 核心
void DFS_AdjT(AdjList adj, int cur)
{
    cout << "AdjList DFS邻接表访问 : " << cur << " " << endl;             // 访问初始顶点 (伪代码，比如 printf)
    visited[cur] = true;     // 标记为已访问

    // 扫描邻接矩阵的第 v 行 , 先搜 该起始节点 v --> 出度 相连的节点 
    ArcNode* node = adj[cur].firstarc;
    // 【修复】必须遍历整个链表，在内部判断
    for (; node; node = node->next) {
        if (!visited[node->adjvex]) 
            DFS_AdjT(adj, node->adjvex);
        
    }
    //cout << "退栈前，逆拓扑序列：DFS访问 : " << cur << " " << endl;
}

// 邻接矩阵 DFS 包装 (处理非连通)
void DFS(GraphMatrix G, int cur = 0)
{
    for (int i = 0; i < G.n; ++i) visited[i] = false;

    DFS_M(G, cur);
    for (int i = 0; i < G.n; ++i)
    {
        if (!visited[i])
        {
            cout << " 调用 Call DFS " << endl;
            if (!visited[i]) DFS_M(G, i);
        }
        
        // 【重要考点】这里调用 DFS/BFS 的次数，等于该无向图的连通分量个数！
    }
    cout << endl;
}

// 邻接表 DFS 包装 (处理非连通)
void DFS_ALG(ALGraph G, int start = 0) {
    for (int i = 0; i < G.n; ++i) visited[i] = false;
    DFS_AdjT(G.vertices, start);

    for (int i = 0; i < G.n; ++i) {
        
        if (!visited[i])
        {
            cout << " 调用 Call DFS " << endl;
            DFS_AdjT(G.vertices, i);
        }
            
    }
    cout << endl;
}

// 邻接矩阵 BFS 核心
void BFS_M(GraphMatrix G, int cur = 0)
{
    int* queue = new int[G.n];
    int front = 0, rear = 0;

    cout << "邻接矩阵BFS访问 : " << cur << " " << endl;
    //入队前必须 ！！！标记已经访问！！！
    visited[cur] = true;
    queue[rear++] = cur;
    while (rear!=front)
    {
        //队头元素
        int top = queue[front++];//front++出队
        
        for (int i = 0; i < G.n; ++i)
        {
            if (G.adjMatrix[top][i] == 1 && !visited[i])
            {
                cout << "邻接矩阵BFS访问 : " << i << " " << endl;
                //入队前必须 ！！！标记已经访问！！！
                visited[i] = true;
                queue[rear++] = i;
            }
        }
    }
    delete[] queue;
}

void BFS_AdjT(ALGraph adj, int cur = 0)
{
    int* queue = new int[adj.n];
    int front = 0, rear = 0;
    cout << "BFS AdjList 邻接表访问 : " << cur << " " << endl;             // 访问初始顶点 (伪代码，比如 printf)
    visited[cur] = true;     // 标记为已访问
    queue[rear++] = cur;

    while (rear != front)
    {
        //队头元素
        int top = queue[front++];//front++出队

        // 扫描邻接矩阵的第 v 行 , 先搜 该起始节点 v --> 出度 相连的节点 
        ArcNode* node = adj.vertices[top].firstarc;
        for (; node; node = node->next)
        {
            if (!visited[node->adjvex]) 
            {
                visited[node->adjvex] = true;
                cout << "BFS AdjList 邻接表访问 : " << node->adjvex << " " << endl;
                queue[rear++] = node->adjvex;
            }

            
        }
    }
    //释放开辟的堆空间
    delete[] queue;
    //cout << "退栈前，逆拓扑序列：BFS访问 : " << cur << " " << endl;
}

void BFS(GraphMatrix G, int cur = 0)
{
    for (int i = 0; i < G.n; ++i)visited[i] = false;
    cout << "BFS_M 遍历序列: ";
    for (int i = 0; i < G.n; ++i)
    {
        if (!visited[i])
        {
            cout << " 调用 Call BFS " << endl;
            BFS_M(G, i);
        }
    }
    cout << endl;
}
void BFS_ALG(ALGraph G, int start = 0) {
    for (int i = 0; i < G.n; ++i) visited[i] = false;
    cout << "BFS_AL 遍历序列: ";
    BFS_AdjT(G, start);
    for (int i = 0; i < G.n; ++i) 
    {
        if (!visited[i])
        {
            cout << " 调用 Call BFS " << endl;
            BFS_AdjT(G, i);
        }
    }
    cout << endl;
}

// ==================== 构建测试图 ====================

// 辅助函数：为无向图邻接矩阵添加边
void addEdgeM(GraphMatrix& G, int u, int v) {
    G.adjMatrix[u][v] = 1;
    G.adjMatrix[v][u] = 1;
}

// 辅助函数：为无向图邻接表添加边 (头插法)
void addEdgeAL(ALGraph& G, int u, int v) {
    ArcNode* node1 = new ArcNode{ v, G.vertices[u].firstarc };
    G.vertices[u].firstarc = node1;

    ArcNode* node2 = new ArcNode{ u, G.vertices[v].firstarc };
    G.vertices[v].firstarc = node2;
}

// 辅助函数：初始化图
void initGraphs(GraphMatrix& GM, ALGraph& ALG, int n) {
    GM.n = ALG.n = n;
    GM.e = ALG.e = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            GM.adjMatrix[i][j] = 0;
        }
        ALG.vertices[i].data = i;
        ALG.vertices[i].firstarc = NULL;
    }
}

#pragma endregion


int testA[MaxSize];
void InitMyUFS(int k)
{
    for (int i = 0; i < MaxSize; ++i) testA[i] = -1;
}
int MyFind(int k)
{
    if (testA[k] < 0) return k;
    //如果不是根节点，继续向上查，直至查到根节点，
    int root = MyFind(testA[k]);

    //并且实现路径压缩
    if (testA[k] != root)
    {
        testA[k] = root; //压缩
        return root;//将根依次传递给其他调用栈
    }
}
bool MyUnion(int a, int b)
{

    int rootA = MyFind(a);
    int rootB = MyFind(b);

    if (rootA == rootB) return false;

    if (testA[rootA] <= testA[rootB])
    {
        testA[rootA] += testA[rootB];
        testA[rootB] = rootA;
    }
    else
    {
        testA[rootB] += testA[rootA];
        testA[rootA] = rootB;
    }
}


void printFloatBitsManual(float f) {
    unsigned char* bytePtr = reinterpret_cast<unsigned char*>(&f);
    // 从最后一个字节开始，以按大端序输出（更符合阅读习惯）
    for (int i = sizeof(float) - 1; i >= 0; --i) {
        std::cout << std::bitset<8>(bytePtr[i]) << " ";
    }
    std::cout << std::endl;
}



int Mid(int a[], int b[], int L)
{
    int p1 = 0, p2 = 0, c=0;
    while (c<L)
    {
        if (a[p1] <= b[p2])
        {
            c++; 
            if (c == L)return a[p1];
            p1++;
        }
        else
        {
            c++;
            if (c == L)return b[p2];
            p2++;
        }
    }
}
int main() {
    

#pragma region  并查集
    //Init(); // 初始状态已在内部打印
    //// 1. 制造底层树形结构 (基础合并)
    //Union(0, 1);
    //Union(2, 3);
    //Union(4, 5);
    //Union(6, 7);
    //Union(8, 9);
    //// 2. 加深树的层级
    //Union(0, 2); // 集合0 (包含0,1) 并入 集合2 (包含2,3)，此时规模变为 4
    //Union(4, 6); // 集合4 (包含4,5) 并入 集合6 (包含6,7)，此时规模变为 4
    //// 3. 构建最深树：集合0 和 集合4 合并，总规模为8
    //Union(0, 4);
    //// 打印此时的未压缩的深树结构
    //PrintForest("合并完成后的树形状态 (Find(7) 之前)");
    //std::cout << "\n============================================\n";
    //std::cout << "准备执行 Find(7) ，观察回溯时的完整路径压缩过程：\n";
    //std::cout << "============================================\n";
    //Find(7);
    //// 打印最终路径压缩后的深树结构
    //PrintForest("路径压缩完成后的最终树形状态 (Find(7) 之后)");
#pragma endregion

    //int a[4] = { 1, 4, -9, 6 };
    //// ---------------- 案例 1：混合正负数（常规情况） ----------------
    //int case1[] = { 1, 4, -9, 6, -3, 2, 5, -1, 8, -2, 3 };

    //int n1 = sizeof(case1) / sizeof(case1[0]);

    //int* res = new int[11];
    //cout << "--- 测试案例 1 ---" << endl;
    //printArray(case1, n1);
    //CalMulSumPro(case1, res, n1);
    //cout << endl;

    //// ---------------- 案例 2：全是正数（包含 0） ----------------
    //int case2[] = { 2, 5, 0, 3, 8, 1, 9, 4, 7, 6, 1 };
    //int n2 = sizeof(case2) / sizeof(case2[0]);
    //int* res1 = new int[11];
    //cout << "--- 测试案例 2 ---" << endl;
    //printArray(case2, n2);
    //CalMulSumPro(case2, res1, n2);
    //cout << endl;

    //// ---------------- 案例 3：全是负数（没有正数，包含 0） ----------------
    //int case3[] = { -1, -5, -2, -9, -3, 0, -8, -4, -6, -7, -3 };
    //int n3 = sizeof(case3) / sizeof(case3[0]);
    //int* res2 = new int[11];
    //cout << "--- 测试案例 3 ---" << endl;
    //printArray(case3, n3);
    //CalMulSumPro(case3, res2, n3);
    //cout << endl;

    //// ---------------- 案例 4：递减序列（考验倒序寻找极值） ----------------
    //int case4[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, -5 };
    //int n4 = sizeof(case4) / sizeof(case4[0]);
    //int* res3 = new int[11];
    //cout << "--- 测试案例 4 ---" << endl;
    //printArray(case4, n4);
    //CalMulSumPro(case4, res3, n4);
    //cout << endl;

   /* int case1[] = { 0, 1,2,3,4,5,6,7,8 };
    int case2[] = { 0, 1,2,3,4,5,6,7,8 };
    int case3[] = { 0, 1,2,3,4,5,6,7,8 };
    int case4[] = { 0, 1,2,3,4,5,6,7,8 };
    int case5[] = { 0, 1,2,3,4,5,6,7,8 };
    MoveArray(case1, 9, 1);
    cout << endl;
    MoveArray(case2, 9, 2);
    cout << endl;
    MoveArray(case3, 9, 3);
    cout << endl;
    MoveArray(case4, 9, 4);
    cout << endl;
    MoveArray(case5, 9, 5);*/


    /*int case1[] = { 0, 1,2,3,4,5,6,7,8 };
    int case2[] = { 0, 1,2,3,4,5,6,7,8 };
    int case3[] = { 0, 1,2,3,4,5,6,7,8 };
    int case4[] = { 0, 1,2,3,4,5,6,7,8 };
    int case5[] = { 0, 1,2,3,4,5,6,7,8 };
    MoveArrayPro(case1, 9, 10);
    cout << endl;
    MoveArrayPro(case2, 9, 11);
    cout << endl;
    MoveArrayPro(case3, 9, 12);
    cout << endl;
    MoveArrayPro(case4, 9, 13);
    cout << endl;
    MoveArrayPro(case5, 9, 14);*/
#pragma region 读文件
    //// 路径使用双反斜杠或正斜杠均可
        //// 也可使用 C++11 原始字符串 R"(...)" 避免转义
        //FILE* fp;
        //errno_t err = fopen_s(&fp, "C:\\Users\\DELL\\Desktop\\py解析文件重启环境.txt", "r");
        //if (err != 0 || fp == nullptr) { // 注意：fopen_s 的返回值 0 代表成功
        //    perror("打开文件失败");
        //    return 1;
        //}
        //char line[1024];  // 缓冲区
        //fread(line, 1024, 1024, fp);
        //while (fgets(line, sizeof(line), fp) != nullptr) {
        //    // fgets 会保留读到的换行符，用 fputs 输出不额外添加换行
        //    fputs(line, stdout);
        //}
        //fclose(fp);


        //std::ifstream file(R"(C:\Users\DELL\Desktop\py解析文件重启环境.txt)");
        //if (!file.is_open()) {
        //    std::cerr << "无法打开文件" << std::endl;
        //    return 1;
        //}
        //std::string line;
        //while (std::getline(file, line)) {
        //    std::cout << line << endl;
        //}
#pragma endregion

    

//char a[] = { "ababcababcabc" };
//char b[] = { "cab" };
//cout << match(a, 14, b, 3)<<endl;
//
//char text[] = "ababcababcababd";
//char pattern[] = "ababcababa";
//int pos = KMP(text, 15, pattern, 10);
//cout << "\n最终结果: 匹配位置 = " << pos << endl;


//int amm[11] = { -5, 18, 24, 123, 32, 21 , 5, 13, 2,  21, 1 };
//MergeSort(amm, 11, 0, 10);
//for (int n : amm) cout << n << " ";


#pragma region 2026算法题二叉树搜索真题
//// 案例1
//TreeNode* root1 = nullptr;
//int vals1[] = { 5,3,7,2,4,6,8,1,0,9 };
//for (int v : vals1) root1 = insert(root1, v);
//TreeNode* res1[10];
//int cnt1 = 0, minAbs1 = INT_MAX;
//Least(root1, 5, res1, cnt1, minAbs1);
//cout << "Case1: minAbs=" << minAbs1 << " cnt=" << cnt1 << " nodes: ";
//for (int i = 0; i < cnt1; i++) cout << res1[i]->data << " ";
//cout << endl;
//
//// 案例2
//TreeNode* root2 = nullptr;
//int vals2[] = { 10,5,15,3,7,13,18,1,6,12 };
//for (int v : vals2) root2 = insert(root2, v);
//TreeNode* res2[10];
//int cnt2 = 0, minAbs2 = INT_MAX;
//Least(root2, 8, res2, cnt2, minAbs2);
//cout << "Case2: minAbs=" << minAbs2 << " cnt=" << cnt2 << " nodes: ";
//for (int i = 0; i < cnt2; i++) cout << res2[i]->data << " ";
//cout << endl;
//
//// 案例3
//TreeNode* root3 = nullptr;
//int vals3[] = { 10,5,15,3,7,12,18,1,4,9,11 };
//for (int v : vals3) root3 = insert(root3, v);
//TreeNode* res3[10];
//int cnt3 = 0, minAbs3 = INT_MAX;
//Least(root3, 8, res3, cnt3, minAbs3);
//cout << "Case3: minAbs=" << minAbs3 << " cnt=" << cnt3 << " nodes: ";
//for (int i = 0; i < cnt3; i++) cout << res3[i]->data << " ";
//cout << endl;
#pragma endregion

//SingleStack2::SingleStack s;
//SingleStack2::InitStack(s);
//SingleStack2::Push(s, 1);
//SingleStack2::Push(s, 2);


// ==========================================
    // 场景 1：连通无向图 (8个节点)
    // ==========================================
#pragma region DFS，BFS遍历案例
//cout << "========== 测试场景 1：连通图 (8个节点) ==========" << endl;
//GraphMatrix GM1; ALGraph ALG1;
//initGraphs(GM1, ALG1, 8);
//
//int edges1[][2] = { {0,1}, {0,2}, {1,3}, {1,4}, {2,5}, {2,6}, {3,7}, {4,7} };
//for (auto& e : edges1) {
//    addEdgeM(GM1, e[0], e[1]);
//    addEdgeAL(ALG1, e[0], e[1]);
//}
//
//DFS(GM1);      // 预期输出: 0 1 3 7 4 2 5 6 
//DFS_ALG(ALG1); // 头插法邻接表输出顺序会因为链表次序不同而与矩阵不同
//BFS(GM1);      // 预期输出: 0 1 2 3 4 5 6 7 
//BFS_ALG(ALG1);
//
//// ==========================================
//// 场景 2：非连通无向图 (9个节点，3个连通分量)
//// ==========================================
//cout << "\n========== 测试场景 2：非连通图 (9个节点，孤岛测试) ==========" << endl;
//GraphMatrix GM2; ALGraph ALG2;
//initGraphs(GM2, ALG2, 9);
//
//int edges2[][2] = {
//    {0,1}, {1,2}, {2,3}, {0,3}, // 分量1: 0,1,2,3 形成环
//    {4,5}, {5,6}, {6,7}         // 分量2: 4,5,6,7 链状
//    // 分量3: 8 孤立
//};
//for (auto& e : edges2) {
//    addEdgeM(GM2, e[0], e[1]);
//    addEdgeAL(ALG2, e[0], e[1]);
//}
//
//// 注意观察输出序列，外层for循环会强制唤醒不同分量的遍历
//DFS(GM2);      // 预期: (0 1 2 3) (4 5 6 7) (8)
//DFS_ALG(ALG2);
//BFS(GM2);      // 预期: (0 1 3 2) (4 5 6 7) (8)
//BFS_ALG(ALG2);
#pragma endregion

//long long mod = pow(2, 32);
//cout << (-4294967298) % mod;
//float a = FLT_MAX;
//float N = FLT_MIN;
//printFloatBitsManual(a);
//printFloatBitsManual(N);
//float s = 0.0f;
//float res = 1.0f / s;
//cout << res << endl;

//int a[5] = { 2,4,6,8,20 };
//int b[5] = { 11,13,15,17,19 };
//
//cout << Mid(a, b, 5) << endl;

int case2[] = { 2, 5, 0, 3, 8, 1, 9, 4, 7, 6, 1 };
MergeSort(case2, 0, 10);
printArray(case2, 11);
}