#include <vector>
#include <deque>
#include <algorithm>
#include <iostream>
#include "Utils.hpp"
#include <unordered_set>
#include <map>
#include <set>
#include <string>
#include <exception>
#include <iomanip>
#include <unordered_map>
#include<memory>
#include <queue>
#include<climits>
using namespace std;


long long mod = 998244353;
int cnt = 0;
vector<int> path;
vector<int> nums;

#pragma region 单增三元组
//全排列 O(n3)
int FullArange(const vector<int>& nums)
{
	int n = nums.size();
	int cnt = 0;
	for (int i = 0; i < n; ++i)
	{
		for (int j = i + 1; j < n; ++j)
		{
			for (int k = j + 1; k < n; ++k)
			{
				if (nums[k] > nums[j] && nums[j] > nums[i])
				{
					(++cnt) % mod;
				}
			}
		}
	}
	return cnt;
}
/// <summary>
/// 深度遍历
/// </summary>
/// <param name="start"></param>
/// <param name="depth"></param>
void dfs(int start, int depth)
{
	if (depth == 3)
	{
		if (path[0] < path[1] && path[1] < path[2])
		{
			++cnt;
		}
		return;
	}
	for (int i = start; i < nums.size(); ++i)
	{
		if (!path.empty() && path.back() > nums[i]) continue;//剪枝DFS

		path.push_back(nums[i]);
		dfs(i + 1, depth + 1);
		path.pop_back();

	}
}

const int MOD = 998244353;

// 树状数组 (Fenwick Tree) 模板
struct FenwickTree {
	int n;
	vector<int> tree;

	FenwickTree(int n) : n(n), tree(n + 1, 0) {}

	// 获取最低位的 1
	inline int lowbit(int x) {
		return x & (-x);
	}

	// 单点增加：在位置 i 加上 delta
	void add(int i, int delta) {
		while (i <= n) {
			tree[i] += delta;
			i += lowbit(i);
		}
	}

	// 区间查询：求 [1, i] 的前缀和
	int query(int i) 
	{
		int sum = 0;
		while (i > 0) 
		{
			sum += tree[i];
			i -= lowbit(i);
		}
	}
};
//void testTree() {
//	// 优化输入输出流
//	ios_base::sync_with_stdio(false);
//	cin.tie(NULL);
//
//	int n;
//	if (!(cin >> n)) return 0;
//
//	vector<int> a(n);
//	vector<int> temp(n);
//	for (int i = 0; i < n; ++i) {
//		cin >> a[i];
//		temp[i] = a[i];
//	}
//
//	// 1. 离散化：去重并排序，以便将任意大小的值映射到 1~n
//	sort(temp.begin(), temp.end());
//	temp.erase(unique(temp.begin(), temp.end()), temp.end());
//
//	// rank_max 是离散化后的最大相对排名
//	int rank_max = temp.size();
//
//	// 将原数组替换为离散化后的排名 (1-based index)
//	for (int i = 0; i < n; ++i) {
//		a[i] = lower_bound(temp.begin(), temp.end(), a[i]) - temp.begin() + 1;
//	}
//
//	vector<int> L(n, 0);
//	vector<int> R(n, 0);
//
//	// 2. 求 L 数组 (左侧比 a[j] 小的个数)
//	FenwickTree bit_left(rank_max);
//	for (int j = 0; j < n; ++j) {
//		L[j] = bit_left.query(a[j] - 1);
//		bit_left.add(a[j], 1);
//	}
//
//	// 3. 求 R 数组 (右侧比 a[j] 大的个数)
//	FenwickTree bit_right(rank_max);
//	for (int j = n - 1; j >= 0; --j) {
//		// 总数减去 <= a[j] 的数，即为 > a[j] 的数
//		R[j] = bit_right.query(rank_max) - bit_right.query(a[j]);
//		bit_right.add(a[j], 1);
//	}
//
//	// 4. 统计答案并取模
//	long long ans = 0;
//	for (int j = 0; j < n; ++j) {
//		// L[j] 和 R[j] 相乘可能会超过 int 的范围，转换为 long long
//		long long current_triplets = (1LL * L[j] * R[j]) % MOD;
//		ans = (ans + current_triplets) % MOD;
//	}
//
//	cout << ans << "\n";
//
//	return 0;
//}
#pragma endregion

