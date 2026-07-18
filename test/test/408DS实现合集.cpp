#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <limits>
#include <algorithm>
#include <thread>   // 新增：用于多线程延时
#include <chrono>   // 新增：用于时间控制
#include <cstring>  // 新增：用于 memset
using namespace std;
namespace My408
{
#pragma region 排序算法


    //堆排序 ： 最好最坏均为时间：O(nlogn), 空间:O(1) 不稳定
#pragma region 堆排序
    
    void AdjustDown(int a[], int n, int root)
    {
        while (1)
        {
            int minIndex = root;
            int l = root * 2 + 1, r = root * 2 + 2;
            if (l < n && a[l] < a[minIndex]) minIndex = l;
            if (r < n && a[r] < a[minIndex]) minIndex = r;

            if (root == minIndex) break;
            swap(a[root], a[minIndex]);
            root = minIndex;
        }
    }
    void Heapify(int a[], int n)
    {
        for (int i = (n >> 1) - 1; i >= 0; --i)
        {
            AdjustDown(a, n, i); //从0开始下沉，将大值下沉，小值浮上
        }
    }

    void HeapSort(int a[], int n)
    {
        if (n <= 1) return;

        //建堆，最小值堆顶
        Heapify(a, n);
        //最后一个元素已经排好序
        for (int i = n - 1; i > 0; --i)
        {
            swap(a[0], a[i]);
            AdjustDown(a, i, 0);
        }

    }
    void AdjustDownBig(vector<int>& a, int n, int root)
    {
        while (true)
        {
            int minindex = root;
            int l = root * 2 + 1, r = root * 2 + 2;
            if (l < n && a[l] < a[minindex]) minindex = l;
            if (r < n && a[r] < a[minindex]) minindex = r;

            if (root == minindex) break;

            swap(a[root], a[minindex]);
            root = minindex;
        }
    }
    /// <summary>
    /// 建堆，从最后一个非叶子节点开始(a.size() >> 1) - 1，向上层遍历，将最大（小）值泵至堆顶
    /// </summary>
    /// <param name="a"></param>

    void HeapifyBig(vector<int>& a)
    {
        for (int i = (a.size() >> 1) - 1; i >= 0; --i)
        {
            AdjustDownBig(a, a.size(), i);
        }
    }

    //方案一：全量排序后取前 10 个（最简单）
    //直接调用你写好的 HeapSort 对整个数组排序，然后取最后 10 个（升序排序取末尾）或前 10 个（降序）。
    //缺点：时间复杂度 O(n log n)，如果数组很大（如 1 亿个数），排序非常耗时，且浪费了大量计算在不需要排序的元素上。

    vector<int> getTop10ByFullSort(vector<int>& a)
    {
        //HeapSort(a);  // 升序排列
        int n = a.size();
        vector<int> res;
        // 取最大的 10 个（排在最后面）
        for (int i = n - 1; i >= max(0, n - 10); --i)
        {
            res.push_back(a[i]);
        }
        return res;
    }
    //方案二：利用大根堆弹出 10 次（基于你现有的堆结构）
// 取前 K 个最大的数（K=10）
    //时间复杂度：建堆 O(n) + 弹出 K 次 O(K log n)。当 K=10 且 n 极大时，远快于全排序。

    //优点：不需要额外内存，原地操作。
    vector<int> getTopKByPop(vector<int>& a, int k = 10)
    {
        //if (a.empty() || k <= 0) return {};

        //Heapify(a);  // 先建堆，O(n)
        //vector<int> result;

        //int n = a.size();
        //for (int i = 0; i < k && i < n; ++i) {
        //    result.push_back(a[0]);      // 堆顶是当前最大值
        //    swap(a[0], a[n - 1 - i]);    // 将堆顶移到数组末尾（逻辑上删除）
        //    AdjustDown(a, n - 1 - i, 0);   // 对缩小后的堆进行调整
        //}
        vector<int> result;
        return result;
    }
    //！！！！最优解！！！
    //方案三：维护一个大小为 K 的小根堆（处理海量数据最优）
    //如果你的数据量极大（比如 100 亿个数，无法全部加载到内存），或者数据是流式动态生成的，推荐使用大小为 10 的小根堆。
    vector<int> getTopK(vector<int>& a, int k = 10)
    {
        //使用数组时
        // int heap[10]
        // ，可以在for循环遍历到i==10以后开始处理
        vector<int>minHeap;
        minHeap.reserve(10);

        for (int data : a) {
            if (minHeap.size() < k) {
                minHeap.push_back(data);
                if (minHeap.size() == k) {
                    HeapifyBig(minHeap);   // 当堆满时立即建堆
                }
            }
            else if (data > minHeap[0]) {
                minHeap[0] = data;
                AdjustDownBig(minHeap, k, 0);
            }
        }
        //最终堆里的10个元素即为最大的10个元素
        sort(minHeap.begin(), minHeap.end(), greater<int>());
        return minHeap;
    }


    void AdjustDown(vector<int>& a, int n, int root)
    {
        while (1)
        {
            int minIndex = root;

            int l = 2 * root + 1, r = 2 * root + 2;

            if (l < n && a[l] < a[minIndex]) minIndex = l;
            if (r < n && a[r] < a[minIndex]) minIndex = r;

            if (minIndex == root) break;

            swap(a[root], a[minIndex]);
            root = minIndex;
        }
    }

