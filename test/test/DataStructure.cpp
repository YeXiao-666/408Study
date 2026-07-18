#include<iostream>
#include<vector>
#include <set>
#include <unordered_set>
#include <stack>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath> 
#include <string>
using namespace std;
#define MaxSize 100

/// <summary>
/// 数据结构
/// </summary>

#pragma region 线性表
/// <summary>
/// 线性表
/// </summary>
typedef struct
{
	int data[10];
	int length;
}SqList;
void InitList(SqList& list)
{
	list.length = 0;
}void Reverse(vector<int>& container)
{
	int mid = (container.size() - 1);
	int middle = (mid >> 1);
	if (mid & 1)
	{
		for (int i = 0; i <= middle; ++i)
		{
			int ind = 2 * middle - i + 1;
			int temp = container[ind];
			container[ind] = container[i];
			container[i] = temp;

		}
	}
	else
	{
		for (int i = 0; i <= middle; ++i)
		{
			int ind = 2 * middle - i;
			int temp = container[ind];
			container[ind] = container[i];
			container[i] = temp;
		}
	}
}
void DeleteM(int m, vector<int>& container)
{
	int cnt = 0;
	for (int i = 0; i < container.size(); ++i)
	{
		if (container[i] == m)
		{
			cnt++;

		}
		else
		{
			container[i - cnt] = container[i];
		}
	}
}


/// <summary>
/// 顺序表
/// </summary>

typedef struct
{
	int data[MaxSize];
	int length;
}MySqList;

void InitList(MySqList& L)
{
	for (int i = 0; i < L.length; ++i) L.data[i] = 0;
	L.length = 0;
}

typedef struct
{
	int* data;
	int Size;
	int length;
}SeqList;

void InitList(SeqList& L)
{
	//delete L.data;               // 释放旧内存
	//L.data = new int[MaxSize];
	L.data = (int*)malloc(MaxSize * sizeof(int));
	L.length = 0;
	L.Size = MaxSize;
}
void IncreaseSize(SeqList& L, int len)
{
	int* p = L.data;
	L.data = (int*)malloc((L.Size + len) * sizeof(int));
	for (int i = 0; i < L.length; ++i)
	{
		L.data[i] = p[i];
		//L.data[i] = *(p+i);
	}
	L.Size += len;
	free(p);
}

/// <summary>
/// 单链表
/// </summary>

typedef struct LNode
{
	int data;
	struct LNode* next;
}LNode, * LinkList;
bool InitLinkList(LinkList& L)
{
	L = NULL;
	return true;
}
bool InitLinkList(LinkList& L, bool hasHead)
{
	L = (LNode*)malloc(sizeof(LNode)); //分配头节点
	if (L == NULL)return false; //内存不足分配失败
	L->next = NULL;
	return true;
}

/// <summary>
/// 栈
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
	return s.top == MaxSize-1;
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
bool Push(SqStack& s,int& res)
{
	if (!Empty(s))
	{
		s.data[--s.top] = res;
		return true;
	}
	else return false;
}
#pragma endregion

#pragma region 树

/// <summary>
/// 线索二叉树
/// </summary>
typedef struct ThreadNode
{
	int data;
	struct ThreadNode* rchild, * lchild;
	int ltag, rtag;
}ThreadNode, * ThreadTree;
ThreadNode* pre = NULL;

void visit(ThreadNode* q)
{
	if (q->lchild == NULL)
	{
		q->lchild = pre;
		q->ltag = 1;
	}
	if (pre != NULL && pre->rchild)
	{
		pre->rchild = q;
		pre->rtag = 1;
	}
	pre = q;
}

/// <summary>
/// 先序线索二叉
/// </summary>
/// <param name="T"></param>
void PreThread(ThreadTree T)
{
	if (T != NULL)
	{
		visit(T);
		if (T->ltag == 0)
			PreThread(T->lchild);
		PreThread(T->rchild);
	}
}

/// <summary>
/// 中序线索二叉
/// </summary>
/// <param name="T"></param>
void InThread(ThreadTree T)
{
	if (T != NULL)
	{
		InThread(T->lchild);
		visit(T);
		InThread(T->rchild);
	}
}
namespace InThreadTree
{
	ThreadNode* FirstNode(ThreadTree T)
	{
		while (T->ltag == 0) T = T->lchild; //第一个遍历的结点ltag == 1无左孩子
		return T;
	}
	ThreadNode* NextNode(ThreadNode* T)
	{
		if (T->rtag == 1) return T->rchild;
		else FirstNode(T->rchild);
	}
}
void Inorder(ThreadTree T)
{
	for (ThreadNode* p = InThreadTree::FirstNode(T); p != NULL; p = InThreadTree::NextNode(p))
		visit(p);
}