#pragma region 滑动窗口最小覆盖子串
string SlideWindow(string& s, string& t)
{
	if (t.empty())return "";
	int needCnt = t.size();
	int n = s.size();
	map<char, int>need;
	map<char, int>have;
	for (char c : t)++need[c];
	int left = 0, right = 0;
	int minLen = INT_MAX;
	int minStart = 0;
	while (right < n)
	{
		if (need[s[right]] > 0)
		{
			++have[s[right]];
			if (have[s[right]] <= need[s[right]])
				--needCnt;
		}
		++right;
		while (needCnt == 0)
		{
			if (right - left < minLen)
			{
				minStart = left;
				minLen = right - left;
			}
			char lval = s[left];
			if (need[lval] > 0)
			{
				if (have[lval] == need[lval])++needCnt;
				--have[lval];
			}
			++left;
		}
	}
	return minLen == INT_MAX ? "" : s.substr(minStart, minLen);

}
#pragma endregion

#pragma region 单调队列维护窗口最大值
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
	deque<int> dq;
	vector<int>res;
	int n = nums.size();
	for (int i = 0; i < n; ++i)
	{
		while (!dq.empty() && dq.front() <= i - k)dq.pop_front();
		while (!dq.empty() && nums[dq.back()] < nums[i])dq.pop_back();
		dq.push_back(i);
		if (i >= k - 1)
		{
			res.push_back(nums[dq.front()]);
		}
	}
	return res;
}
#pragma endregion

#pragma region 线性哈希,最长连续递增子序列
int LongestConsecutive(vector<int>& nums)
{
	unordered_set<int> _set(nums.begin(), nums.end());
	int maxLen = 0;
	int maxStart = 0;
	for (int num : nums)
	{
		if (_set.find(num - 1) == _set.end())
		{

			int len = 1;
			while (_set.find(++num) != _set.end())++len;
			/*maxLen = maxLen > len ? maxLen : len;*/
			if (maxLen < len)
			{
				maxStart = num;
				maxLen = len;
			}
		}
	}
	return maxLen;
}
int longestConsecutive2(vector<int>& nums) {
	unordered_set<int> uset;
	for (int num : nums) uset.insert(num);

	int res = 0;
	for (int num : uset)			//O(n²)
	{
		int curLen = 1;
		int curNum = num;
		while (uset.find(curNum + 1) != uset.end())
		{
			curLen++;
			curNum++;

		}
		cout << "curLen = " << curLen << endl;
		res = max(res, curLen);
	}
	return res;
}
#pragma endregion

