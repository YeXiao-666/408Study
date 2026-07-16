#include<iostream>
#include<vector>
#include<algorithm>
#include "Utils.hpp"
using namespace std;
#include <random>
#include <queue>
#include <set>
#include <map>
#include <list>
#include <string>

// 定义最小的 Run 长度，通常取 32 或 64
const int RUN = 32;

// ==========================================
// 1. 插入排序模块
// 用于对较小的数组块（Run）进行高效排序
// ==========================================
void insertionSort(vector<int>& arr, int left, int right) {
	for (int i = left + 1; i <= right; i++) {
		int temp = arr[i];
		int j = i - 1;
		// 寻找合适的插入位置，同时将较大的元素后移
		while (j >= left && arr[j] > temp) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = temp;
	}
}

// ==========================================
// 2. 归并排序模块
// 用于将两个已经有序的 Run 合并成一个更大的有序数组
// ==========================================
void merge(vector<int>& arr, int l, int m, int r) {
	// 计算两个子数组的长度
	int len1 = m - l + 1;
	int len2 = r - m;

	// 创建临时数组并拷贝数据
	vector<int> left(len1), right(len2);
	for (int i = 0; i < len1; i++) left[i] = arr[l + i];
	for (int i = 0; i < len2; i++) right[i] = arr[m + 1 + i];

	int i = 0, j = 0, k = l;

	// 比较并合并
	while (i < len1 && j < len2) {
		// 注意这里的 <= 保证了排序的稳定性（Stability）
		if (left[i] <= right[j]) {
			arr[k++] = left[i++];
		}
		else {
			arr[k++] = right[j++];
		}
	}

	// 将左边或右边剩余的元素拷贝回去
	while (i < len1) arr[k++] = left[i++];
	while (j < len2) arr[k++] = right[j++];
}

// ==========================================
// 3. TimSort 主控函数
// ==========================================
void timSort(vector<int>& arr) {
	int n = arr.size();
	if (n <= 1) return;

	// 步骤 A: 将数组划分为大小为 RUN 的块，并分别使用插入排序
	for (int i = 0; i < n; i += RUN) {
		// 注意不要越界，最后一个块可能小于 RUN
		int right = min((i + RUN - 1), (n - 1));
		insertionSort(arr, i, right);
	}

	// 步骤 B: 归并这些已经有序的块
	// size 初始为 RUN，每次翻倍: 32, 64, 128...
	for (int size = RUN; size < n; size = 2 * size) {
		// 每次将相邻的两个大小为 size 的块合并
		for (int left = 0; left < n; left += 2 * size) {

			// 找到中间点和右边界
			int mid = left + size - 1;
			int right = min((left + 2 * size - 1), (n - 1));

			// 如果 mid 已经越界，说明这部分不需要合并了
			if (mid < right) {
				merge(arr, left, mid, right);
			}
		}
	}
}
void BubbleSort(vector<int>& array)
{
	int n = array.size();
	int flag = 0;
	for (int i = 0; i < n; ++i)
	{

		for (int j = 1; j < n - i; ++j)
		{
			if (array[j] < array[j - 1])
			{
				flag = 1;
				swap(array[j], array[j - 1]);
			}
		}
		if (!flag) break;
	}
}

int mypartition(vector<int>& array, int l, int r)
{
	// 初始化随机数生成器（通常在程序开始时只做一次）
	std::random_device rd;                       // 用于获取真随机种子
	std::mt19937 gen(rd());                       // 选用 Mersenne Twister 引擎
	std::uniform_int_distribution<int> dist(l, r); // 定义分布范围 [l, r]

	// 生成随机索引
	int pivot_index = dist(gen);

	// 交换元素
	std::swap(array[l], array[pivot_index]);
	int i = l, j = r;
	while (i < j)
	{
		while (i < j && array[j] >= array[l]) j -= 1;
		while (i < j && array[i] <= array[l]) i += 1;
		swap(array[i], array[j]);
	}
	swap(array[i], array[l]);
	return i;
}
void QuickSort(vector<int>& array, int l, int r)
{
	if (l >= r) return;
	int i = mypartition(array, l, r);
	QuickSort(array, l, i);
	QuickSort(array, i + 1, r);
}

void MergeSort(vector<int>& array, int l, int r)
{
	if (l >= r) return;
	int m = (l + r) >> 1;
	MergeSort(array, l, m);
	MergeSort(array, m + 1, r);
	vector<int> tmp(array.begin() + l, array.begin() + r + 1);
	int i = 0, j = m - l + 1;
	for (int k = l; k <= r; ++k)
	{
		if (i == m - l + 1)
		{
			array[k] = tmp[j++];
		}
		else if (j == r - l + 1 || tmp[j] > tmp[i])
		{
			array[k] = tmp[i++];
		}
		else
		{
			array[k] = tmp[j++];
		}
	}

}