    vector<int> GetTopK(int a[], int n, int k)
    {
        vector<int> res;
        res.reserve(k);
        for (int i = 0; i < n; ++i)
        {
            if (i < k)
            {
                res.push_back(a[i]);
                /////////
                if (i == k - 1)
                {
                    for (int j = (k >> 1) - 1; j >= 0; j--)
                    {
                        AdjustDown(res, k, j);
                    }
                }
            }
            else
            {
                ////////!!!!
                if (a[i] > res[0])
                {
                    res[0] = a[i];
                    AdjustDown(res, k, 0);
                }

            }
        }
        return res;
    }

    void GetTopK2(int a[], int n, int k, int res[])
    {
        if (k <= 0 || n <= 0)return;

        for (int i = 0; i < k; ++i)res[i] = a[i];

        if (n <= k) return;
        Heapify(res, k);

        for (int i = k; i < n; ++i)
        {
            if (a[i] > res[0])
            {
                res[0] = a[i];
                AdjustDown(res, k, 0);
            }
        }

    }

    void DeleteHeap(int a[], int& n, int k)
    {
        if (k < 0 || k >= n)return;

        // 用最后一个元素覆盖被删元素
        a[k] = a[n - 1];
        --n;   // 堆大小减 1
        // 如果删除的就是最后一个，不需要调整
        if (k == n) return;

        // 从 k 开始，先尝试向下调整
        int parent = (k - 1) / 2;
        // 小顶堆：如果当前节点比父节点小，则向上调整（上滤）
        if (k > 0 && a[k] < a[parent]) {
            while (k > 0 && a[k] < a[(k - 1) / 2]) {
                swap(a[k], a[(k - 1) / 2]);
                k = (k - 1) / 2;
            }
        }
        else {
            // 否则向下调整
            AdjustDown(a, n, k);
        }
    }
#pragma endregion
    //归并排序 ：最好最坏均为 时间：O(nlogn), 暂存数组空间:O(n) 稳定
#pragma region 归并排序
    void MergeSort(int a[], int n, int l, int r)
    {
        if (l >= r)return; // 划分至最小的子数组，只有一个元素

        int m = (l + r) / 2;
        //分治，二路归并，划分左右子数组
        //划分右数组
        MergeSort(a, n, l, m);
        MergeSort(a, n, m + 1, r);
        //合并

        //暂存左右子数组待合并元素， 通过比较修改主数组的元素，从而排序
        int* tmp = new int[r - l + 1];

        for (int i = l; i <= r; ++i)tmp[i - l] = a[i];
        // i为tmp左端点 即 左子数组的第一个元素, j 为右子树组的第一个元素
        int i = 0, j = m - l + 1;
        // 遍历主数组的元素，通过比较暂存数组的值，进行排序
        for (int k = l; k <= r; ++k)
        {
            //左子数组排序完毕，右子数组未完成
            if (i == m - l + 1) a[k] = tmp[j++];
            //右子数组排序完毕，左子数组未完成，或者 相同元素 i下标小放前面，稳定！！大于等于
            else if (j == r - l + 1 || tmp[j] >= tmp[i]) a[k] = tmp[i++];
            //左右数组均为排序完毕
            else a[k] = tmp[j++];
        }
        delete[] tmp;

    }
#pragma endregion

   //!!!不考察代码!!! O(n²)
    /// <summary>
    /// 插入排序 
    /// </summary>
    /// <param name="a"></param>
    void InsertSort(vector<int>& a)
    {
        for (int i = 1; i < a.size(); ++i)
        {
            int key = a[i];
            int j = i - 1;
            while (j >= 0 && a[j] > key)   // 1 3 4 5  --- 2
            {
                a[j + 1] = a[j];            
                --j;                        // 1    3 4 5
            }   
                                
            a[j+1] = key;                   // 1 2 3 4 5
        }
    }

    /// <summary>
    ///  折半插入排序
    /// </summary>
    /// <param name="a"></param>
    void InsertSort2(vector<int>& a)
    {
        // O n * n
        for (int i = 1; i < a.size(); ++i) // O n
        {
            int key = a[i];
            //闭区间
            int l = 0, r = i - 1;
            while (l <= r)   //闭区间 小于等于
            {
                int m = (r + l) / 2;
                //!!!!如果改为 大于等于会破坏稳定性!!!!!
                //! //!!!!如果改为 大于等于会破坏稳定性!!!!!
                //! //!!!!如果改为 大于等于会破坏稳定性!!!!!
                if (a[m] > key)r = m - 1;   //m-1

                else l = m + 1;    // m + 1
            }

            // 大头 O n
            //a[l] 第一个大于key的数,需要将其向后移动, 所以要注意大于等于!!!!!!
            for (int j = i - 1; j >= l; --j)
            {
                if (a[j] == key) cout << "不稳定" << endl;
                a[j + 1] = a[j];
                
            }
            //l为插入的位置,即第一个大于key的位置
            a[l] = key;                   // 1  2 2  5
        }
    }

    //快速排序 ： 最好平均时间：O(nlogn) 最坏为有序时，O(n²), 空间:O(logn 到 n 之间) 不稳定
#pragma region 快速排序
    int patition(vector<int>& a, int l, int r)
    {
        // l 为基准元素
        int i = l, j = r;

        while (i < j)
        {
            //注意要大于等于，小于等于，不稳定！！否则遇到与基准元素相同的元素时会死循环，ij不动
            //如 1 3 4 5 6 1 2 3 4 2 1
            while (i < j && a[j] >= a[l]) --j;
            while (i < j && a[i] <= a[l]) ++i;
            swap(a[i], a[j]);
        }
        swap(a[i], a[l]);
        return i;
    }
    void QuickSort(vector<int>& a, int l, int r)
    {
        if (l >= r) return;

        int i = patition(a, l, r);

        QuickSort(a, l, i - 1);
        QuickSort(a, i+1, r);
    }
    
#pragma endregion
    //希尔排序 ：最好均为 时间：O(n1.3方),最坏为有序时，O(n²)， 空间:O(1)不稳定
#pragma region ShellSort