void CreateThreadTree(ThreadTree T)
{
	if (T != NULL)
	{
		pre = NULL;
		InThread(T);
		/*if (pre->rchild == NULL)
			pre->rtag = 1;*/
		pre->rchild == NULL;
		pre->rtag = 1;
	}
}

/// <summary>
/// 二叉树
/// </summary>
typedef struct BinNode
{
	int weight;
	struct BinNode* left;
	struct BinNode* right;
} BinNode, * BinTree;
BinNode* CreateNode(int w)
{
	BinNode* n = (BinNode*)malloc(sizeof(BinNode*));
	n->left = n->right = NULL;
	n->weight = w;
	return n;
}
int CalSum(BinTree n, int dep)
{
	if (!n)return 0;
	if (!n->left && !n->right)
		return n->weight * dep;
	return CalSum(n->left, dep + 1) + CalSum(n->right, dep + 1);
}


/// <summary>
/// 并查集，无向图判环
/// </summary>
class DSU {
private:
	vector<int> parent; // 记录每个节点的父节点
	vector<int> size;   // 记录以该节点为根的集合的大小（用于优化合并）

public:
	// 构造函数：初始化 n 个节点，编号从 0 到 n-1（或 1 到 n，视需求而定）
	DSU(int n) {
		parent.resize(n, -1);
		size.resize(n, 1); // 初始时每个节点各自为一个大小为 1 的集合
	}

	// 核心操作 1：查找节点 x 所属的集合的根节点（带路径压缩优化）
	int find(int x)
	{
		//if (parent[x] != x) {
		//	// 递归查找根节点，并将当前节点的父节点直接更新为根节点（路径压缩）
		//	parent[x] = find(parent[x]);
		//}
		int root = x;
		while (parent[root] >= 0)
		{
			root = parent[root];
		}
		while (x != root)
		{
			int t = parent[x];
			parent[x] = root;
			x = t;

		}
		return root;
	}

	// 核心操作 2：合并节点 x 和节点 y 所在的两个集合（按大小合并优化）
	// 返回值：如果成功合并返回 true；如果原本就在同一个集合，返回 false
	bool unite(int x, int y) {
		int rootX = find(x);
		int rootY = find(y);

		if (rootX == rootY) {
			return false; // 已经在同一个集合中，合并失败（说明这俩点之间如果连边，会产生环）
		}

		// 按大小合并：将小树挂到大树下面，保持树的平衡
		if (size[rootX] < size[rootY]) {
			swap(rootX, rootY); // 确保 rootX 是较大的树
		}

		parent[rootY] = rootX;      // y 的根指向 x 的根
		size[rootX] += size[rootY]; // 更新大树的大小
		parent[rootX] = -1 * size[rootX];

		return true;
	}

	// 辅助操作：判断 x 和 y 是否连通（属于同一集合）
	bool connected(int x, int y) {
		return find(x) == find(y);
	}

	// 辅助操作：获取某个节点所在集合的大小
	int getSize(int x) {
		return size[find(x)];
	}
	void getParent()
	{
		for (int i = 0; i < parent.size(); ++i)cout << "i = " << i << " " << parent[i] << endl;
	}
};

#pragma region B树
/// <summary>
/// B树，多路平衡查找树
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T>
struct BTreeNode {
	int m;                    // 阶数
	bool isLeaf;              // 是否为叶子节点
	int keyCount;             // 关键字数量
	std::vector<T> keys;      // 容量分配为 m (预留一个溢出位方便分裂)
	std::vector<BTreeNode*> children; // 容量分配为 m + 1定义

	BTreeNode(int degree, bool leaf) {
		m = degree;
		isLeaf = leaf;
		keyCount = 0;
		keys.resize(m);
		children.resize(m + 1, nullptr);
	}
};

template <typename T>
class BTree {
private:
	BTreeNode<T>* root;
	int m; // B树的阶

public:
	BTree(int degree) : m(degree), root(nullptr) {}

	// 1. 求高操作
	// 分析：因为B树所有叶子节点都在同一层，因此时间复杂度仅为 O(h)
	int getHeight() {
		if (root == nullptr) return 0;
		int height = 1;
		BTreeNode<T>* current = root;
		// 一直往最左边的子树走，直到叶子节点
		while (!current->isLeaf) {
			height++;
			current = current->children[0];
		}
		return height;
	}

