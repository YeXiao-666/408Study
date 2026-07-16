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
#pragma endregion


   //!!!不考察代码!!! 
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
                if (a[m] > key)r = m - 1;   //m-1

                else l = m + 1;    // m + 1
            }

            // 大头 O n
            for (int j = i - 1; j >= l; --j)
            {
                if (a[j] == key) cout << "不稳定" << endl;
                a[j + 1] = a[j];
                
            }
                
            a[l] = key;                   // 1  2 2  5
        }
    }

#pragma region 快速排序
    int patition(vector<int>& a, int l, int r)
    {
        // l 为基准元素
        int i = l, j = r;

        while (i < j)
        {
            //注意要大于等于，小于等于，否则遇到与基准元素相同的元素时会死循环，ij不动
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
    struct ArcNode* next;   // 指针：指向下一条链路信息
} ArcNode;

// --------------------------------------------------------
// 2. 顶点表结点 (VNode)：对应LSI表中的 Router ID
// --------------------------------------------------------
typedef struct VNode {
    unsigned int routerID;  // 顶点标识：路由器ID (如 10.1.1.1)
    ArcNode* firstArc;      // 头指针：指向该路由器的第一条链路信息
} VNode, AdjList[MAX_ROUTER_NUM];

// --------------------------------------------------------
// 3. 邻接表图结构 (ALGraph)：完整包装整个网络拓扑
// --------------------------------------------------------
typedef struct {
    AdjList vertices;       // 顶点表数组
    int vexNum;             // 当前网络中的路由器(顶点)数量
    int arcNum;             // 当前网络中的链路(边)数量
} ALGraph;
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
    vector<int> arr1 = { 3, 1, 4, 1, 5, 9, 2,6, 12, 23, 14, 11, 23, 34, 23, 22 ,22 ,231, 222 };
    int a[9] = { 23,17,72,60,25,8,68,71,52 };
   // My408::Heapify(arr);
    //vector<int> res = My408::getTopK(arr);
    ////for (int x : arr) cout << x << " ";
    //for (int x : res) cout << x << " ";
    //// 输出：1 1 2 3 4 5 6 9


    My408::InsertSort2(arr);
    for (int x : arr) cout << x << " ";
    //My408::InsertSort(arr1);
    //for (int x : arr1) cout << x << " ";
    cout << endl;
    //My408::QuickSort(arr1, 0, arr1.size()-1);
    //for (int x : arr1) cout << x << " ";
    
    My408::ShellSort(arr1,(arr1.size() + 1) / 2);
    for (int x : arr1) cout << x << " ";
    cout << endl;
    My408::HeapSort(a,9);
    for (int i = 0; i < 9; ++i)
        cout << a[i] << " ";

    

    return 0;

}