    /*int dlta[] = { 5, 3, 1 };
    ShellSort(a, dlta, 3);*/
    //若指明步长数组
    // // dlta 为增量序列数组，t 为增量个数
    void ShellSort2(vector<int>& a, int dlta[], int t) 
    {
        for (int k = 0; k < t; ++k) 
        {          // 依次取每个增量
            int gap = dlta[k];
            for (int i = gap; i < a.size(); ++i) 
            {
                int temp = a[i];
                int j;
                for (j = i - gap; j >= 0 && a[j] > temp; j -= gap)
                    a[j + gap] = a[j];
                a[j + gap] = temp;
            }
        }
    }
    // step = (a.size() + 1) / 2
    int j;
    void ShellSort(vector<int>& a, int step)
    {
        for (int gap = step; gap > 0;  gap /= 2)
        {
            for (int i = gap; i < a.size(); ++i)
            {
                int t = a[i];
                for (j = i - gap; j >= 0 && a[j] > t; j -= gap)
                {
                    a[j + gap] = a[j];
                }
                a[j + gap] = t;
            }
        }
    }
#pragma endregion


    void SelectSort(vector<int>& arr) {
        int n = arr.size();
        for (int i = 0; i < n - 1; i++) {
            int min_idx = i;
            // 寻找最小元素的索引
            for (int j = i + 1; j < n; j++) {
                if (arr[j] < arr[min_idx]) {
                    min_idx = j;
                }
            }
            // 将最小元素交换到已排序部分的末尾
            if (min_idx != i) {
                std::swap(arr[i], arr[min_idx]);
            }
        }
    }

#pragma region 基数排序
    //基数排序
    ////LinkTable 为什么声明时不带 * 号？
    //因为* LinkTable 中的* 号已经包含在 typedef 的定义里了。
    //Node 是结构体本身的类型。
    //LinkTable 是 Node* （即指向 Node 结构体的指针）的类型别名。
    //所以，当你写 LinkTable table = new Node[n]; 时，编译器看到的其实等效于 Node* table = new Node[n]; 
    typedef struct
    {
        int next;
        int data;
    }Node, * LinkTable;
    void RadixSort(int a[], int n)
    {

        LinkTable table = new Node[n];
        int maxValue = a[0];
        for (int i = 0; i < n; ++i)
        {
            table[i].data = a[i];
            if (table[i].data > maxValue) maxValue = table[i].data;
            table[i].next = (i == n - 1) ? -1 : i + 1;
        }
        // 初始为空 -1
        int front[10], rear[10];
        int head = 0; // 链表头游标
        for (int exp = 1; (maxValue / exp) > 0; exp *= 10)
        {
            for (int i = 0; i < 10; ++i)
            {
                front[i] = rear[i] = -1;
            }
            int p = head;
            while (p != -1)
            {
                int digit = (table[p].data / exp) % 10;
                int next = table[p].next;
                table[p].next = -1;

                if (front[digit] == -1) front[digit] = p; //初始化为队头队尾
                else table[rear[digit]].next = p; //将该元素连接到队尾
                rear[digit] = p;//置为队尾

                p = next;
            }

            head = -1;
            int tail = -1;

            for (int i = 0; i < 10; ++i)
            {
                //如果当前桶有元素
                if (front[i] != -1)
                {
                    //如果收集的是第一个元素
                    if (head == -1)
                    {
                        head = front[i]; //收集队列的队头
                    }
                    else
                    {
                        //将整个链表挂在上个链表的尾部
                        table[tail].next = front[i];
                    }
                    //将队尾置为当前链表的尾部
                    tail = rear[i];

                }
            }


        }
        //最后的排序完毕的队列头结点
        int p = head;
        for (int i = 0; i < n; ++i)
        {
            a[i] = table[p].data;
            p = table[p].next;
        }

        // 释放内存池，防止内存泄漏
        delete[] table;
    }
#pragma endregion



#pragma endregion

#pragma region 迪杰斯特拉算法
    // 定义无穷大
    const int INF = numeric_limits<int>::max();