	BTreeNode<T>* search(T key) {
		return searchInternal(root, key);
	}
	void insert(T key) {
		// 边界条件：树为空
		if (root == nullptr) {
			root = new BTreeNode<T>(m, true);
			root->keys[0] = key;
			root->keyCount = 1;
			return;
		}

		// 边界条件：根节点已满。此时必须先分裂根节点，树的高度增加 1
		if (root->keyCount == m - 1) {
			BTreeNode<T>* newRoot = new BTreeNode<T>(m, false);
			newRoot->children[0] = root;
			splitChild(newRoot, 0); // 分裂原根节点

			// 决定新值插入到分裂后的哪一边
			int i = 0;
			if (newRoot->keys[0] < key) { i++; }
			insertNonFull(newRoot->children[i], key);

			root = newRoot; // 更新根指针
		}
		else {
			insertNonFull(root, key); // 根未满，直接进入常规流程
		}
	}
	void remove(T key) {
		if (!root) {
			std::cout << "The tree is empty\n";
			return;
		}

		deleteInternal(root, key);

		// 顶级边界条件：如果根节点因为合并操作失去了所有关键字（keyCount == 0）
		// 但它还有子节点，那么树的高度减少 1，将根的唯一子树提升为新根
		if (root->keyCount == 0) {
			BTreeNode<T>* tmp = root;
			if (root->isLeaf) {
				root = nullptr;
			}
			else {
				root = root->children[0];
			}
			delete tmp;
		}
	}
	// 辅助测试：按层级打印 B树，直观查看结构
	void printLevelOrder() {
		if (!root) return;
		queue<BTreeNode<T>*> q;
		q.push(root);
		int level = 1;

		while (!q.empty()) {
			int size = q.size();
			cout << "Level " << level << ": ";
			for (int i = 0; i < size; i++) {
				BTreeNode<T>* curr = q.front();
				q.pop();

				cout << "[";
				for (int j = 0; j < curr->keyCount; j++) {
					cout << curr->keys[j] << (j == curr->keyCount - 1 ? "" : ",");
				}
				cout << "] ";

				if (!curr->isLeaf) {
					for (int j = 0; j <= curr->keyCount; j++) {
						if (curr->children[j] != nullptr) {
							q.push(curr->children[j]);
						}
					}
				}
			}
			cout << endl;
			level++;
		}
		cout << "-----------------------" << endl;
	}
	// 2. 查找操作 (内部递归实现)
private:
	BTreeNode<T>* searchInternal(BTreeNode<T>* node, T key) {
		if (node == nullptr) return nullptr;

		int i = 0;
		// 找到第一个大于或等于 key 的关键字位置
		while (i < node->keyCount && key > node->keys[i]) {
			i++;
		}

		// 边界条件 A：恰好命中，直接返回当前节点
		if (i < node->keyCount && node->keys[i] == key) {
			return node;
		}

		// 边界条件 B：未命中且当前是叶子节点，说明树中不存在该值
		if (node->isLeaf) {
			return nullptr;
		}

		// 边界条件 C：未命中且为内部节点，进入第 i 个子树继续查找
		return searchInternal(node->children[i], key);
	}
	// 分裂满载的子节点
	// 参数说明：parent 是父节点，childIndex 是满载子节点在父节点中的索引
	void splitChild(BTreeNode<T>* parent, int childIndex) {
		BTreeNode<T>* fullChild = parent->children[childIndex];
		// 创建一个新的同级节点，用于接收 fullChild 的右半部分
		BTreeNode<T>* newNode = new BTreeNode<T>(m, fullChild->isLeaf);

		// 确定中间分裂点
		int mid = (m - 1) / 2;
		// 比如 5阶B树，满载为 4个key，mid索引为1 (即第2个元素)

		// 1. 将 fullChild 中间点右侧的 key 转移给 newNode
		newNode->keyCount = (m - 1) - mid - 1;
		for (int j = 0; j < newNode->keyCount; j++) {
			newNode->keys[j] = fullChild->keys[mid + 1 + j];
		}

		// 2. 如果不是叶子节点，还需要转移子节点指针
		if (!fullChild->isLeaf) {
			for (int j = 0; j <= newNode->keyCount; j++) {
				newNode->children[j] = fullChild->children[mid + 1 + j];
			}
		}

		fullChild->keyCount = mid; // 更新原节点的数量

		// 3. 将中间点提升到父节点 parent 中
		// 首先将 parent 中 childIndex 之后的子节点指针后移一位，腾出空间
		for (int j = parent->keyCount; j >= childIndex + 1; j--)
		{
			parent->children[j + 1] = parent->children[j];
		}
		parent->children[childIndex + 1] = newNode;

		// 然后将 parent 中 childIndex 之后的 key 后移一位
		for (int j = parent->keyCount - 1; j >= childIndex; j--)
		{
			parent->keys[j + 1] = parent->keys[j];
		}
		parent->keys[childIndex] = fullChild->keys[mid]; // 放入中间值
		parent->keyCount++;
	}