#pragma region 前缀和+哈希，求最长和为0的子串
void PredixSum()
{
	string s;
	int curSum = 0, maxLen = 0;
	while (cin >> s)
	{
		if (s == "EOF")break;
		int n = s.size();
		vector<int> prefix(2 * n + 1, -2);
		prefix[n] = -1; // 前缀和为0的下标为-1

		for (int i = 0; i < n; ++i)
		{
			curSum += (s[i] == '1' ? 1 : -1);
			int preSum = curSum + n;
			if (prefix[preSum] != -2)
			{
				maxLen = max(maxLen, i - prefix[preSum]);
			}
			else
			{
				prefix[preSum] = i;
			}
		}
	}
	cout << maxLen << endl;

}
#pragma endregion
//int myAtoi(string str) {
//	int i = 0;
//	while (str[i] == ' ')
//	{
//		++i;
//	}
//	queue<int> a;
//	const long long MOD = (1LL << 31) + 1;   // 整数常量
//	const long long MOD1 = (1LL << 31);   // 整数常量
//	long res = 0;
//	for (int j = i; j < str.size(); ++j)
//	{
//		if (str[j] == '-')
//		{
//			++j;
//			while (j < str.size())
//			{
//				if (str[j] >= '0' && str[j] <= '9')
//				{
//					res = (res * 10 + (str[j] - '0'));
//					if (res > MOD) return -pow(2, 31);
//					++j;
//				}
//				else
//				{
//					return -res;
//					break;
//				}
//			}return -res;
//			break;
//		}
//		else if (str[j] > '9' && str[j] != '+') continue;
//		else
//		{
//			if (str[j] > '9' || str[j] < '0') ++j;
//			while (j < str.size())
//			{
//
//				if (str[j] >= '0' && str[j] <= '9')
//				{
//					res = (res * 10 + (str[j] - '0'));
//					if (res > MOD1) return pow(2, 31) - 1;
//					++j;
//				}
//				else
//				{
//
//					break;
//				}
//			}
//			return res;
//			break;
//		}
//	}
//	return res;
//}
#pragma region 拓扑排序
bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
	vector<int> indegree(numCourses);
	vector<vector<int>> graph(numCourses);
	for (auto it : prerequisites)
	{
		int a = it[0], b = it[1];
		graph[b].push_back(a);
		++indegree[a];
	}
	queue<int>q;
	for (int i = 0; i < numCourses; ++i)
	{
		if (indegree[i] == 0)q.push(i);
	}
	int cnt = 0;
	while (!q.empty())
	{
		int course = q.front();
		q.pop();
		++cnt;
		for (auto it : graph[course])
		{
			--indegree[it];
			if (indegree[it] == 0)q.push(it);
		}
	}
	return numCourses == cnt;
}
#pragma endregion

#pragma region 最长子串的长度
int lengthOfLongestSubstring(string s) {
	int n = s.size();
	map<char, int>need;
	int left = 0, right = 0;
	int maxLen = 0;
	while (right < n)
	{
		char c = s[right];
		if (need[c] == 1)
		{
			maxLen = right - left;
			while (s[left++] != c)
			{
			}
			right = left;

		}
		else need[c] = 1;
		++right;
	}
	return maxLen;
}
#pragma endregion