    // 辅助函数：根据 prev 数组回溯并打印路径
    void printPath(const string& target, unordered_map<string, string>& prev) {
        vector<string> path;
        for (string at = target; at != ""; at = prev[at]) {
            path.push_back(at);
        }
        // 因为是从目标节点往回找，所以需要反转
        reverse(path.begin(), path.end());

        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i] << (i == path.size() - 1 ? "" : "->");
        }
    }

    // 迪杰斯特拉算法核心实现
    void dijkstra(unordered_map<string, vector<pair<string, int>>>& graph, const string& start) {
        unordered_map<string, int> dist;       // 记录到各个节点的最短距离
        unordered_map<string, string> prev;    // 记录最短路径上的前驱节点

        // 初始化距离为无穷大
        for (const auto& pair : graph) {
            dist[pair.first] = INF;
            prev[pair.first] = "";
        }

        // 源点到自身的距离为 0
        dist[start] = 0;

        // 优先队列（最小堆），存储 {距离, 节点名}
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
        pq.push({ 0, start });

        int step = 1;

        cout << "========== 算法运行步骤 ==========\n";
        while (!pq.empty()) {
            int current_dist = pq.top().first;
            string u = pq.top().second;
            pq.pop();

            // 如果取出的距离大于已记录的最短距离，说明是过期数据，直接跳过
            if (current_dist > dist[u]) continue;

            cout << "[第 " << step++ << " 步] 锁定节点: " << u << " (当前最短距离: " << current_dist << ")\n";

            // 遍历当前节点的所有邻居
            for (const auto& edge : graph[u]) {
                string v = edge.first;
                int weight = edge.second;

                // 松弛操作 (Relaxation)
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    pq.push({ dist[v], v });
                    cout << "  -> 发现更短路径! 更新邻居 [" << v << "] 的距离为: " << dist[v] << " (经由 " << u << ")\n";
                }
            }

            // ====== 核心修改：新增延时，这里暂停 1000 毫秒 (1秒) ======
            this_thread::sleep_for(chrono::milliseconds(1000));
            // =========================================================
        }

        // 打印最终结果，格式对齐图片中的要求
        cout << "\n========== 最终计算结果 (以 " << start << " 为源点) ==========\n";
        vector<string> targetNets = { "Net0", "Net1", "Net2", "Net3", "Net4", "Net5", "Net6" };

        for (const string& net : targetNets) {
            cout << "到" << net << "的最短路径: cost=" << dist[net] << ", ";
            printPath(net, prev);
            cout << "\n";
            // 最终结果也可以加上延时，让阅读更清晰
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
#pragma endregion
#pragma region 拓扑排序
    bool canFinish40(int numCourses, vector<vector<int>>& prerequisites) {
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

#pragma region 2024真题寻找唯一拓扑排序序列
#define MAXV 10
    typedef struct
    {
        int numVertices, numEdges;
        char VerticesList[MAXV];
        int Edge[MAXV][MAXV];
    }MGraph;

    // 辅助函数：初始化图为空图
    void InitGraph(MGraph& G, int numV, const char* vertices) {
        G.numVertices = numV;
        G.numEdges = 0;
        for (int i = 0; i < numV; ++i) {
            G.VerticesList[i] = vertices[i];
        }
        memset(G.Edge, 0, sizeof(G.Edge));
    }

    // 辅助函数：添加有向边 from -> to
    void AddEdge(MGraph& G, int from, int to) {
        if (G.Edge[from][to] == 0) {
            G.Edge[from][to] = 1;
            G.numEdges++;
        }
    }

    int uniquely(MGraph G)
    {
        //！！！拓扑排序的数据结构！！！
        //入度用vector存储
        vector<int> inDegree(G.numVertices);
        //图使用二维vector存储，即类似邻接表存储其出度所对应的顶点，
        // 便于后续在入度数组中直接定位,将入度-1
        //
        vector<vector<int>> graph(G.numVertices);
        for (int i = 0; i < G.numVertices; ++i)
        {
            for (int j = 0; j < G.numVertices; ++j)
            {
                if (G.Edge[j][i] == 1)
                {
                    //构造邻接表
                    graph[j].push_back(i);
                    ++inDegree[i];
                }

            }
        }
        int cnt = 0;
        queue<int> q;
        //先遍历入度为0的节点
        for (int i = 0; i < G.numVertices; ++i)
        {
            if (inDegree[i] == 0)
            {
                cnt++;
                if (cnt > 1)return 0;
                q.push(i);
            }

        }

        while (!q.empty())
        {
            int top = q.front();
            q.pop();
            int cnt = 0;
            //将所有该点连接的顶点入度-1
            for (auto next : graph[top])
            {
                inDegree[next]--;
                if (inDegree[next] == 0)
                {
                    //记得压入入度为0的结点
                    q.push(next);
                    cnt++;
                }
                if (cnt > 1)return 0;

            }
        }
        //判断有环的情况
        for (int i = 0; i < G.numVertices; ++i)
        {
            if (inDegree[i] != 0)return 0;
        }
        return 1;

    }
    using namespace std;
    //优化版本，直接在邻接矩阵中操作
    int uniquely2(MGraph G)
    {
        vector<int> inDegree(G.numVertices);
        for (int i = 0; i < G.numVertices; ++i)
        {
            for (int j = 0; j < G.numVertices; ++j)
            {
                if (G.Edge[j][i] == 1)
                {
                    ++inDegree[i];
                }

            }
        }
        int cnt = 0;
        queue<int> q;
        //先遍历入度为0的节点
        for (int i = 0; i < G.numVertices; ++i)
        {
            if (inDegree[i] == 0)
            {
                cnt++;
                if (cnt > 1)return 0;
                q.push(i);
            }

        }

        while (!q.empty())
        {
            int top = q.front();
            q.pop();
            int cnt = 0;
            //将所有该点连接的顶点入度-1
            for (int next = 0; next < G.numVertices; ++next)
            {
                if (G.Edge[top][next] == 1)
                {
                    inDegree[next]--;
                    if (inDegree[next] == 0)
                    {
                        //记得压入入度为0的结点
                        q.push(next);
                        cnt++;
                    }
                    if (cnt > 1)return 0;
                }


            }
        }
        //判断有环的情况
        for (int i = 0; i < G.numVertices; ++i)
        {
            if (inDegree[i] != 0)return 0;
        }
        return 1;

    }
#pragma endregion

#pragma region 2019真题链表的换序排列


    typedef struct node {
        int data;
        struct node* next;
    } NODE;

    void reorderList(NODE* head)
    {
        if (head == nullptr || head->next == nullptr || head->next->next == nullptr) return;


        // 第一步：快慢指针找中点
    // 【修正】必须从首元结点开始，而不是头结点
        NODE* slow = head->next;
        NODE* fast = head->next;
        while (fast->next && fast->next->next)
        {
            slow = slow->next;       // 慢指针走一步
            fast = fast->next->next; // 快指针走两步
        }

        // 第二步：断开链表，并就地逆置后半段
        NODE* mid = slow->next;      // mid 是后半段的第一个有效结点
        slow->next = nullptr;        // 【关键踩分点：断表！】彻底切断前半段和后半段

        NODE* rhead = nullptr;
        while (mid)
        {
            NODE* node = mid->next;
            mid->next = rhead;
            rhead = mid;
            mid = node;
        }

        NODE* l = head->next;
        NODE* r = rhead;

        while (l && r)
        {
            NODE* t1 = l->next;

            l->next = r;
            NODE* t2 = r->next;
            r->next = t1;
            l = t1;
            r = t2;

        }

    }

    // ---------------- 以下为辅助测试设施 ----------------

    // 工具函数：尾插法将 vector 转换为带头结点的单链表
    NODE* createList(const vector<int>& arr) {
        NODE* head = new NODE;   // 头结点
        head->data = -1;         // 头结点数据域无意义
        head->next = NULL;
        NODE* tail = head;

        for (int val : arr) {
            NODE* newNode = new NODE;
            newNode->data = val;
            newNode->next = NULL;
            tail->next = newNode;
            tail = newNode;
        }
        return head;
    }

    // 工具函数：打印带头结点的单链表
    void printList(NODE* head) {
        if (head == NULL || head->next == NULL) {
            cout << "Empty List" << endl;
            return;
        }
        NODE* p = head->next;
        while (p != NULL) {
            cout << p->data;
            if (p->next != NULL) cout << " -> ";
            p = p->next;
        }
        cout << endl;
    }

    // 工具函数：释放链表内存（工程好习惯）
    void destroyList(NODE* head) {
        NODE* p = head;
        while (p != NULL) {
            NODE* tmp = p->next;
            delete p;
            p = tmp;
        }
    }

    // ---------------- 测试用例执行 ----------------

    void runTest(const string& testName, const vector<int>& inputData) {
        cout << "========== " << testName << " ==========" << endl;
        NODE* list = createList(inputData);

        cout << "重排前: ";
        printList(list);

        reorderList(list);

        cout << "重排后: ";
        printList(list);
        cout << endl;

        destroyList(list); // 避免内存泄漏
    }

#pragma endregion
}
#pragma region 网络拓扑图，邻接表的定义（用于OSPF协议泛洪Dijkstra）
#define MAX_ROUTER_NUM 100 // 定义最大路由器节点数

// --------------------------------------------------------
// 1. 边表结点 (ArcNode)：对应LSI表中的 Link 和 Net 信息
// --------------------------------------------------------
typedef struct ArcNode {
    unsigned int destIP;    // 目标IP：相邻路由器IP地址 或 直连网络前缀
    unsigned int metric;    // 权重：到达目的地的费用 (Metric)
    int type;               // 节点类型：0代表路由器(Link)，1代表直连网络(Net) - [注：此项为工程完善项，考场写出前两项即可满分]
    ArcNode* next;   // 指针：指向下一条链路信息
} ArcNode;

// --------------------------------------------------------
// 2. 顶点表结点 (VNode)：对应LSI表中的 Router ID
// --------------------------------------------------------
typedef struct{
    unsigned int routerID;  // 顶点标识：路由器ID (如 10.1.1.1)
    ArcNode* firstArc;      // 头指针：指向该路由器的第一条链路信息
} VNode, *AdjList;

// --------------------------------------------------------
// 3. 邻接表图结构 (ALGraph)：完整包装整个网络拓扑
// --------------------------------------------------------
typedef struct {
    AdjList vertices;       // 顶点表数组
    int vexNum;             // 当前网络中的路由器(顶点)数量
    int arcNum;             // 当前网络中的链路(边)数量
} ALGraph;
#pragma endregion




void InsertSort(int a[], int n)
{
    for (int i = 1; i < n; ++i)
    {
        int t = a[i];
        int j;
        for (j = i - 1; j >=0 && a[j] > t; --j)
        {
            a[j + 1] = a[j];
        }
        a[j + 1] = t;///!!!!
    }
}
void InsertSort2(int a[], int n)
{
    for (int i = 1; i < n; ++i)
    {
        int t = a[i];
        
        int l = 0, r = i - 1;
        while (l <= r)
        {
            int m = (l + r) / 2;
            if (a[m] > t) r = m - 1;
            else l = m + 1;
        }
        for (int j = i - 1; j >= l && a[j] > t; --j)
        {
            a[j + 1] = a[j];
        }
        a[l] = t;///!!!!
    }
}
void ShellSort(int a[], int n, int step)
{
    for (int gap = step; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; ++i)
        {
            int t = a[i];
            int j;
            for (j = i - gap; j >= 0 && a[j] > t; j -= gap)
            {
                a[j + gap] = a[j];
            }
            a[j + gap] = t;
        }
    }
}