	// 在确认非满载的节点中执行插入
	void insertNonFull(BTreeNode<T>* node, T key) {
		int i = node->keyCount - 1;

		// 边界条件 A：如果是叶子节点，直接找到位置插入
		if (node->isLeaf) {
			while (i >= 0 && node->keys[i] > key) {
				node->keys[i + 1] = node->keys[i];
				i--;
			}
			node->keys[i + 1] = key;
			node->keyCount++;
		}
		// 边界条件 B：如果是内部节点，需要向下寻找合适的子节点
		else {
			while (i >= 0 && node->keys[i] > key) { i--; }
			i++; // 目标子节点的索引

			// 预防性检查：如果目标子树已满，先对其进行分裂
			if (node->children[i]->keyCount == m - 1) {
				splitChild(node, i);
				// 分裂后，中间值提了上来，需要判断 key 应该进入左边还是右边的新子树
				if (key > node->keys[i]) {
					i++;
				}
			}
			insertNonFull(node->children[i], key);
		}
	}
	void deleteInternal(BTreeNode<T>* node, T key)
	{
		int idx = 0;
		while (idx < node->keyCount && node->keys[idx] < key) { ++idx; }

		// 命中情况：当前节点包含该 key
		if (idx < node->keyCount && node->keys[idx] == key) {
			if (node->isLeaf) {
				// 边界策略 1：在叶子节点中命中，直接移除并左移后续元素
				removeFromLeaf(node, idx);
			}
			else {
				// 边界策略 2：在内部节点中命中，用前驱或后继替换，然后递归删除前驱/后继
				removeFromNonLeaf(node, idx);
			}
		}
		// 未命中情况：key 存在于更下层的子树中
		else {
			if (node->isLeaf) {
				std::cout << "The key " << key << " does not exist in the tree.\n";
				return;
			}

			// 边界策略 3：在进入子节点前，必须确保子节点包含至少 ⌈m/2⌉ 个关键字。
			// 只有这样，后续递归深处的删除操作哪怕引起“借用”或“合并”，也不会向上波及。
			bool flag = (idx == node->keyCount); // 标记是否将要进入最右侧子节点

			int minKeys = (m + 1) / 2 - 1; // ⌈m/2⌉ - 1 的整数计算方式
			if (node->children[idx]->keyCount <= minKeys) {
				fill(node, idx); // 如果子节点“贫困”，通过借用兄弟或合并来充实它
			}

			// 处理特殊合并导致的索引偏移
			if (flag && idx > node->keyCount) {
				deleteInternal(node->children[idx - 1], key);
			}
			else {
				deleteInternal(node->children[idx], key);
			}
		}
	}

	// 核心充实机制（解决“贫困”节点）：
	void fill(BTreeNode<T>* node, int idx) {
		int minKeys = (m + 1) / 2 - 1;   // m/2取上界 减一
		// 1. 尝试向左兄弟借 (左旋)
		if (idx != 0 && node->children[idx - 1]->keyCount > minKeys) {
			borrowFromPrev(node, idx);
		}
		// 2. 尝试向右兄弟借 (右旋)
		else if (idx != node->keyCount && node->children[idx + 1]->keyCount > minKeys) {
			borrowFromNext(node, idx);
		}
		// 3. 左右兄弟都穷，执行合并 (Merge)
		else {
			if (idx != node->keyCount) {
				merge(node, idx); // 与右兄弟合并
			}
			else {
				merge(node, idx - 1); // 与左兄弟合并
			}
		}
	}
	// ---------------- 删除前驱/后继查找模块 ----------------
	T getPred(BTreeNode<T>* node, int idx) {
		BTreeNode<T>* curr = node->children[idx];
		while (!curr->isLeaf) curr = curr->children[curr->keyCount];
		return curr->keys[curr->keyCount - 1];
	}

	T getSucc(BTreeNode<T>* node, int idx) {
		BTreeNode<T>* curr = node->children[idx + 1];
		while (!curr->isLeaf) curr = curr->children[0];
		return curr->keys[0];
	}

