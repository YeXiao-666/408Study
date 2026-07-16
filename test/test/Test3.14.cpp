#include<iostream>
#include<vector>
#include<algorithm>
#include "Utils.hpp"
#include <random>
#include <queue>
#include <set>
#include <map>
#include <list>
#include <string>
#include <iomanip>
#include <unordered_map>
#include<memory>
#include<cstring>
#include<array>

using namespace std;

#include<ctime>
#include<fstream>
#include <any>
#include <functional>
#include <stack>
extern const int INF;
#pragma region 最长公共子串
// 辅助函数：计算两个字符串的最长公共子串长度 (Longest Common Substring)
int getLCSLength(const string& s1, const string& s2) {
	int maxLen = 0;
	int n = s1.length();
	int m = s2.length();
	//dp[i][j]表示以s[i-1], s[j-1]为结尾的最长连续子串的长度
	vector<vector<int>> dp(n + 1, vector<int>(m + 1));

	for (int i = 1; i < n + 1; ++i)
	{
		for (int j = 1; j < m + 1; ++j)
		{
			if (s1[i - 1] == s2[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
			maxLen = maxLen > dp[i][j] ? maxLen : dp[i][j];
		}
	}

	return maxLen;
}

//空间压缩（一维滚动数组）
//时间复杂度不变仍为 O(NM)，但空间复杂度从 O(N M) 骤降到 O(M)，
// 内存消耗极小，且缓存命中率（Cache Locality）极高，实际运行速度会快很多
int getLCSLength1(const string& s1, const string& s2)
{
	int maxLen = 0;
	int n = s1.length();
	int m = s2.length();

	// 空间优化：只需要一维数组，空间复杂度降为 O(M)
	vector<int> dp(m + 1, 0);

	for (int i = 1; i <= n; ++i) {
		// 关键：必须从后往前倒序遍历 j！
		for (int j = m; j >= 1; --j) {
			if (s1[i - 1] == s2[j - 1]) {
				dp[j] = dp[j - 1] + 1;
				maxLen = max(maxLen, dp[j]); // 内置的 max 通常比三元运算符更具可读性
			}
			else {
				dp[j] = 0; // 不相等时必须清零，因为复用了一维数组
			}
		}
	}

	return maxLen;
}

//滑动窗口法（双指针）—— 极致的O(1) 空间
//空间复杂度可以直接降为 O(1)（零额外空间分配），时间复杂度依然是 O(NM)。
int getLCSLength2(const string& s1, const string& s2)
{
	int n = s1.length(), m = s2.length();
	int maxLen = 0;

	// 遍历所有可能的对齐方式
	// 想象 s2 贴着 s1 从左向右滑动
	for (int i = 0; i < n; ++i) {
		int len = 0;
		for (int j = 0; i + j < n && j < m; ++j) {
			if (s1[i + j] == s2[j]) {
				maxLen = max(maxLen, ++len);
			}
			else {
				len = 0;
			}
		}
	}
	// 想象 s1 贴着 s2 从左向右滑动
	for (int j = 1; j < m; ++j) {
		int len = 0;
		for (int i = 0; i < n && j + i < m; ++i) {
			if (s1[i] == s2[j + i]) {
				maxLen = max(maxLen, ++len);
			}
			else {
				len = 0;
			}
		}
	}
	return maxLen;
}

/// <summary>
/// //解法一：后缀自动机(Suffix Automaton, SAM)时间复杂度： $O(| S1 | +| S2 | ) —— 真正的线性时间！
// 空间复杂度： O(| S1 | \times | \Sigma | )，其中  | \Sigma |  是字符集大小。
/// </summary>
/// <param name="s1"></param>
/// <param name="s2"></param>
/// <returns></returns>
// SAM 节点结构体
struct State {
	int len;      // 当前状态所代表的最长子串长度
	int link;     // 后缀链接 (失败时的回退指针)
	int next[128]; // 状态转移数组 (假设字符集为所有 ASCII 字符)
};

const int MAXLEN = 100005; // 假设单个字符串最大长度为 10w
State st[MAXLEN * 2];      // SAM 的状态数最多是 2N - 1
int sz, last;

// 初始化 SAM
void sam_init() {
	st[0].len = 0;
	st[0].link = -1;
	memset(st[0].next, 0, sizeof(st[0].next));
	sz = 1;
	last = 0;
}

// 向 SAM 中逐步添加字符 (构建过程)
void sam_extend(char c) {
	int cur = sz++;
	st[cur].len = st[last].len + 1;
	memset(st[cur].next, 0, sizeof(st[cur].next));

	int p = last;
	while (p != -1 && !st[p].next[c]) {
		st[p].next[c] = cur;
		p = st[p].link;
	}

	if (p == -1) {
		st[cur].link = 0;
	}
	else {
		int q = st[p].next[c];
		if (st[p].len + 1 == st[q].len) {
			st[cur].link = q;
		}
		else {
			int clone = sz++;
			st[clone].len = st[p].len + 1;
			memcpy(st[clone].next, st[q].next, sizeof(st[q].next));
			st[clone].link = st[q].link;
			while (p != -1 && st[p].next[c] == q) {
				st[p].next[c] = clone;
				p = st[p].link;
			}
			st[q].link = st[cur].link = clone;
		}
	}
	last = cur;
}

int getLCSLength_SAM(const string& s1, const string& s2) {
	sam_init();
	// 1. 对 s1 构建后缀自动机
	for (char c : s1) {
		sam_extend(c);
	}

	int maxLen = 0;
	int v = 0; // 当前在 SAM 中的状态节点
	int current_len = 0; // 当前匹配的长度

	// 2. 将 s2 放在 SAM 上跑匹配
	for (char c : s2) {
		// 如果当前节点没有对应的字符转移，就沿着 link 回退
		while (v != 0 && !st[v].next[c]) {
			v = st[v].link;
			current_len = st[v].len; // 长度缩减为回退后状态的最大长度
		}

		// 如果有转移，则继续走
		if (st[v].next[c]) {
			v = st[v].next[c];
			current_len++;
		}

		// 更新全局最大值
		maxLen = max(maxLen, current_len);
	}
	return maxLen;
}



// 辅助函数：构建后缀数组 (基于 O(N log^2 N) 的简单倍增法)
vector<int> build_SA(const string& s) {
	int n = s.length();
	vector<int> sa(n), rk(n), tmp(n);
	for (int i = 0; i < n; ++i) {
		sa[i] = i;
		rk[i] = s[i];
	}

	// 倍增法排序
	for (int k = 1; k < n; k *= 2) {
		auto cmp = [&](int a, int b) {
			if (rk[a] != rk[b]) return rk[a] < rk[b];
			int ra = a + k < n ? rk[a + k] : -1;
			int rb = b + k < n ? rk[b + k] : -1;
			return ra < rb;
			};
		sort(sa.begin(), sa.end(), cmp);
		tmp[sa[0]] = 0;
		for (int i = 1; i < n; ++i) {
			tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
		}
		rk = tmp;
	}
	return sa;
}

// 辅助函数：Kasai 算法计算 LCP (Height 数组)
vector<int> build_LCP(const string& s, const vector<int>& sa) {
	int n = s.length();
	vector<int> rk(n), lcp(n, 0);
	for (int i = 0; i < n; ++i) rk[sa[i]] = i;

	int h = 0;
	for (int i = 0; i < n; ++i) {
		if (rk[i] > 0) {
			int j = sa[rk[i] - 1];
			while (i + h < n && j + h < n && s[i + h] == s[j + h]) h++;
			lcp[rk[i]] = h;
			if (h > 0) h--;
		}
	}
	return lcp;
}

/// <summary>
/// 解法二：后缀数组 (Suffix Array) + LCP时间复杂度： 构建 SA 的复杂度通常是 O(L \log^2 L) 或 O(L \log L)
/// Kasai 算法求 LCP 是 O(L)。
/// 总体为 O(L \log L) 级别，其中 $L = |S1| + |S2|。空间复杂度： O(L)
/// </summary>
/// <param name="s1"></param>
/// <param name="s2"></param>
/// <returns></returns>
int getLCSLength_SA(const string& s1, const string& s2) {
	int len1 = s1.length();
	// 1. 拼接字符串，使用一个不可能出现的字符作分隔符
	string S = s1 + "#" + s2;

	// 2. 构建后缀数组和 LCP 数组
	vector<int> sa = build_SA(S);
	vector<int> lcp = build_LCP(S, sa);

	int maxLen = 0;
	int n = S.length();

	// 3. 遍历 LCP 数组，寻找分别属于 s1 和 s2 的相邻后缀
	for (int i = 1; i < n; ++i) {
		int pos1 = sa[i];
		int pos2 = sa[i - 1];

		// 判断两个后缀的起始位置是否分布在 '#' 的两侧
		// pos < len1 表示属于 s1，pos > len1 表示属于 s2
		if ((pos1 < len1 && pos2 > len1) || (pos1 > len1 && pos2 < len1)) {
			maxLen = max(maxLen, lcp[i]);
		}
	}

	return maxLen;
}
#pragma endregion

#pragma region Function的优缺点
// 假设我们要定义一个值类型为 int 的哈希表别名，键的类型作为模板参数
// 使用 using 非常简单自然：
template <typename Key>
using IntMap = std::unordered_map<Key, int>;

// typedef 无法直接作用于模板，必须要在外层套一个 struct 才能勉强实现（极其繁琐，这里就不演示了）。
template <typename Key>
struct S
{
	typedef unordered_map<Key, int> IntMap;
};

/// <summary>
	/// std::function特点
	/// （1）是 C++11 引入的通用多态函数封装器。它的作用用一句话概括就是：
	/// 它可以装下任何“可以像函数一样调用的东西”（Callable 对象），只要签名匹配。
	//在你的代码 std::function<std::unique_ptr<Animal>()> 中：
	//（2）尖括号里的 < ... > 定义了这个函数的签名（Signature）。
	//std::unique_ptr<Animal> 是这个函数的返回值类型。
	//最后的() 表示这个函数不接收任何参数。
	// 
	//缺点 ： （1）在游戏引擎的底层循环（比如每秒执行 60 次，每次遍历上万个实体的 Tick 函数）或高频量化交易系统中，
	// std::function 的性能开销会被无限放大，成为“性能杀手”。
	// 
	//为什么 std::function 会慢？
	//std::function 的“慢”主要来源于为了实现其“万能容器”特性而付出的代价（类型擦除）：

	//（2）不可控的堆内存分配（Heap Allocation）：
	//虽然标准库对 std::function 实现了小对象优化（SBO, Small Buffer Optimization）
	// ——如果你的 Lambda 捕获的状态很少（通常小于 16 或 32 字节），它会直接存在栈上。
	// 但如果你的 Lambda 捕获了大量变量（比如几个 std::string 或大结构体），SBO 就会失效，
	// std::function 会在幕后偷偷调用 new 进行堆分配。在主循环中频繁 new / delete 会导致严重的内存碎片和 Cache Miss。

	//（3）间接调用与内联阻断（Indirect Call & Inlining Barrier）：
	//std::function 底层通过虚函数指针（或函数指针数组）来实现类型擦除。这意味着每次调用都需要通过指针进行间接跳转。
	// 更致命的是，这会彻底打断编译器的内联（Inlining）优化。编译器无法在编译期知道具体执行的是哪段代码，自然无法将代码展开。

	//《1》方案 1：模板参数（Compile - time Polymorphism）—— 零开销、极致内联
	//适用场景：将回调函数作为参数传递给一个算法或通用函数，且不需要将它们存储在容器（如 std::vector）中。
	//原理：利用模板让编译器在编译期知道具体的 Callable 类型，直接生成内联代码。

// 慢：使用 std::function（会产生间接调用，可能分配堆内存）
void ProcessSlow(const std::vector<int>& data, const std::function<void(int)>& callback) {
	for (int v : data) callback(v);
}

// 快：使用模板（零开销，极大概率被编译器内联展开）
template <typename Callable>
void ProcessFast(const std::vector<int>& data, Callable callback) {
	for (int v : data) callback(v);
}

void test_templates() {
	std::vector<int> data = { 1, 2, 3 };
	// 编译器会为这个特定的 Lambda 生成一个专属的 ProcessFast 版本，没有任何运行时开销
	ProcessFast(data, [](int v) { std::cout << v * 2 << " "; });
}
//《2》方案 2：std::function_ref(C++26) —— 不拥有所有权的高效传递
//适用场景：同样是向下传递回调函数，但你不想把核心函数写成模板（为了避免代码膨胀或头文件依赖），又想避免 std::function 的堆分配。
//原理：它是一个轻量级的、 * *非拥有（Non - owning） * *的视图类型，底层仅保存一个对象指针和一个函数指针。
// 它绝不分配内存。（如果你还在用 C++17 / 20，可以使用 Google Abseil 库的 absl::FunctionRef 或自己手撸一个）。
// 不用模板，编译期固定签名，但由于是 reference，绝不触发堆分配
//void ProcessWithRef(int value, std::function_ref<void(int)> callback) {
//	callback(value);
//}
//
//void test_function_ref() {
//	int multiplier = 10; // 捕获局部变量
//	// 直接传递，安全且极速（只要 callback 的生命周期不长于 Lambda）
//	ProcessWithRef(5, [&](int v) { std::cout << v * multiplier; });
//}

/// </summary>
	//using CreatorFunc = std::function<std::unique_ptr<Animal>()>; // 无参返回值为unique_ptr<Animal>类型
	//using MethodFunc = std::function<void(Animal*)>;// 无参返回值为(Animal*)类型
	//typedef std::function<std::unique_ptr<Animal>()> CreatorFunc; //等价using声明方式
	//typedef std::function<void(Animal*)> MethodFunc;

#pragma endregion

#pragma region 数学逼近问题
//double fx(double a, double k)
//{
//	if (a > pow(k, 2) - 0.5) return a;
//	return fx(k * pow(a, 0.5f), k);
//}
//double k;
//
//while (cin >> k && k)
//{
//	cout << fx(1, k) << endl;
//}
//cout << "k = 0 程序结束！" << endl;
#pragma endregion

#pragma region Prac

#pragma region 匿名类，Lambda表达式的使用
class MyCmp
{
public:
	bool operator()(const pair<int, int>& a, const pair<int, int>& b) const
	{
		return a.second > b.second;
	}
};

class Solution {
public:
	static bool cmp1(pair<int, int>& a, pair<int, int>& b)
	{
		return a.second > b.second;
	};
	vector<int> topKFrequent(vector<int>& nums, int k) {

		auto cmp = [](pair<int, int>& a, pair<int, int>& b) {
			return a.second > b.second; // 频率小的优先出队（小顶堆）
			};
		unordered_map<int, int>map;
		for (int num : nums)
		{
			map[num]++;
		}
		//priority_queue<pair<int,int>, vector<pair<int, int>>,bool(*)(pair<int,int>&, pair<int,int>&)> q(cmp);
		//priority_queue<pair<int,int>, vector<pair<int, int>>,bool(*)(pair<int,int>&, pair<int,int>&)> q(cmp1);
		//priority_queue<pair<int,int>, vector<pair<int, int>>,decltype(&cmp1)> q(cmp1);
		//priority_queue<pair<int,int>, vector<pair<int, int>>,decltype(cmp)> q(cmp);
		priority_queue<pair<int, int>, vector<pair<int, int>>, MyCmp> q;
		for (auto& [num, cnt] : map)
		{
			q.push({ num, cnt });
			if (q.size() > k)
			{
				q.pop();
			}
		}
		vector<int>res;
		while (!q.empty())
		{
			int top = q.top().first;
			q.pop();
			res.push_back(top);
		}
		return res;
	}
};
#pragma endregion

#pragma region STL容器迭代器删除元素
/// <summary>
	/// erase迭代器删除方法
	/// </summary>
	//std::vector< int> v = { 1, 2, 3, 4 };
	//std::cout << "Size before erase all: " << v.size() << '\n'; // 0
	//for (auto it = v.begin(); it != v.end(); ) {
	//	it = v.erase(it);
	//}
	//std::cout << "Size after erase all: " << v.size() << '\n'; // 0
	//for (int num : v)
	//{
	//	cout << num << " ";
	//}

	//for (int i = static_cast<int>(v.size()) - 1; i >= 0; --i) {

	//		v.erase(v.begin() + i);
	//	
	//}
	//std::cout << "Size after erase all: " << v.size() << '\n'; // 0
#pragma endregion

#pragma region C++文件IOStream
/*ofstream outFile;
	outFile.open("testC++.txt");
	outFile << " [ " << endl
		<< "    \"quote\": \"The world as we have created it is a process of our thinking. It cannot be changed without changing our thinking.\"," << endl
		<< "    \"author\": \"Albert Einstein\"" << endl
		<< "    \"quote\": \"It is our choices, Harry, that show what we truly are, far more than our abilities.\"," << endl
		<< "    \"author\": \"J.K. Rowling\"" << endl
		<< "]" << endl;*/
		/*ifstream inFile;
		char fileName[20];
		cin.getline(fileName, 20);
		inFile.open(fileName);
		if (!inFile.is_open())
		{
			cout << "Could not open the file!!" << endl;
			exit(EXIT_FAILURE);
		}
		cout << "Success Open the file" << endl;
		string s;
		inFile >> s;
		int count = 0;
		while (inFile.good())
		{
			++count;
			cout << s;
			inFile >> s;
		}
		if (inFile.eof())cout << "End of file reached" << endl;
		else if(inFile.fail()){
			cout << "Input terminated by data mismatched." << endl;
		}
		else
		{
			cout << "Input terminated by unknown reason" << endl;
		}
		if(count == 0)cout << "No data left!" << endl;
		else cout << "Item read		:"  << count << endl;
		inFile.close();*/
		//array<int, 4> arr = { 1, 2, 3 , 4 };
		///*int* num = new int[3];
		//num[0] = 1;
		//num[1] = 2;
		//num[2] = 3;
		//cout << &num << endl;
		//cout << num << endl;*/
		//int x = 10;
		//{
		//	cout << x << endl;
		//	int x = 100;
		//	cout << x << endl;
		//}
		//float secs;
		//cin >> secs;
		//clock_t delay = secs * CLOCKS_PER_SEC;
		//clock_t start = clock();
		//while (clock() - start < delay);

		//cout << "done!" << endl;
#pragma endregion

#pragma region 左值右值的声明
//int n;
		//// 以下的p、b、c、*p都是左值
		//int* p = new int(0);
		//int b = 1;
		//const int c = 2;
		//*p = 10;
		//string s("11111111");
		//s[0];

		////左值引用给左值取别名
		//int& r1 = b;
		//int*& r2 = p;
		//cout << r2  << " r2 == " << *r2 << endl;
		//int& r3 = *p;
		//cout << r3  << endl;
		//string& r4 = s;

		////左值引用引用给右值取别名
		//const int& rx1 = 10;
		//const double& rx2 = b + c;
		//const double& rx3 = fmin(c, b);
		//const string&& rx4 = string("2222222");

		////右值引用给右值取别名
		//int&& rr1 = 10;
		//double&& rr2 = b + c;
		//double&& rr3 = fmin(b, c);
		//cout << "int&& rr1 = " << rr1 << endl;

		////右值引用引用给左值取别名
		//int&& rrx1 = move(b);
		//int*&& rrx2 = move(p);
		//int&& rrx3 = move(*p);
		//string&& rrx4 = move(s);
#pragma endregion

#pragma region map遍历插入
//map<string, int> map;
		//map.insert({ "zhaoyun", 23 });
		//map.insert(make_pair("zhangfei", 28));
		//map.emplace("liubei", 50);
		//map["zhugeliang"] = 90;
		////for (auto& it : map)cout << "name :" << it.first << " years : " << it.second << endl;
		//for (auto&[name, years] : map)cout << "name :" << name << " years : " << years << endl;	
		/*list<int> v{ 1, 2, 3 };
		list<int> v1{ 10, 20, 30 };
		list<int>::iterator it = v.begin();
		v.splice(it, v1);
		for (int& n : v)
			cout << n << " ";*/
#pragma endregion

#pragma region 智能指针

class Cat
{
public:
	string Name;
	char* addr;
	void Sound()
	{
		char addrN[] = "222-B S.H.";
		addr = new char[strlen(addrN) + 1];
		strcpy_s(addr, strlen(addrN) + 1, addrN);
		cout << "miaomiao" << addr << endl;
	}
	Cat(string name, const char* a) :Name(name)
	{
		addr = new char[strlen(a) + 1];// 分配足够内存存储字符串（包括结尾的 '\0'）
		strcpy_s(addr, strlen(a) + 1, a);
		/*this->Name = name;*/
		cout << this->Name + "被创建了! address : " + addr << endl;
	}
	~Cat()
	{
		cout << this->Name + "被销毁了!" << endl;
		if (addr != NULL)
		{
			delete[] addr;
			cout << this->Name + "的 address : " + addr << endl;
			cout << &addr << endl;
			addr = NULL;
		}
	}
};
unique_ptr<Cat> catSound(unique_ptr<Cat> c)
{
	c->Sound();
	return c;
}
void party(shared_ptr<Cat> p)
{
	cout << "party: " << p.use_count() << endl;
}
/// <summary>
/// std::unique_ptr
/// (1)内存独占	
/// (2)生命周期绑定(RALL Resource Acquisition Is Initialization)
/// 其核心思想是将资源的生命周期绑定到对象的生命周期，
/// 通过对象的构造函数来获取资源，通过对象的析构函数来释放资源。
/// 这种方式避免了显式的资源管理，减少了资源泄漏的可能性。
/// 功能：
///		<1>内存管理：通过智能指针（如 std::unique_ptr 和 std::shared_ptr）自动管理堆内存。
///		<2>文件管理：通过 std::fstream 自动管理文件的打开和关闭。
///		<3>互斥锁管理：通过 std::lock_guard 或 std::unique_lock 自动管理锁的获取和释放。
/// (3)零开销（Zero OverHead）：和裸指针一样块，编译后的指令也几乎一样，几乎不拖慢速度
/// </summary>


/// <summary>
/// std::shared_ptr : 核心机制：引用计数， 计数器为0时，自动delete释放该块内存。
/// 好处：（1）更优雅（2）使用new需要申请2次内存，一次给对象，一次给计数器，但shared_ptr只申请一次内存
/// 它将对象和计数器打包，效率更高，内存碎片更少。
/// （3）可被复制，即该内存引用次数加1，生命周期延长，
/// 缺点：（1）内存泄露，循环引用（Circle Reference）
/// (2)性能代价，内存比裸指针大两倍（对象和控制块），
///			速度：引用计数加减是原子操作（Atomic Operation），为保证多线程安全比普通整数加减慢
/// （3）管理的对象不是线程安全的，只管对象的生命周期，不管对象的数据竞争（需要加锁（Mutex))；
/// </summary>
/// <returns></returns>

//内存泄露，循环引用
class A;
class B
{
public:
	shared_ptr<A> b;
	~B() { cout << "B out" << endl; }
};
class A
{
public:
	shared_ptr<B> a;
	~A() { cout << "A out" << endl; }
};

/// <summary>
/// std::weak_ptr:特点
/// （1）不能独立存在，通常是std::shared_ptr的跟班，必须从std::shared_ptr或者另一个weak_ptr 变出来
/// （2）不可直接访问对象的数据方法, (因为其不控制对象的生命周期)可以先确定对象是否存在，
/// 再通过lock()升级为强引用 
/// </summary>
/// <returns></returns>
void check(weak_ptr<int> wp)
{
	if (shared_ptr<int> temp = wp.lock())
	{
		cout << " it exists still val :" << *temp << endl;
	}
	else cout << " it has died " << endl;
}

class Child; // 前向声明
class Parent
{
public:
	shared_ptr<Child> child;
	~Parent() { cout << "Parent out" << endl; }
};

class Child
{
	// 注意：这里去掉了 public Parent，子类不需要继承父类
public:
	weak_ptr<Parent> parent; // 使用 weak_ptr 打破循环
	~Child() { cout << "Child out" << endl; }
};

//// 1. 创建对象，此时 a1 和 b1 的引用计数均为 1
//shared_ptr<Parent> a1 = make_shared<Parent>();
//shared_ptr<Child> b1 = make_shared<Child>();

//// 2. 建立关系
//a1->child = b1; // b1 的引用计数变为 2 (因为 child 是 shared_ptr)
//b1->parent = a1; // a1 的引用计数依然是 1 (因为 parent 是 weak_ptr，直接赋值即可)

// 3. 退出作用域时：
// - a1 销毁，Parent 的引用计数从 1 降到 0，触发 ~Parent()。
// - Parent 被销毁，它内部的 child (shared_ptr) 也会随之销毁，b1 的引用计数从 2 降到 1。
// - b1 销毁，Child 的引用计数从 1 降到 0，触发 ~Child()。
// 内存完美释放，不产生泄露。;


void IntellegentPointer()
{
	//内存泄露，循环引用
	shared_ptr<A> a = make_shared<A>();
	shared_ptr<B> b = make_shared<B>();
	a->a = b;
	b->b = a;

	shared_ptr<Cat> c = make_shared<Cat>("Coffee", " MoMo House");
	cout << "ref count : " << c.use_count() << endl;
	shared_ptr<Cat> d = c;
	cout << "after copy ref count :  " << c.use_count() << endl;
	party(c);

	unique_ptr<Cat> uCat = make_unique<Cat>("mimi", "221-B");
	uCat->Sound();
	cout << uCat->Name + " will leave soon" << endl;
	unique_ptr<Cat> ca = catSound(move(uCat));
	Cat* cat = ca.get();
	if (uCat == NULL)cout << "mimi has died" << endl;
	else cout << "mimi still live" << endl;

	weak_ptr<int> wp1;
	{
		shared_ptr<int> sp = make_shared<int>(100);
		wp1 = sp;
		check(wp1);
	}//超出{}的作用域，sp已经结束生命
	check(wp1);

	shared_ptr<Cat> cp = make_shared<Cat>("", "");
	weak_ptr<Cat> wp = cp;
	cout << cp.use_count() << endl;
}

#pragma endregion
#pragma region 完美转发，万能引用


//6.2 forward 完美转发在传参的过程中保留对象原生类 型属性
//在下面的示例中，准备了各种形式的左值和右值，但是我们要在传参的过程中保留对象的原来的属性，就要加上forward。
void Fun(int& x) { cout << "左值引用" << endl; }
void Fun(const int& x) { cout << "const 左值引用" << endl; }
void Fun(int&& x) { cout << "右值引用" << endl; }
void Fun(const int&& x) { cout << "const 右值引用" << endl; }

//6.1 模板中的 && 万能引用
//(1) 下面代码的模板中的 && 不代表右值引用，而是万能引用，其既能接收左值又能接收右值。
//
//(2) 模板的万能引用只是提供了能够接收同时接收左值引用和右值引用的能力，但是引用类型的唯一作用就是限制了接收的类型，后续使用中都退化成了左值，我们希望能够在传递过程中保持它的左值或者右值的属性, 就需要用我们下面学习的完美转发。

template<typename T>//模板中的 && 不代表右值引用，而是万能引用，其既能接收左值又能接收右值。
void PerfectForward(T&& t)
{
	Fun(forward<T>(t));
}

//template<typename T>
//void PerfectForward(T&& t)
//{
//	Fun(t);
//}
void PerfectTransfer()
{
	PerfectForward(10);           // 右值

	int a;
	PerfectForward(a);            // 左值

	PerfectForward(std::move(a)); // 右值

	const int b = 8;
	PerfectForward(b);             // const 左值

	PerfectForward(std::move(b));  // const 右值

	cout << "完美转发 : PerfectTransfer " << endl;
}

#pragma endregion


#pragma region 函数指针

typedef void (*funcptr)(int);  // funcptr 的类型是 void (*)(int)

void func(int a)
{
	cout << "func " << a << endl;
}


//这是一个变量声明。
//它声明了一个变量 funcPtr，其类型是 函数指针，指向“参数为 2 个 int，返回 int” 的函数。
//注意 * funcPtr 两边的括号是必需的，它们让 * 先和 funcPtr 结合，表示指针，然后才是函数调用符。
//因此 funcPtr 是指针变量，不是函数。	
int (*funcPtr)(int, int);

// 定义一个函数
int add(int a, int b) {
	return a + b;
}

// 另一个函数，返回一个函数指针
int (*getAddFunctionPointer())(int, int) {
	return &add;
}

void add1(int a, int b, funcptr ptr)
{
	ptr(a + b);
}
int add1(int a, int b)
{
	return a + b;
}

void FuncPtrTest()
{
	funcptr ptr = &func;
	ptr(10); //相当于调用 myFunction(10);

	// 获取 add 函数的函数指针
	funcPtr = getAddFunctionPointer();
	int (*MyfuncPtr)(int, int) = add;

	// 通过函数指针调用函数
	int result = funcPtr(3, 4);  // 相当于调用 add(3, 4)，result 等于 7

	cout << result << "   " << funcPtr << "   " << &add << endl;
	cout << MyfuncPtr(5, 9) << "   " << MyfuncPtr << "   " << &add << endl;
	add1(5, 5, [](int sum)
		{
			cout << " lambda : " << sum << endl;
		});//回调函数，定义其主函数类似Action，
	add1(5, 5, ptr);
	//cout << typeid(ptr).name()<<endl;
}
#pragma endregion


#pragma endregion


#pragma region mihoyo3.14锯齿形数组
//int n;
//cin >> n;
//if (n < 2)
//{
//	cout << 0;
//	return;
//}
//vector<int> nums(n);
//
//for (int i = 0; i < n; ++i)cin >> nums[i];
//
//int preDiff = 0, curDiff = 0;
//int cnt = 1;
//for (int i = 1; i < n; ++i)
//{
//	curDiff = nums[i] - nums[i - 1];
//	if (curDiff > 0 && preDiff <= 0)
//	{
//		cnt++;
//		preDiff = curDiff;
//	}
//	else if (curDiff < 0 && preDiff >= 0)
//	{
//		cnt++;
//		preDiff = curDiff;
//	}
//	else
//	{
//
//	}
//
//}
//cout << n - cnt;

#pragma endregion

#pragma region mihoyohard树路径的异或和
//typedef long long ll;
//const int MOD = 1e9 + 7;
//
//struct Edge {
//	int to;
//	int weight;
//};
//
//void solve() {
//	int n;
//	if (!(cin >> n)) return;
//
//	vector<vector<Edge>> adj(n + 1);
//	for (int i = 0; i < n - 1; ++i) {
//		int u, v, w;
//		cin >> u >> v >> w;
//		adj[u].push_back({ v, w });
//		adj[v].push_back({ u, w });
//	}
//
//	// d[i] 存储根节点（节点1）到节点 i 的路径异或和
//	vector<int> d(n + 1, 0);
//	vector<int> q;
//	vector<bool> vis(n + 1, false);
//
//	// BFS 计算所有节点到根的异或距离
//	q.push_back(1);
//	vis[1] = true;
//	int head = 0;
//	while (head < q.size()) {
//		int u = q[head++];
//		for (auto& edge : adj[u]) {
//			if (!vis[edge.to]) {
//				vis[edge.to] = true;
//				d[edge.to] = d[u] ^ edge.weight;
//				q.push_back(edge.to);
//			}
//		}
//	}
//
//	ll total_sum = 0;
//	// 题目 w 范围到 10^9，考虑 31 位即可
//	for (int k = 0; k < 31; ++k) {
//		ll count1 = 0;
//		for (int i = 1; i <= n; ++i) {
//			if ((d[i] >> k) & 1) {
//				count1++;
//			}
//		}
//		ll count0 = n - count1;
//
//		// 这一位贡献的路径数是 count0 * count1
//		ll paths_with_bit_k = (count0 * count1) % MOD;
//		ll contribution = (paths_with_bit_k * ((1LL << k) % MOD)) % MOD;
//		total_sum = (total_sum + contribution) % MOD;
//	}
//
//	cout << total_sum << endl;
//}
//
//int main3124123() {
//	ios::sync_with_stdio(false);
//	cin.tie(nullptr);
//
//	int t;
//	if (!(cin >> t)) return 0;
//	while (t--) {
//		solve();
//	}
//	return 0;
//}
#pragma endregion

#pragma region mihoyoHard 找出数组中两数相乘为target
//int n;
//cin >> n;
//vector<int> nums(n);
//unordered_multimap<int, int> seen;
//for (int i = 0; i < n; ++i)
//{
//	cin >> nums[i];
//	seen.insert(make_pair(nums[i], i));
//}
//int q;
	//cin >> q;
	//while (q--) {
	//	int x;
	//	cin >> x;
	//	int ni = -1, nj = -1;
	//	for (int i = 0; i < n; ++i) {
	//		int a = nums[i];
	//		if (x % a != 0) continue; // 必须整除
	//		int target = x / a;
	//		auto it = seen.find(target);
	//		if (it != seen.end() && it->second != i) {
	//			ni = (i < it->second) ? i : it->second;
	//			nj = (i < it->second) ? it->second : i;
	//			break;
	//		}
	//	}
	//	if (ni != -1) {
	//		cout << ni + 1 << " " << nj + 1 << endl;
	//	}
	//	else {
	//		cout << "-1 -1" << endl;
	//	}
	//}


//int n;
//cin >> n;
//vector<float>nums(n);
//for (int i = 0; i < n; ++i)
//	cin >> nums[i];
//int q;
//cin >> q;
//while (q--)
//{
//	float x;
//	cin >> x;
//	int ni = -1, nj = -1;
//	for (int i = 0; i < n; ++i)
//	{
//		float target = x / nums[i];
//
//		auto it = find(nums.begin(), nums.end(), target);
//		if (it != nums.end())
//		{
//			ni = i;
//			nj = distance(nums.begin(), it);
//			if (nj == i)
//			{
//				it = find(nums.begin() + i + 1, nums.end(), target);
//				nj = distance(nums.begin(), it);
//				break;
//			}
//			break;
//		}
//
//	}
//
//	cout << ((ni != -1) ? ni + 1 : -1) << " " << ((nj != -1) ? nj + 1 : -1) << endl;
//}
#pragma endregion

#pragma region 分割回文串
//vector<vector<string>> partition(string s) {
//	int count = 0;
//	vector<string>cur;
//	vector<vector<string>> res;
//	backtrack(s, 0, cur, res);
//	return res;
//}
//void backtrack(const string& s, int start, vector<string>& cur, vector<vector<string>>& res)
//{
//	if (start == s.size())
//	{
//		res.push_back(cur);
//		return;
//	}
//	for (int end = start; end < s.size(); ++end)
//	{
//		if (isPanlidrome(s, start, end))
//		{
//			cur.push_back(s.substr(start, end - start + 1));
//			backtrack(s, end + 1, cur, res);
//			cur.pop_back();
//		}
//	}
//
//}
//bool isPanlidrome(const string& s, int l, int r)
//{
//	int n = s.size();
//	while (l < r)
//	{
//		if (s[l++] != s[r--])return false;
//	}
//	return true;
//}
#pragma endregion

const int MOD = 1e9 + 7;
#pragma region mihoyo树路径的异或和
//void dfs_naive(int u, int p, int current_xor, const vector<vector<pair<int, int>>>& adj, long long& total_sum) {
//	total_sum = (total_sum + current_xor) % MOD;
//	for (auto& edge : adj[u]) {
//		int v = edge.first;
//		int w = edge.second;
//		if (v != p) {
//			dfs_naive(v, u, current_xor ^ w, adj, total_sum);
//		}
//	}
//}
//
//
//void solve_naive() {
//	int n;
//	cin >> n;
//	vector<vector<pair<int, int>>> adj(n + 1); //带权图zui实用的数据结构，邻接表
//	for (int i = 0; i < n - 1; ++i) {
//		int u, v, w;
//		cin >> u >> v >> w;
//		adj[u].push_back({ v, w });
//		adj[v].push_back({ u, w });
//	}
//
//	long long total_sum = 0;
//	// 从每个节点出发跑一次 DFS
//	for (int i = 1; i <= n; ++i) {
//		dfs_naive(i, 0, 0, adj, total_sum);
//	}
//
//	// 因为 (u, v) 和 (v, u) 算作了两次，(u, u) 为 0 不影响，需要除以 2。
//	// 在模意义下除以 2，相当于乘 2 的乘法逆元 (MOD + 1) / 2
//	long long inv2 = (MOD + 1) / 2;
//	total_sum = (total_sum * inv2) % MOD;
//
//	cout << total_sum << "\n";
//}
//
//int main() {
//	ios_base::sync_with_stdio(false);
//	cin.tie(NULL);
//	int t;
//	cin >> t;
//	while (t--) {
//		solve_naive();
//	}return 0;
//}
#pragma endregion


void dfs(vector<vector<int>>& res, vector<int>& path, int& sum, int i, int& target)
{
	if (sum == target)
	{
		for (int i = 1; i < path.size(); i++)
		{
			if (path[i] - path[i - 1] != 1)return;
		}
		res.push_back(path);
		return;
	}
	for (int j = i; j <= target / 2 + 1; ++j)
	{
		path.push_back(j);
		sum += j;
		dfs(res, path, sum, j + 1, target);
		sum -= j;
		path.pop_back();
	}
}
vector<vector<int>> fileCombination(int target) {
	vector<vector<int>>res;
	vector<int>path;
	int sum = 0;
	dfs(res, path, sum, 1, target);
	return res;
}

//int maindsf() {
//	// 优化输入输出流速度
//	ios_base::sync_with_stdio(false);
//	cin.tie(NULL);
//	////cout << INF<<endl;
//	//stack<int> x;
//	///*PerfectTransfer();*/
//	//FuncPtrTest();
//
//	/*vector<vector<int>> res = fileCombination(18);
//	for (int i = 0; i < res.size(); ++i)
//	{
//		for (int j = 0; j < res[0].size(); ++j)cout << res[i][j];
//		cout << endl;
//	}*/
//
//	cout << "Test 3.14" << endl;
//	return 0;
//}