int Patition(int a[], int n, int l, int r)
{
    
    int i = l, j = r;

    while (i < j)
    {
        while (i < j && a[j] >= a[l])--j;
        while (i < j && a[i] <= a[l])++i;
        swap(a[i], a[j]);
    }
    swap(a[i], a[l]);
    for (int i = 0; i < n; ++i)cout << a[i] << " ";
    cout << endl;

    return i;
}
void QuickSort(int a[], int n, int l, int r)
{
    if (l >= r) return;//子数组中只剩下一个元素，排序完毕

    int i = Patition(a, n, l, r);

    QuickSort(a, n, l, i - 1);
    QuickSort(a, n, i+1, r);
}

void AdjustDown(int a[], int n, int root)
{
    while (1)
    {
        int minIndex = root;
        int l = root * 2 + 1, r = root * 2 + 2;
        if (l < n && a[l] < a[minIndex]) minIndex = l;
        if (r < n && a[r] < a[minIndex]) minIndex = r;

        if (root == minIndex) break;
        swap(a[root], a[minIndex]);
        root = minIndex;
    }
}
void Heapify(int a[], int n)
{
    for (int i = (n >> 1) - 1; i >= 0; --i)
    {
        AdjustDown(a, n, i); //从0开始下沉，将大值下沉，小值浮上
    }
}