	// ---------------- 删除结构修复模块 ----------------
	void removeFromLeaf(BTreeNode<T>* node, int idx) {
		for (int i = idx + 1; i < node->keyCount; ++i) node->keys[i - 1] = node->keys[i];
		node->keyCount--;
	}
	// ★ 本次补齐的核心方法 ★
	void removeFromNonLeaf(BTreeNode<T>* node, int idx) {
		T k = node->keys[idx];
		int minKeys = (m + 1) / 2 - 1;

		// 策略1：如果左子树有富余，找前驱替换
		if (node->children[idx]->keyCount > minKeys) {
			T pred = getPred(node, idx);
			node->keys[idx] = pred;
			deleteInternal(node->children[idx], pred);
		}
		// 策略2：如果左子树不够，但右子树有富余，找后继替换
		else if (node->children[idx + 1]->keyCount > minKeys) {
			T succ = getSucc(node, idx);
			node->keys[idx] = succ;
			deleteInternal(node->children[idx + 1], succ);
		}
		// 策略3：左右子树都穷，直接合并，然后在新节点中删除 k
		else {
			merge(node, idx);
			deleteInternal(node->children[idx], k);
		}
	}
	// 从左兄弟（前驱兄弟）借一个关键字
	void borrowFromPrev(BTreeNode<T>* node, int idx) {
		BTreeNode<T>* child = node->children[idx];        // 当前贫困节点
		BTreeNode<T>* sibling = node->children[idx - 1];  // 富有的左兄弟

		// 1. child 节点的所有元素（key 和 children）向后移动一位，腾出 [0] 的位置
		for (int i = child->keyCount - 1; i >= 0; --i) {
			child->keys[i + 1] = child->keys[i];
		}
		if (!child->isLeaf) {
			for (int i = child->keyCount; i >= 0; --i) {
				child->children[i + 1] = child->children[i];
			}
		}

		// 2. 将父节点的 key 降落到 child 的第一个位置
		child->keys[0] = node->keys[idx - 1];

		// 3. 将左兄弟的最后一个子节点指针，挂在 child 的最左侧
		if (!child->isLeaf) {
			child->children[0] = sibling->children[sibling->keyCount];
		}

		// 4. 将左兄弟的最后一个 key 提升到父节点
		node->keys[idx - 1] = sibling->keys[sibling->keyCount - 1];

		// 5. 更新两个节点的关键字数量
		child->keyCount += 1;
		sibling->keyCount -= 1;
	}
	// 从右兄弟（后继兄弟）借一个关键字
	void borrowFromNext(BTreeNode<T>* node, int idx) {
		BTreeNode<T>* child = node->children[idx];        // 当前贫困节点
		BTreeNode<T>* sibling = node->children[idx + 1];  // 富有的右兄弟

		// 1. 将父节点的 key 降落到 child 的最后一个位置
		child->keys[child->keyCount] = node->keys[idx];

		// 2. 将右兄弟的第一个子节点指针，挂在 child 的最右侧
		if (!child->isLeaf) {
			child->children[child->keyCount + 1] = sibling->children[0];
		}

		// 3. 将右兄弟的第一个 key 提升到父节点
		node->keys[idx] = sibling->keys[0];

		// 4. 右兄弟的所有元素（key 和 children）向前移动一位填补空缺
		for (int i = 1; i < sibling->keyCount; ++i) {
			sibling->keys[i - 1] = sibling->keys[i];
		}
		if (!sibling->isLeaf) {
			for (int i = 1; i <= sibling->keyCount; ++i) {
				sibling->children[i - 1] = sibling->children[i];
			}
		}

		// 5. 更新数量
		child->keyCount += 1;
		sibling->keyCount -= 1;
	}
	// ---------------- 删除结构修复模块：合并操作 ----------------
// 参数说明：node 是父节点，idx 是左侧子节点在 children 数组中的索引
	void merge(BTreeNode<T>* node, int idx) {
		BTreeNode<T>* child = node->children[idx];        // 左子节点 (合并后的主体)
		BTreeNode<T>* sibling = node->children[idx + 1];  // 右兄弟节点 (将被合并并销毁)

		// 步骤 1：将父节点中的分隔关键字“降落”到左侧子节点的末尾
		child->keys[child->keyCount] = node->keys[idx];

		// 步骤 2：将右兄弟节点中的所有关键字，依次拼接到左侧子节点的末尾
		for (int i = 0; i < sibling->keyCount; ++i) {
			child->keys[child->keyCount + 1 + i] = sibling->keys[i];
		}

		// 步骤 3：如果不是叶子节点，还需要将右兄弟节点挂载的子树指针一并转移
		if (!child->isLeaf) {
			for (int i = 0; i <= sibling->keyCount; ++i) {
				child->children[child->keyCount + 1 + i] = sibling->children[i];
			}
		}

		// 步骤 4：更新左侧子节点的关键字数量
		// 新数量 = 原数量 + 父节点降落的 1 个 + 右兄弟的数量
		child->keyCount += sibling->keyCount + 1;

		// 步骤 5：修复父节点（填补因关键字降落和子节点合并产生的空缺）
		// 将父节点中 idx 之后的关键字前移一位
		for (int i = idx + 1; i < node->keyCount; ++i) {
			node->keys[i - 1] = node->keys[i];
		}
		// 将父节点中 idx + 1 之后的子树指针前移一位
		for (int i = idx + 2; i <= node->keyCount; ++i) {
			node->children[i - 1] = node->children[i];
		}

		// 步骤 6：更新父节点的关键字数量，并释放右兄弟的内存
		node->keyCount--;
		delete sibling;
	}