#pragma region 频率前K个数
struct MyCmp  //class
{
public:
	bool operator()(const pair<int, int>& a, const pair<int, int>& b) const
	{
		return a.second > b.second;
	}
};
static bool cmp1(pair<int, int>& a, pair<int, int>& b)
{
	return a.second > b.second;
};
vector<int> topKFrequent(vector<int>& nums, int k) {

	auto cmp = [](const pair<int, int>& a, const pair<int, int>& b) {
		return a.second > b.second; // 频率小的优先出队（小顶堆）
		};
	std::unordered_map<int, int>map;
	for (int num : nums)
	{
		map[num]++;
	}
	//MySTL::priority_queue<pair<int,int>, vector<pair<int, int>>,bool(*)(pair<int,int>&, pair<int,int>&)> q(cmp);
	//MySTL::priority_queue<pair<int,int>, vector<pair<int, int>>,bool(*)(pair<int,int>&, pair<int,int>&)> q(cmp1);
	//MySTL::priority_queue<pair<int,int>, vector<pair<int, int>>,decltype(&cmp1)> q(cmp1);

	priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> q(cmp);//显示传入lambda匿名类实例，C++20才支持无捕获的lambda的默认构造
	//MySTL::priority_queue<pair<int, int>, vector<pair<int, int>>, MyCmp> q;
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
#pragma endregion

#pragma region 查询十进制数1的个数
class SolutionTable {
private:
	vector<int> dp; // 存储前缀和

public:
	// 构造函数中进行打表，max_val 是题目可能给出的最大数值
	SolutionTable(int max_val)
	{
		dp.resize(max_val + 1, 0);
		int total_ones = 0;

		for (int i = 1; i <= max_val; ++i)
		{
			int curNum = i;
			int curCnt = 0;
			while (curNum)
			{
				if (curNum % 10 == 1)curCnt++;

				curNum /= 10;
			}
			total_ones += curCnt;
			dp[i] = total_ones;
		}
	}

	// O(1) 的查询操作
	int countDigitOne(int n)
	{
		if (n < 0) return 0;
		if (n >= dp.size()) return dp.back(); // 防越界保护
		return dp[n];
	}
};

int countDigitOne1(int n) {
	if (n <= 0) return 0;

	long long count = 0; // 记录 1 出现的总次数
	long long m = 1;     // 当前统计的位，1代表个位，10代表十位，100代表百位...

	// 当 m 小于等于 n 时继续统计
	while (m <= n)
	{
		int cur = (n / m) % 10;
		int high = n / m / 10;
		int low = n % m;
		if (cur == 0) count += high * m;
		else if (cur == 1)count += high * m + low + 1;
		else count += (high + 1) * m;

		m *= 10;
	}


	return count;
}
#pragma endregion

class MyException :public exception
{
public:
	const char* what()const override
	{
		return "error!!!!!";
	}
};

template<bool isUse>
int func(int a, int b)
{
	if constexpr(isUse) return a + b;
	else return a * b;
}
int gcd(int a, int b) {
	return b == 0 ? a : gcd(b, a % b);
}

// 判断一个字符是否为字母
bool isLetter(char ch) {
	return std::isalpha(static_cast<unsigned char>(ch));
}

// 将字符串转换为小写
std::string toLower(const std::string& s) {
	std::string result = s;
	for (char& ch : result) {
		ch = std::tolower(static_cast<unsigned char>(ch));
	}
	return result;
}

// 从文本中提取单词（连续字母序列）
std::vector<std::string> extractWords(const std::string& text) {
	std::vector<std::string> words;
	std::string word;
	for (char c : text)
	{
		if (isLetter(c))
		{
			word.push_back(c);
		}
		else
		{
			if (!word.empty())
			{
				words.push_back(toLower(word));
				word.clear();
			}
		}
	}
	// 处理最后一个单词
	if (!word.empty()) {
		words.push_back(toLower(word));
	}
	return words;
}

int fitst() {
	std::cout << "请输入一段文本（以 Ctrl+Z / Ctrl+D 结束输入）：" << std::endl;
	std::string line;
	std::string fullText;

	// 读取所有行，拼接成一个完整字符串
	while (std::getline(std::cin, line)) {
		fullText += line + " ";
	}

	// 提取所有单词
	std::vector<std::string> words = extractWords(fullText);

	// 统计频率
	std::map<std::string, int> freq;
	for (const auto& w : words) {
		freq[w]++;
	}

	// 将 map 中的内容转移到 vector 中以便按值排序
	vector<pair<string, int>> freqVec(freq.begin(), freq.end());

	// 按频率降序排序，频率相同则按单词字母升序
	std::sort(freqVec.begin(), freqVec.end(),
		[](const auto& a, const auto& b) {
			if (a.second != b.second)
				return a.second > b.second;   // 频率高的在前
			return a.first < b.first;         // 频率相同，单词字典序小的在前
		});

	// 输出结果
	std::cout << "\n单词频率统计（按频率从高到低）：" << std::endl;
	for (const auto& p : freqVec) {
		std::cout << p.first << " : " << p.second << std::endl;
	}

	return 0;
}
void sceond()
{
	PredixSum();
	/*int a, b = 0;
	char m[first], n[11];
	cout << first;*/
	//string h, l;
	///*cin.getline(m, 10);*/
	//cin >> l;
	//cout << "shuru" << endl;
	///*cin.getline(n, 10);*/
	//cin >> h;
	//cout << h << endl;
	//cout<< l << endl;
	//cout << static_cast<char>(97)<< endl;
	//cout << char(97)<< endl;
	//cout << static_cast<int>('A') << endl;







	/*unsigned a = UINT_MAX;
	int b = 045;
	int c = 0x45;
	cout << sizeof(unsigned long) << endl;
	cout << dec;
	cout << a << endl;
	cout << "12345L " << sizeof(12345L) << endl;
	cout << "12345U" << sizeof(12345U) << endl;
	cout << "12345UL" << sizeof(12345UL) << endl;
	cout << "12345LL" << sizeof(12345LL) << endl;
	char m = 'M';
	int mm = m;
	cout << ++m << "  if  " << ++mm << endl;
	cout << oct;
	cout << b << endl;

	cout << hex;
	cout << c << endl;*/


	/*int a = 2,  b = 3;
	cout << func<true>(a, b) << endl;
	cout << func<false>(a, b) << endl;*/

	/*int n;
	cin >> n;
	nums.resize(n);
	for (int i = 0; i < n; ++i)
		cin >> nums[i];
	dfs(0, 0);
	cout << cnt % mod << endl;

	string s = "ADOBECODEBANC";
	string t = "ABC";
	cout << SlideWindow(s, t) << endl;

	set<int, less<int>> set;
	multiset<int> mulset;
	vector<int> nums = { 100, 4, 200, 1, 3, 2, 101, 102, 105, 103, 104 };
	unordered_multiset<int> _set(nums.begin(), nums.end());
	cout << LongestConsecutive(nums);*/
}

#pragma region 通配符匹配
namespace 通配符匹配递归算法
{
	/// <summary>
	/// 递归 时复：O(2 ^(m+n)) 空复: O(m+n)
	/// </summary>
	/// <param name="s"></param>
	/// <param name="p"></param>
	/// <returns></returns>
	
	bool recursion(string& s, string& p, int i, int j, int n, int m) {
		// 跳过连续的非 '*' 字符，并匹配
		while (i < m && j < n && p[i] != '*') {
			if (p[i] == '?' || p[i] == s[j]) {
				i++;
				j++;
			}
			else {
				return false;
			}
		}

		// 模式已耗尽
		if (i == m) {
			return j == n;
		}

		// 字符串已耗尽，检查剩余模式是否全为 '*'
		if (j == n) {
			for (int k = i; k < m; k++) {
				if (p[k] != '*') return false;
			}
			return true;
		}

		// 当前字符为 '*'
		// 选择1：跳过这个 '*'（匹配0个字符）
		// 选择2：用 '*' 匹配一个字符（保留 '*' 继续匹配）
		return recursion(s, p, i + 1, j, n, m) || recursion(s, p, i, j + 1, n, m);
	}

	bool isMatch(string s, string p) {
		int n = s.size(), m = p.size();
		return recursion(s, p, 0, 0, n, m);
	}

	bool recursion(string& s, string& p, int i, int j, int n, int m, vector<vector<int>>& memo) {
		// 1. 查表：如果当前状态之前已经计算过，直接返回结果，避免重复递归
		if (memo[i][j] != -1) {
			return memo[i][j] == 1;
		}

		// 记录进入函数时的原始 i 和 j，因为后面的 while 循环会改变它们的值
		// 我们需要把结果存入进入时的状态坐标中
		int original_i = i;
		int original_j = j;

		// 2. 原逻辑：跳过连续的非 '*' 字符，并匹配
		while (i < m && j < n && p[i] != '*') {
			if (p[i] == '?' || p[i] == s[j]) {
				i++;
				j++;
			}
			else {
				// 不匹配，写入备忘录并返回 false
				return memo[original_i][original_j] = 0;
			}
		}

		// 3. 模式已耗尽
		if (i == m) {
			return memo[original_i][original_j] = (j == n ? 1 : 0);
		}

		// 4. 字符串已耗尽，检查剩余模式是否全为 '*'
		if (j == n) {
			for (int k = i; k < m; k++) {
				if (p[k] != '*') {
					return memo[original_i][original_j] = 0;
				}
			}
			return memo[original_i][original_j] = 1;
		}

		// 5. 当前字符为 '*'，进行分支递归
		// 选择1：跳过这个 '*'（匹配0个字符） -> recursion(..., i + 1, j, ...)
		// 选择2：用 '*' 匹配一个字符（保留 '*' 继续匹配） -> recursion(..., i, j + 1, ...)
		bool res = recursion(s, p, i + 1, j, n, m, memo) ||
			recursion(s, p, i, j + 1, n, m, memo);

		// 6. 将最终计算出来的结果写入备忘录
		return memo[original_i][original_j] = (res ? 1 : 0);
	}
	bool isMatchR(string s, string p) {
		int n = s.size(), m = p.size();

		// 建立备忘录：-1 代表未计算，1 代表 true，0 代表 false。
		// 大小设置为 (m + 1) x (n + 1)，因为 i 和 j 的最大值可以达到 m 和 n。
		vector<vector<int>> memo(m + 1, vector<int>(n + 1, -1));

		return recursion(s, p, 0, 0, n, m, memo);
	}
}

namespace 通配符匹配DP算法
{
	/// <summary>
	/// DP 时复：O(m * n) 空复: O(mn)
	/// </summary>
	/// <param name="s"></param>
	/// <param name="p"></param>
	/// <returns></returns>
	bool isMatch(string s, string p) {
		int n = s.size();
		int m = p.size();

		// dp[i][j] 表示 s 的前 i 个字符与 p 的前 j 个字符是否匹配
		vector<vector<bool>> dp(n + 1, vector<bool>(m + 1, false));

		// 初始状态：空字符串匹配空模式
		dp[0][0] = true;

		// 初始化：当 s 为空时，只有 p 连续为 '*' 才能匹配
		for (int j = 1; j <= m; ++j) {
			if (p[j - 1] == '*') {
				dp[0][j] = dp[0][j - 1];
			}
		}

		for (int i = 1; i <= n; ++i) {
			for (int j = 1; j <= m; ++j) {
				if (p[j - 1] == '?' || s[i - 1] == p[j - 1]) {
					// 当前字符匹配，取决于之前的状态
					dp[i][j] = dp[i - 1][j - 1];
				}
				else if (p[j - 1] == '*') {
					// '*' 匹配 0 个 (dp[i][j-1]) 或 匹配 1 个及以上 (dp[i-1][j])
					dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
				}
			}
		}

		return dp[n][m];
	}
};
namespace 通配符匹配双指针贪心算法
{
	/// <summary>
	/// DP 时复：O(m + n) 空复: O(1)
	/// </summary>
	/// <param name="s"></param>
	/// <param name="p"></param>
	/// <returns></returns>
	bool isMatch(string s, string p) {
		int i = 0, j = 0;
		int sStar = -1, pStar = -1;
		int n = s.size(), m = p.size();

		while (i < n) {
			if (j < m && (p[j] == '?' || p[j] == s[i])) {
				i++; j++;
			}
			else if (j < m && p[j] == '*') {
				pStar = j;     // 记录星号位置
				sStar = i;     // 记录星号开始匹配 s 的位置
				j++;           // 尝试星号匹配 0 个字符
			}
			else if (pStar != -1) {
				// 当前不匹配，但之前有星号，回溯
				j = pStar + 1;
				sStar++;       // 让星号多匹配一个 s 中的字符
				i = sStar;
			}
			else {
				return false;
			}
		}

		// 检查 p 剩余部分是否全是 '*'
		while (j < m && p[j] == '*') j++;
		return j == m;
	}
}

#pragma endregion

#pragma region 中缀表达式字符串直接构建表达式树
#include <stack>

#include <cctype>

//// 表达式树节点
//struct TreeNode {
//	string val;          // 存储操作数或运算符（用字符串方便区分）
//	TreeNode* left;
//	TreeNode* right;
//	TreeNode(string x) : val(x), left(nullptr), right(nullptr) {}
//};
//
//class ExpressionTreeBuilder {
//private:
//	// 运算符优先级
//	unordered_map<char, int> precedence = {
//		{'+', 1}, {'-', 1},
//		{'*', 2}, {'/', 2}
//	};
//
//	// 判断是否为运算符
//	bool isOperator(char c) {
//		return c == '+' || c == '-' || c == '*' || c == '/';
//	}
//
//	// 中缀表达式 → 后缀表达式（token 序列）
//	vector<string> infixToPostfix(const string& s) {
//		vector<string> postfix;
//		stack<char> ops;
//
//		for (int i = 0; i < s.size(); ++i) {
//			char c = s[i];
//			if (c == ' ') continue;                      // 忽略空格
//
//			if (isdigit(c)) {                            // 操作数：解析完整数字
//				string num;
//				while (i < s.size() && isdigit(s[i])) {
//					num += s[i++];
//				}
//				--i;                                     // 回退一位
//				postfix.push_back(num);
//			}
//			else if (c == '(') {
//				ops.push(c);
//			}
//			else if (c == ')') {
//				while (!ops.empty() && ops.top() != '(') {
//					postfix.push_back(string(1, ops.top()));
//					ops.pop();
//				}
//				ops.pop();                               // 弹出 '('
//			}
//			else if (isOperator(c)) {
//				// 处理一元负号（如表达式开头的 -5 或 (-5）
//				if (c == '-' && (i == 0 || s[i - 1] == '(')) {
//					postfix.push_back("0");              // 补 0 变为 0 - 5
//				}
//				// 弹出栈顶优先级 >= 当前运算符的运算符
//				while (!ops.empty() && ops.top() != '(' &&
//					precedence[ops.top()] >= precedence[c]) {
//					postfix.push_back(string(1, ops.top()));
//					ops.pop();
//				}
//				ops.push(c);
//			}
//		}
//		// 弹出剩余运算符
//		while (!ops.empty()) {
//			postfix.push_back(string(1, ops.top()));
//			ops.pop();
//		}
//		return postfix;
//	}
//
//	// 后缀表达式 → 表达式树
//	TreeNode* buildTreeFromPostfix(const vector<string>& postfix) {
//		stack<TreeNode*> st;
//		for (const string& token : postfix) {
//			TreeNode* node = new TreeNode(token);
//			if (token.size() == 1 && isOperator(token[0])) {
//				// 运算符：弹出两个操作数作为子树
//				TreeNode* right = st.top(); st.pop();
//				TreeNode* left = st.top(); st.pop();
//				node->left = left;
//				node->right = right;
//			}
//			st.push(node);
//		}
//		return st.empty() ? nullptr : st.top();
//	}
//
//public:
//	TreeNode* build(const string& infix) {
//		vector<string> postfix = infixToPostfix(infix);
//		return buildTreeFromPostfix(postfix);
//	}
//};
//
//// ---------- 辅助函数：后序遍历（验证树结构）----------
//void postorder(TreeNode* root) {
//	if (!root) return;
//	postorder(root->left);
//	postorder(root->right);
//	cout << root->val << " ";
//}
//
//// ---------- 测试 ----------
//int main51246() {
//	ExpressionTreeBuilder builder;
//	string expr = "3+4*5";
//	TreeNode* root = builder.build(expr);
//
//	cout << "后缀表达式（后序遍历）: ";
//	postorder(root);   // 应输出: 3 4 5 * + 
//	cout << endl;
//
//	// 也可再加一个中序遍历（加括号）来还原中缀
//	return 0;
//}
#pragma endregion

#pragma region 双栈法基本计算器 III
//class Solution {
//public:
//	stack<long> nums; stack<char> ops;
//	unordered_map<char, int> prio = { {'+',1}, {'-',1}, {'*',2}, {'/',2} };
//
//	// 辅助函数：从栈顶取数并执行一次运算
//	void calc() {
//		long b = nums.top(); nums.pop();
//		long a = nums.top(); nums.pop();
//		char op = ops.top(); ops.pop();
//		switch (op) {
//		case '+': nums.push(a + b); break;
//		case '-': nums.push(a - b); break;
//		case '*': nums.push(a * b); break;
//		case '/': nums.push(a / b); break;
//		}
//	}
//
//	int calculate(string s) {
//		nums.push(0); // 哨兵，用于处理表达式以负号开头的情况
//		for (int i = 0; i < s.size(); ++i) {
//			char c = s[i];
//			if (c == ' ') continue;
//			if (isdigit(c)) {
//				long num = 0;
//				while (i < s.size() && isdigit(s[i]))
//					num = num * 10 + (s[i++] - '0');
//				nums.push(num);
//				--i; // 回退，因为外层的for循环会自增
//			}
//			else if (c == '(') {
//				ops.push(c);
//				// 处理左括号后紧跟负号的情况: (-2+3) -> (0-2+3)
//				if (i + 1 < s.size() && s[i + 1] == '-') nums.push(0);
//			}
//			else if (c == ')') {
//				while (!ops.empty() && ops.top() != '(') calc();
//				if (!ops.empty()) ops.pop(); // 弹出 '('
//			}
//			else {
//				// 处理负号作为一元运算符: -1+2 -> 0-1+2
//				if (c == '-' && (i == 0 || s[i - 1] == '(')) nums.push(0);
//				while (!ops.empty() && ops.top() != '(' && prio[ops.top()] >= prio[c])
//					calc();
//				ops.push(c);
//			}
//		}
//		while (!ops.empty()) calc();
//		return (int)nums.top();
//	}
//};
#pragma endregion


//逆波兰表达式
//逆波兰表达式求值(RPN) - 算法可视化
//使用栈来计算后缀表达式的值。遇到数字入栈，遇到运算符出栈计算并入栈。
int mergeSort(int l, int r, vector<int>& record, vector<int>& tmp) {
		// 终止条件
		if (l >= r) return 0;
		// 递归划分
		int m = (l + r) / 2;
		int res = mergeSort(l, m, record, tmp) + mergeSort(m + 1, r, record, tmp);
		// 合并阶段
		int i = l, j = m + 1;
		for (int k = l; k <= r; k++)
			tmp[k] = record[k];
		for (int k = l; k <= r; k++) {
			if (i == m + 1)
				record[k] = tmp[j++];
			else if (j == r + 1 || tmp[i] <= tmp[j])
				record[k] = tmp[i++];
			else {
				record[k] = tmp[j++];
				res += m - i + 1; // 统计逆序对
			}
		}
		return res;
	}


//网络迪杰斯特拉
int networkDelayTime(vector<vector<int>>& times, int n, int k) 
{
	//邻接表
	unordered_map<int, vector<pair<int, int>>> graph;
	for (auto& edge : times)
	{
		int u = edge[0], v = edge[1], w = edge[2];
		graph[u].push_back({ v, w });
	}
	//初始化距离向量数组，默认距离不可达无穷大
	vector<int>dis(n + 1, INT_MAX);
	// 优先队列，小根堆，优先边权值最短
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
	// 插入起始节点k
	pq.push({ 0, k });
	dis[k] = 0;

	while (!pq.empty())
	{
		auto [d, u] = pq.top(); pq.pop();
		// 无效旧数据，跳过
		if (d > dis[u]) continue;
		for (auto& [v, w] : graph[u])
		{
			//发现更短路径,将距离和顶点v插入队列,
			//再从v洪泛松弛其邻居
			if (dis[u] + w < dis[v])
			{
				dis[v] = dis[u] + w;
				pq.push({ dis[v], v });
			}
		}
	}
	int res = 0;
	//遍历所有节点，取最大距离即为答案
	//最大距离即为，单源最短路径，从当前节点出发，到各节点的最短距离
	for (int i = 1; i <= n; ++i)
	{
		if (dis[i] == INT_MAX) return -1;
		res = max(res, dis[i]);
	}
	return res;
}