void HeapSort(int a[], int n)
{
    if (n <= 1) return;

    //建堆，最小值堆顶
    Heapify(a, n);
    //最后一个元素已经排好序
    for (int i = n - 1; i > 0; --i)
    {
        swap(a[0], a[i]);
        AdjustDown(a, i, 0);
    }

}

#pragma region 多项式
//一、
typedef struct Node
{
    int co;
    int ex;
    int cnt;
    Node* next;
}* Polynomial;

Polynomial createPolynomial(const int coefs[], const int exps[], int n) {
    Polynomial head = new Node(); // 建立头结点
    head->next = nullptr;
    head->cnt = 0;

    Node* tail = head; // 尾指针，用于尾插法保持输入顺序
    for (int i = 0; i < n; ++i) {
        if (coefs[i] == 0) continue; // 忽略系数为0的无效项

        Node* newNode = new Node();
        newNode->co = coefs[i];
        newNode->ex = exps[i];
        newNode->next = nullptr;

        tail->next = newNode; // 接入链表尾部
        tail = newNode;       // 尾指针后移
        head->cnt++;          // 计数器递增
    }
    return head;
}

Polynomial Add_Optimized(Polynomial& p1, Polynomial& p2) {
    if (!p1 || !p2) return p1 ? p1 : p2;

    Node* pa = p1->next;      // pa 遍历 p1 有效项
    Node* pb = p2->next;      // pb 遍历 p2 有效项
    Node* tail = p1;          // tail 作为结果链表的尾指针，直接复用 p1 的头结点
    p1->cnt = 0;              // 重置有效项计数

    while (pa != nullptr && pb != nullptr) {
        if (pa->ex > pb->ex) {
            tail->next = pa;  // 牵走 pa 节点
            tail = pa;
            pa = pa->next;
            p1->cnt++;
        }
        else if (pa->ex < pb->ex) {
            tail->next = pb;  // 牵走 pb 节点
            tail = pb;
            pb = pb->next;
            p1->cnt++;
        }
        else { // 指数相等
            int sum = pa->co + pb->co;
            if (sum != 0) {
                pa->co = sum;     // 就地修改 pa 的系数作为保留节点
                tail->next = pa;
                tail = pa;
                pa = pa->next;
                p1->cnt++;

                Node* del_b = pb; // 冗余的 pb 节点必须被释放
                pb = pb->next;
                delete del_b;
            }
            else {
                // 系数抵消为 0，两个节点均失效，必须全部释放
                Node* del_a = pa;
                Node* del_b = pb;
                pa = pa->next;
                pb = pb->next;
                delete del_a;
                delete del_b;
            }
        }
    }

    // 极简拼接：将未遍历完的一条链表直接挂载到 tail 后面
    tail->next = (pa != nullptr) ? pa : pb;

    // 统计剩余挂载片段的节点数量以更新 cnt（408 若无明确要求可省略此步，但工程上必须严谨）
    Node* curr = tail->next;
    while (curr != nullptr) {
        p1->cnt++;
        curr = curr->next;
    }

    delete p2;    // p2 的节点已全部融合或被销毁，释放其孤立的头结点
    p2 = nullptr; // 防止野指针

    return p1;
}
#pragma endregion

#pragma region 并查集
class DSU {
private:
    // 408大纲标准单数组设计：
    // 若 S[i] >= 0，代表其父节点索引；若 S[i] < 0，代表其为根节点，且绝对值为该集合的节点总数。
    std::vector<int> S;
    int cnt;

public:
    // 构造函数：初始化 n 个节点
    DSU(int n) {
        S.resize(n, -1); // 初始状态，所有节点自成一派，大小为1（表现为-1）
    }

    // 核心操作 1：Find (带路径压缩迭代版)
    int find(int x) {
        int root = x;
        // 寻根跳跃：只要值大于等于0，说明是常规子节点
        while (S[root] >= 0) {
            root = S[root];
        }

        // 路径压缩跳跃：将查找路径上的所有节点直接挂载到 root 下
        while (x != root) {
            int t = S[x]; // 暂存原父节点
            S[x] = root;  // 直接指向根
            x = t;        // 游标上移
        }
        return root;
    }

    // 核心操作 2：Union (按规模合并，Union by Weight/Size)
    bool unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY) {
            return false; // 已在同一集合（图论中连边即成环）
        }

        // 按规模合并核心逻辑：
        // 注意 S 中存的是负数！数值越大（越接近0），绝对值越小，代表树的规模越小。
        // 我们强制让 rootX 成为“大树”（即 S[rootX] 更小，更负）。
        if (S[rootX] > S[rootY]) {
            std::swap(rootX, rootY);
        }

        S[rootX] += S[rootY]; // 更新大树的总节点数 (负数累加)
        S[rootY] = rootX;     // 将小树的根节点指向大树的根节点

        return true;
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }

    int getSize(int x) {
        return -S[find(x)]; // 根节点存储的负数取绝对值
    }
    int GetConnectedComponents()
    {
        int cnt = 0;
        for (int i = 0; i < S.size(); ++i)
            if (S[i] < 0) cnt++;

        return cnt;
    }
};