	/// <summary>
	/// B+ 树节点定义
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T>
	struct BPlusNode {
		bool isLeaf;
		vector<T> keys;                  // 关键字数组
		vector<BPlusNode*> children;     // 子树指针数组 (仅内部节点使用)
		BPlusNode* next;                 // 叶子节点的链表指针 (仅叶子节点使用)

		int m; // B+ 树的阶 (最大容量)

		BPlusNode(int degree, bool leaf) : m(degree), isLeaf(leaf), next(nullptr) {
			// 在这个流派中，我们不再预先分配固定大小，而是动态管理
			// 但严格保证在内部节点中：keys.size() == children.size()
		}

		// 辅助函数：获取当前节点的最大值
		T getMaxKey() {
			return keys.back();
		}
	};

	template <typename T>
	class BPlusTree {
	private:
		BPlusNode<T>* root;
		int m;

		// 内部递归查找逻辑
		BPlusNode<T>* searchInternal(BPlusNode<T>* node, T key) {
			if (node == nullptr) return nullptr;

			if (node->isLeaf) {
				// 在叶子节点中线性查找（也可用二分）
				for (T k : node->keys) {
					if (k == key) return node;
				}
				return nullptr;
			}
			else {
				// 内部节点查找逻辑：k 个关键字对应 k 个子树
				// keys[i] 代表 children[i] 子树中的最大值
				for (size_t i = 0; i < node->keys.size(); ++i) {
					if (key <= node->keys[i]) {
						return searchInternal(node->children[i], key);
					}
				}
				// 如果比所有关键字都大，通常意味着超出了当前树的最大值边界
				// 在某些实现中会将其路由到最后一个子树，或者直接判定不存在
				return searchInternal(node->children.back(), key);
			}
		}

		// 处理叶子节点的分裂
		// 返回分裂出的新节点，以便父节点将其接入
		BPlusNode<T>* splitLeaf(BPlusNode<T>* leaf) {
			BPlusNode<T>* newLeaf = new BPlusNode<T>(m, true);

			int mid = leaf->keys.size() / 2;

			// 将后半部分数据移动到新叶子节点
			for (size_t i = mid; i < leaf->keys.size(); ++i) {
				newLeaf->keys.push_back(leaf->keys[i]);
			}
			leaf->keys.resize(mid); // 截断原叶子节点

			// 维护叶子节点的双向/单向链表
			newLeaf->next = leaf->next;
			leaf->next = newLeaf;

			return newLeaf;
		}

	public:
		BPlusTree(int degree) : m(degree), root(nullptr) {}

		// 查找操作
		bool search(T key) {
			return searchInternal(root, key) != nullptr;
		}

		// 简化的插入操作 (重点展示 k=k 结构下的分裂处理)
		void insert(T key) {
			if (root == nullptr) {
				root = new BPlusNode<T>(m, true);
				root->keys.push_back(key);
				return;
			}

			// --- 以下为简化版的插入演示，省略了完整的递归回溯逻辑 ---
			// 实际工程中，需要递归地将新节点和新的 MaxKey 插入到父节点中
			if (root->isLeaf) {
				// 维持有序插入
				auto it = lower_bound(root->keys.begin(), root->keys.end(), key);
				root->keys.insert(it, key);

				// 如果超出阶数限制，进行分裂
				if (root->keys.size() > m) {
					BPlusNode<T>* newLeaf = splitLeaf(root);

					// 根节点分裂，树高 +1
					BPlusNode<T>* newRoot = new BPlusNode<T>(m, false);

					// 重点：k 个关键字，k 个子树
					// 父节点的 keys 严格等于其子节点的 getMaxKey()
					newRoot->keys.push_back(root->getMaxKey());
					newRoot->children.push_back(root);

					newRoot->keys.push_back(newLeaf->getMaxKey());
					newRoot->children.push_back(newLeaf);

					root = newRoot;
				}
			}
			// ... (内部节点向下路由及递归分裂逻辑，基于同样的 getMaxKey 更新机制)
		}