int BinSearch(const vector<int>& a, int target) {
	int i = 0, j = a.size() - 1;
	while (i <= j) {
		int m = i + (j - i) / 2;  // 防止溢出
		if (a[m] > target)
			j = m - 1;
		else if (a[m] < target)
			i = m + 1;
		else
			return m;
	}
	return -1;
}
void insertsort(vector<int>& nums) {
	for (int i = 1; i < nums.size(); ++i) {
		int key = nums[i];
		int j = i - 1;
		while (j >= 0 && nums[j] > key) {
			nums[j + 1] = nums[j];
			--j;
		}
		nums[j + 1] = key;
	}
}

void BuildHeap(vector<int>& a, int n, int root)
{
	int maxIndex = root;
	int left = 2 * root + 1, right = 2 * root + 2;
	if (left < n && a[left] > a[maxIndex]) maxIndex = left;
	if (right < n && a[right] > a[maxIndex]) maxIndex = right;
	if (maxIndex != root)
	{
		swap(a[root], a[maxIndex]);
		BuildHeap(a, n, maxIndex);
	}
}
void HeapSort(vector<int>& a)
{

	int n = a.size();
	for (int i = (n >> 1) - 1; i >= 0; --i)
	{
		BuildHeap(a, n, i);
	}
	for (int i = n - 1; i >= 0; --i)
	{
		swap(a[i], a[0]);
		BuildHeap(a, i, 0);
	}
}
void CountSort(vector<int>& a, int exp) {
	int n = a.size();
	vector<int> out(n);
	int cnt[10] = { 0 };

	for (int i = 0; i < n; ++i)
		cnt[(a[i] / exp) % 10]++;

	for (int i = 1; i < 10; ++i)
		cnt[i] += cnt[i - 1];

	for (int i = n - 1; i >= 0; --i) {
		int d = (a[i] / exp) % 10;
		out[--cnt[d]] = a[i];   // 更简洁的写法
	}

	a = move(out);  // 直接转移所有权，避免复制
}

void radixSort(vector<int>& a) {
	if (a.empty()) return;
	int maxVal = *max_element(a.begin(), a.end());
	for (int exp = 1; maxVal / exp > 0; exp *= 10) {
		CountSort(a, exp);
	}
}
int findKthLargest(vector<int>& nums, int k) {
	priority_queue<int, vector<int>, greater<int>> pq;
	std::vector<int>::iterator it = nums.begin();
	while (it != nums.end())
	{
		pq.push(*it);
		if (pq.size() == k)
		{
			pq.pop();

		}
		++it;
	}
	return pq.top();
}
struct TreeNode {
	int val;
	TreeNode* left;
	TreeNode* right;
	TreeNode() : val(0), left(nullptr), right(nullptr) {}
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
	TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}

};
struct Node
{
	int x, y, dis;
	int CalDis(const Node& node)
	{
		dis = abs(node.x - this->x) + abs(node.y - this->x);
		return dis;
	}
	bool operator<(const Node& node)const
	{
		return this->dis < node.dis;
	}
};

int find_max_recursive(vector<int>& nums)
{
	if (nums.size() == 1)return nums[0];

	int first = nums[0];
	vector<int> temp(nums.begin() + 1, nums.end());
	int left_max = find_max_recursive(temp);
	return first > left_max ? first : left_max;
}





///// <summary>
///// 将当前自己所在子树每个子树都
///// </summary>
///// <param name="a"></param>
///// <param name="n">对从 root 开始的 n个元素调整堆</param>
///// <param name="root"> 从root 节点 开始调整堆 </param>
//void AdjustDown(vector<int>& a, int n, int root)
//{
//	while (true)
//	{
//		int maxindex = root;
//		int l = root * 2 + 1, r = root * 2 + 2;
//		if (l < n && a[l] > a[maxindex]) maxindex = l;
//		if (r < n && a[r] > a[maxindex]) maxindex = r;
//
//		if (root == maxindex) break;
//
//		swap(a[root], a[maxindex]);
//		root = maxindex;
//	}
//}
///// <summary>
///// 建堆，从最后一个非叶子节点开始(a.size() >> 1) - 1，向上层遍历，将最大（小）值泵至堆顶
///// </summary>
///// <param name="a"></param>
//
//void Heapify(vector<int>& a)
//{
//	for (int i = (a.size() >> 1) - 1; i >= 0; --i)
//	{
//		AdjustDown(a, a.size(), i);
//	}
//}
//// 堆排序主函数
//void HeapSort(vector<int>& a) {
//	if (a.size() <= 1) return;
//
//	Heapify(a);  // 建大根堆
//
//	int n = (int)a.size();
//
//	//每次将最大值放到数据末尾，
//	//最后形成升序排列 12345
//	for (int i = n - 1; i > 0; --i) {
//		swap(a[0], a[i]);      // 将堆顶（最大值）移到末尾
//		AdjustDown(a, i, 0);     // 对前 i 个元素重新调整堆
//	}
//}