int ComponentCnt(int g[5][5])
{
    int S[5] = { -1,-1,-1,-1,-1 };

    
}
#pragma endregion

#pragma region 共享栈
#define MaxSize 100
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
            s.top--;
            s.data[s.top] = num;
        }
        else if (flag == 0)
        {
            s.bom++;
            s.data[s.bom] = num;
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




int main() {
    //// 使用哈希表构建无向图的邻接表表示
    //unordered_map<string, vector<pair<string, int>>> graph;

    //// R1 的链路
    //graph["R1"] = { {"R2", 3}, {"R3", 1}, {"R4", 3}, {"Net1", 1}, {"Net0", 2} };
    //// R2 的链路
    //graph["R2"] = { {"R1", 3}, {"Net2", 1} };
    //// R3 的链路
    //graph["R3"] = { {"R1", 1}, {"R4", 1}, {"R5", 3}, {"Net3", 2} };
    //// R4 的链路
    //graph["R4"] = { {"R1", 3}, {"R3", 1}, {"R6", 4}, {"Net4", 1} };
    //// R5 的链路
    //graph["R5"] = { {"R3", 3}, {"R6", 2}, {"Net5", 1} };
    //// R6 的链路
    //graph["R6"] = { {"R4", 4}, {"R5", 2}, {"Net6", 3} };

    //// 为了让目标网络(Net)也能正常参与路径回溯，将它们作为叶子节点双向连接
    //graph["Net0"] = { {"R1", 2} };
    //graph["Net1"] = { {"R1", 1} };
    //graph["Net2"] = { {"R2", 1} };
    //graph["Net3"] = { {"R3", 2} };
    //graph["Net4"] = { {"R4", 1} };
    //graph["Net5"] = { {"R5", 1} };
    //graph["Net6"] = { {"R6", 3} };

    //// 运行算法，以 R1 为起点
    //dijkstra(graph, "R1");


    /*cout << myAtoi("   +1");*/

#pragma region 唯一拓扑测试案例


    //MGraph G1;
    //InitGraph(G1, 4, "ABCD"); // 4个顶点：A(0), B(1), C(2), D(3)

    //// 构造菱形：A -> B, A -> C, B -> D, C -> D
    //AddEdge(G1, 0, 1); // A -> B
    //AddEdge(G1, 0, 2); // A -> C
    //AddEdge(G1, 1, 3); // B -> D
    //AddEdge(G1, 2, 3); // C -> D

    //MGraph G2;
    //InitGraph(G2, 4, "ABCD"); // 4个顶点：A(0), B(1), C(2), D(3)

    //// 构造单链：A -> B -> C -> D
    //AddEdge(G2, 0, 1); // A -> B
    //AddEdge(G2, 1, 2); // B -> C
    //AddEdge(G2, 2, 3); // C -> D

    ///* 邻接矩阵形态：
    //   0 1 0 0
    //   0 0 1 0
    //   0 0 0 1
    //   0 0 0 0
    //*/
    //MGraph G3;
    //InitGraph(G3, 4, "ABCD"); // 4个顶点：A(0), B(1), C(2), D(3)

    //// 构造单链：A -> B -> C -> D
    //AddEdge(G3, 0, 1); // A -> B
    //AddEdge(G3, 1, 2); // B -> C
    //AddEdge(G3, 2, 0); // C -> D
    //AddEdge(G3, 3, 0); // C -> D

    //cout << "我是有向无环图，且有多个菱形" << uniquely(G1) << endl;
    //cout << "我是有向无环图，且单链序列" << uniquely(G2) << endl;
    //cout << "我是有环图" << uniquely(G3) << endl;
#pragma endregion
#pragma region 链表换序案例
    // 测试用例 1：常规偶数个节点
    // 预期输出: 1 -> 6 -> 2 -> 5 -> 3 -> 4
    //runTest("Test 1: 偶数个节点 (6个)", { 1, 2, 3, 4, 5, 6 });

    //// 测试用例 2：常规奇数个节点
    //// 预期输出: 1 -> 5 -> 2 -> 4 -> 3
    //runTest("Test 2: 奇数个节点 (5个)", { 1, 2, 3, 4, 5 });

    //// 测试用例 3：极限边界条件（短链表防御）
    //// 预期输出: 1 -> 2 (不应发生崩溃或改变)
    //runTest("Test 3: 边界测试 (仅2个数据节点)", { 1, 2 });

    //// 测试用例 4：极限边界条件（只有一个数据节点）
    //// 预期输出: 1
    //runTest("Test 4: 边界测试 (仅1个数据节点)", { 1 });

    //// 测试用例 5：极限边界条件（空链表，只有头结点）
    //// 预期输出: Empty List
    //runTest("Test 5: 边界测试 (空链表)", {});
#pragma endregion
    vector<int> arr = { 3, 1, 4, 1, 5, 9, 2,6, 12, 23, 14, 11, 23, 33 };
    //vector<int> arr1 = { 3, 1, 4, 1, 5, 9, 2,6, 12, 23, 14, 11, 23, 34, 23, 22 ,22 ,231, 222 };
    int arr1[19] = {3, 1, 4, 1, 5, 9, 2,6, 12, 23, 14, 11, 23, 34, 23, 22 ,22 ,231, 222};
    int a[11] = { 23,17,72,60,25,8,2, 5, 13, 6, 3};
    int a1[15] = {36, 18, 10, 2, 88, 53, 27, 99,0,38,46,77,78,55,40};
   // My408::Heapify(arr);
    //vector<int> res = My408::getTopK(arr);
    ////for (int x : arr) cout << x << " ";
    //for (int x : res) cout << x << " ";
    //// 输出：1 1 2 3 4 5 6 9


    //My408::InsertSort2(arr);
    //for (int x : arr) cout << x << " ";
    ////My408::InsertSort(arr1);
    ////for (int x : arr1) cout << x << " ";
    //cout << endl;
    ////My408::QuickSort(arr1, 0, arr1.size()-1);
    ////for (int x : arr1) cout << x << " ";
    //
    //My408::ShellSort(arr1,(arr1.size() + 1) / 2);
    //for (int x : arr1) cout << x << " ";
    //cout << endl;
    //My408::HeapSort(a,9);
    //for (int i = 0; i < 9; ++i)
    //    cout << a[i] << " ";

    //InsertSort2(a, 6);
    //for (int x : a) cout << x << " ";
    
   /* My408::RadixSort(a, 11);
    for (int x : a) cout << x << " ";

    cout << endl;*/

    /*QuickSort(arr1, 19, 0, 18);*/
    //HeapSort(arr1, 19);
    //for (int x : arr1) cout << x << " ";
    //cout << endl;
    //int res[3];
    //GetTopK2(a, 6, 3, res);

    //for (int x : res) cout << x << " ";

    // 第一个多项式：3x^2 + 2x + 1

    //Node p2;
    //int co1[] = { 3, 2, 1 };
    //int ex1[] = { 2, 1, 0 };
    //Polynomial poly1 = createPolynomial(co1, ex1, 3);

    //// 第二个多项式：4x^3 + x (注意降序排列是链表多项式相加的前提)
    //int co2[] = { 4, 1 };
    //int ex2[] = { 3, 1 };
    //Polynomial poly2 = createPolynomial(co2, ex2, 2);

    /*QuickSort(a1, 15, 0, 14);*/
    std::cout << "==================================================\n";
    std::cout << "         408数据结构：一元多项式就地相加评测          \n";
    std::cout << "==================================================\n\n";

    // ---- 【测试样例一：指数完全错开】 ----
    {
        std::cout << "【测试样例一：指数完全错开】\n";
        Polynomial p1 = createPolynomial({ 3, 2 }, { 5, 3 });
        Polynomial p2 = createPolynomial({ 5, 1 }, { 4, 1 });
        std::cout << "多项式 A: "; printPolynomial(p1);
        std::cout << "多项式 B: "; printPolynomial(p2);
        p1 = Add_Optimized(p1, p2);
        std::cout << "相加结果: "; printPolynomial(p1);
        std::cout << "--------------------------------------------------\n";
    }

    // ---- 【测试样例二：指数相同系数不为0】 ----
    {
        std::cout << "【测试样例二：存在同次项合并】\n";
        Polynomial p1 = createPolynomial({ 4, 2 }, { 4, 2 });
        Polynomial p2 = createPolynomial({ 3, 1 }, { 4, 2 });
        std::cout << "多项式 A: "; printPolynomial(p1);
        std::cout << "多项式 B: "; printPolynomial(p2);
        p1 = Add_Optimized(p1, p2);
        std::cout << "相加结果: "; printPolynomial(p1);
        std::cout << "--------------------------------------------------\n";
    }

    // ---- 【测试样例三：高频陷阱-系数抵消为0】 ----
    {
        std::cout << "【测试样例三：高频陷阱-系数抵消】\n";
        Polynomial p1 = createPolynomial({ 5, 3, 2 }, { 6, 4, 1 });
        Polynomial p2 = createPolynomial({ -5, 1 }, { 6, 2 });
        std::cout << "多项式 A: "; printPolynomial(p1);
        std::cout << "多项式 B: "; printPolynomial(p2);
        p1 = Add_Optimized(p1, p2);
        std::cout << "相加结果: "; printPolynomial(p1);
        std::cout << "--------------------------------------------------\n";
    }

    // ---- 【测试样例四：一长一短且短链表先耗尽】 ----
    {
        std::cout << "【测试样例四：短链表先耗尽】\n";
        int a1[3] = { 9, 7, 5 };
        int a2[3] = { 8, 7, 5 };
        int a3[1] = { 2 };
        int a4[3] = { 7 };
        Polynomial p1 = createPolynomial(a1, a2, 3);
        Polynomial p2 = createPolynomial(a3, a4, 1);
        std::cout << "多项式 A: "; printPolynomial(p1);
        std::cout << "多项式 B: "; printPolynomial(p2);
        p1 = Add_Optimized(p1, p2);
        std::cout << "相加结果: "; printPolynomial(p1);
        std::cout << "--------------------------------------------------\n";
    }

    // ---- 【测试样例五：极端边界-空链表输入】 ----
    {
        std::cout << "【测试样例五：极端边界-存在空多项式】\n";
        Polynomial p1 = createPolynomial({}, {}); // 空
        Polynomial p2 = createPolynomial({ 3, 1 }, { 2, 0 });
        std::cout << "多项式 A: "; printPolynomial(p1);
        std::cout << "多项式 B: "; printPolynomial(p2);
        p1 = Add_Optimized(p1, p2);
        std::cout << "相加结果: "; printPolynomial(p1);
        std::cout << "==================================================\n";
    }




    return 0;

}