		// 层次遍历打印，直观验证 k=k 的结构
		void printTree() {
			if (!root) return;
			vector<BPlusNode<T>*> currentLevel;
			currentLevel.push_back(root);

			int level = 1;
			while (!currentLevel.empty()) {
				cout << "Level " << level << ": ";
				vector<BPlusNode<T>*> nextLevel;

				for (auto node : currentLevel) {
					cout << "[";
					for (size_t i = 0; i < node->keys.size(); ++i) {
						cout << node->keys[i];
						if (i != node->keys.size() - 1) cout << ",";
					}
					cout << "] ";

					if (!node->isLeaf) {
						// 验证：这里可以直接根据 keys.size() 遍历 children
						for (size_t i = 0; i < node->keys.size(); ++i) {
							nextLevel.push_back(node->children[i]);
						}
					}
				}
				cout << endl;
				currentLevel = nextLevel;
				level++;
			}
		}
	};
};
#pragma endregion


#pragma endregion

#pragma region 朴素匹配算法
int ExistSubString(string s, string t)
{
	int i = 0, j = 0, m = s.length(), n = t.length();
	while (i < m && j < n)
	{
		if (s[i] == t[j])
		{
			++i, ++j;
		}
		else
		{
			i = i - j + 1;
			j = 0;
		}
	}
	if (j == n)
		return i - j;
	else return -1;
}
void get_next(string t, vector<int>& next, vector<int>& nextval)
{
	int i = 1, j = 0;
	next[0] = 0;
	int maxLen = 0;
	while (i < t.length())
	{
		if (t[maxLen] == t[i])
		{
			maxLen++;
			next[i] = maxLen;
			i++;
		}
		else
			if (maxLen == 0)
			{
				next[i] = 0;
				i++;
			}
			else
				maxLen = next[maxLen - 1];
	}
	for (int k = 1; k < t.length(); ++k)
	{
		if (t[next[k - 1]] == t[k])
			nextval[k] = nextval[next[k - 1]];
		else
			nextval[k] = next[k];
	}
}
int KMP(string s, string t)
{
	int i = 0, j = 0, m = s.length(), n = t.length();
	vector<int>next(n);
	vector<int>nextval(n);
	get_next(t, next, nextval);
	for (int k = n - 1; k > 0; --k)
	{
		if (t[next[k - 1]] == t[k])
			nextval[k] = nextval[next[k - 1]];
		else
			nextval[k] = next[k];
	}
	while (i < m)
	{
		if (s[i] == t[j])
		{
			++i, ++j;
		}
		else if (j > 0)
			j = nextval[j - 1];
		else
			i += 1;
		if (j == n)
			return i - j;
	}

	return -1;
}
//void get_next(string t, vector<int>& next)
//{
//	int i = 1, j = 0;
//	next[0] = 0;
//	int maxLen = 0;
//	while (i < t.length())
//	{
//		if (j == 0 || t[i] != t[j])
//		{
//			next[i++] = j++;
//		}
//		else
//			j = next[j];
//	}
//}
////KMP算法
//int KMP(string s, string t)
//{
//	int i = 0, j = 0, m = s.length(), n = t.length();
//	vector<int>next(n);
//	get_next(t, next);
//	while (i < m && j < n)
//	{
//		if (j == -1 || s[i] == t[j])
//		{
//			++i, ++j;
//		}
//		else
//		{
//			j = next[j];
//		}
//	}
//	if (j == n)
//		return i - j;
//	else return -1;
//}
#pragma endregion

size_t a = -2200;
typedef int* INT;

/// <summary>
/// 十字链表
/// </summary>
/// <returns></returns>

#pragma region 图
/// <summary>
/// 资源分配图
/// </summary>
typedef struct DistrictNode
{
	int id;
	int size;
	int addr;
	bool status = false;

}DistrictNode, * DistrictTable;
#pragma endregion



//// --- 使用示例 ---
int main51132351() {

	// 实例化一个 3阶 B树 (最多 2 个关键字)
	BTree<int> btree(4);

	//cout << "=== 测试 1: 基础插入 (10, 20) ===" << endl;
	//btree.insert(10);
	//btree.insert(20);
	//btree.printLevelOrder();

	//cout << "=== 测试 2: 触发分裂 (插入 30) ===" << endl;
	//// 预期：根节点分裂，20 上去，10 和 30 成为叶子
	//btree.insert(30);
	//btree.printLevelOrder();

	//cout << "=== 测试 3: 继续插入 (插入 40) ===" << endl;
	//// 预期：40 进入右子树，与 30 放在一起
	//btree.insert(40);
	//btree.printLevelOrder();

	//cout << "=== 测试 4: 内部节点吸收分裂 (插入 50) ===" << endl;
	//// 预期：右子树满载分裂，40 提上去到根节点，根变为 [20, 40]
	//btree.insert(50);
	btree.insert(5);
	btree.insert(6);
	btree.insert(9);
	btree.insert(13);
	btree.insert(8);
	btree.insert(2);
	btree.insert(12);
	btree.insert(15);
	btree.printLevelOrder();

	cout << "=== 测试 5: 查找测试 ===" << endl;
	int target1 = 30;
	int target2 = 99;
	cout << "查找 " << target1 << ": " << (btree.search(target1) ? "命中 (Found)" : "未命中 (Not Found)") << endl;
	cout << "查找 " << target2 << ": " << (btree.search(target2) ? "命中 (Found)" : "未命中 (Not Found)") << endl;
#pragma region 并查集的实例
	//	int n = 10; // 假设有 5 个节点：0, 1, 2, 3, 4
//	DSU dsu(n);
//
//	dsu.unite(0, 1); // 连接 0 和 1
//	dsu.unite(1, 2); // 连接 1 和 2
//	dsu.unite(2, 3); // 连接 1 和 2
//	dsu.unite(1, 4); // 连接 1 和 2
//	dsu.unite(2, 4); // 连接 1 和 2
//DistrictTable table = new DistrictNode[199];
//DistrictNode node1
//{
//	node1.id = 10,
//	node1.size = 10,
//	node1.addr = 10,
//	node1.status = false,
//};
//table[0] = node1;
//	cout << "0 和 2 是否连通: " << (dsu.connected(0, 2) ? "是" : "否") << "\n"; // 输出：是
//	cout << "0 和 3 是否连通: " << (dsu.connected(0, 3) ? "是" : "否") << "\n"; // 输出：否
//
//	cout << "节点 2 所在集合的大小: " << dsu.getSize(2) << "\n"; // 输出：3 (包含0,1,2)
//	dsu.getParent();
//
#pragma endregion
#pragma region KMP算法示例
	//string s = "I love China forever!";
////vector<int> next = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
//
//string t = "China forever!";
//string res;
//string t1 = "china";
//string t2 = "d陈博涵是dad大傻逼";
//vector<int> next(t2.length());
//vector<int> nextval(t2.length());
//string s2 = "vehnovhechewiomochemod陈博涵是dad大傻逼chenbosadhaswfsagghanb";
//cout << StrCompare(s, t) << endl;
//
//cout << MyIndex(s, t) << endl;
//
//cout << ExistSubString(s, t) << endl;
//get_next(t2, next, nextval);
//print(next);
//print(nextval);
//
//cout << KMP(s2, t2) << endl;
//SubString(s2, res, 21, t2.length());
//cout << res << endl;
//cout << t1.length() << endl;
//cout << t1.size() << endl;
#pragma endregion

#pragma region 二叉树的带权路径长度
	/*BinNode* root = CreateNode(1);
	root->left = CreateNode(2);
	root->right = CreateNode(3);
	root->left->left = CreateNode(4);
	root->left->right = CreateNode(5);
	root->right->left = CreateNode(6);
	root->right->right = CreateNode(7);
	cout << CalSum(root, 1) << endl;*/
#pragma endregion

	
	return 0;
}

#pragma region 5进制和2进制输出在一块
//int x = 5, y = 2;
//vector<int> p = { 1,1,3,2,2,1,1,0,1,0,0,0,1,0,1 };
//stack<int> s;
//int pSize = p.size();
//vector<long long>dpY(pSize);
//vector<long long>dpX(pSize);
//dpX[0] = p[0];
//for (int i = 1; i < pSize; ++i)
//{
//	dpX[i] = dpX[i - 1] * x + p[i];
//}
//dpY[pSize - 1] = p[pSize - 1];
//long long power = y;
//for (int j = pSize - 2; j >= 0; --j)
//{
//	dpY[j] = p[j] * power + dpY[j + 1];
//	power *= y;
//}
//bool isFound = false;
//for (int i = 0; i < pSize - 1; ++i)
//{
//	if (dpX[i] == dpY[i + 1]) {
//		cout << "dpX[" << i << "] = dpY[" << i + 1 << "] = " << dpX[i] << endl;
//		isFound = true;
//	}
//	if (dpX[i + 1] == dpY[i]) {
//		cout << "dpX[" << i + 1 << "] = dpY[" << i << "] = " << dpX[i + 1] << endl;
//		isFound = true;
//	}
//}
//if (!isFound) {
//	cout << "未找到满足条件的元素对。" << endl;
//}
//else
//{
//	print(dpX);
//	print(dpY);
//}

#pragma endregion







