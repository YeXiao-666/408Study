#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

#pragma region 网易互娱第五人格
extern const int INF = 1e9;
//
//struct Point {
//    int r, c;
//};
//
//int n, m, k;
//vector<string> grid;
//Point start_p, exit_p, hunter_p;
//vector<Point> generators;
//
//int dr[] = { 0, 0, 1, -1 };
//int dc[] = { 1, -1, 0, 0 };
//
//// 检查格子是否安全且可通行
//bool is_safe(int r, int c) {
//    if (r < 0 || r >= n || c < 0 || c >= m) return false;
//    if (grid[r][c] == '#') return false;
//    int dist_to_hunter = abs(r - hunter_p.r) + abs(c - hunter_p.c);
//    return dist_to_hunter > k;
//}
//
//// BFS 计算点 s 到地图上所有点的最短距离
//vector<vector<int>> bfs(Point s) {
//    vector<vector<int>> d(n, vector<int>(m, INF));
//    if (!is_safe(s.r, s.c)) return d;
//    d[s.r][s.c] = 0;
//    queue<Point> q;
//    q.push(s);
//    while (!q.empty()) {
//        Point curr = q.front();
//        q.pop();
//        for (int i = 0; i < 4; ++i) {
//            int nr = curr.r + dr[i];
//            int nc = curr.c + dc[i];
//            if (is_safe(nr, nc) && d[nr][nc] == INF) {
//                d[nr][nc] = d[curr.r][curr.c] + 1;
//                q.push({ nr, nc });
//            }
//        }
//    }
//    return d;
//}
//
//int main31313() {
//    // 输入地图尺寸和距离约束
//    if (!(cin >> n >> m >> k)) return 0;
//    grid.resize(n);
//    for (int i = 0; i < n; ++i) {
//        cin >> grid[i];
//        for (int j = 0; j < m; ++j) {
//            if (grid[i][j] == 'S') start_p = { i, j };
//            else if (grid[i][j] == 'E') exit_p = { i, j };
//            else if (grid[i][j] == 'T') hunter_p = { i, j };
//            else if (grid[i][j] == '$') generators.push_back({ i, j });
//        }
//    }
//
//    // 汇总所有关键点：0为起点，1-7为电机，8为出口
//    vector<Point> points;
//    points.push_back(start_p);
//    for (auto p : generators) points.push_back(p);
//    points.push_back(exit_p);
//
//    int num_pts = points.size();
//    vector<vector<int>> adj(num_pts, vector<int>(num_pts, INF));
//
//    // 计算关键点之间的两两距离
//    for (int i = 0; i < num_pts; ++i) {
//        vector<vector<int>> d = bfs(points[i]);
//        for (int j = 0; j < num_pts; ++j) {
//            adj[i][j] = d[points[j].r][points[j].c];
//        }
//    }
//
//    int num_gen = generators.size(); // 应该是 7
//    // dp[mask][i] 表示修完 mask 集合的电机，当前在第 i 个电机
//    vector<vector<int>> dp(1 << num_gen, vector<int>(num_gen, INF));
//
//    // 初始化：从起点到第一个电机的距离
//    for (int i = 0; i < num_gen; ++i) {
//        if (adj[0][i + 1] != INF) {
//            dp[1 << i][i] = adj[0][i + 1];
//        }
//    }
//
//    // 状态转移
//    for (int mask = 1; mask < (1 << num_gen); ++mask) {
//        for (int i = 0; i < num_gen; ++i) {
//            if (!(mask & (1 << i)) || dp[mask][i] == INF) continue;
//            for (int j = 0; j < num_gen; ++j) {
//                if (mask & (1 << j)) continue;
//                if (adj[i + 1][j + 1] != INF) {
//                    dp[mask | (1 << j)][j] = min(dp[mask | (1 << j)][j], dp[mask][i] + adj[i + 1][j + 1]);
//                }
//            }
//        }
//    }
//
//    // 统计结果：修够至少 5 个电机并前往出口
//    int min_dist = INF;
//    for (int mask = 0; mask < (1 << num_gen); ++mask) {
//        int count = 0;
//        for (int i = 0; i < num_gen; ++i) if (mask & (1 << i)) count++;
//        if (count >= 5) {
//            for (int i = 0; i < num_gen; ++i) {
//                if (dp[mask][i] != INF && adj[i + 1][num_pts - 1] != INF) {
//                    min_dist = min(min_dist, dp[mask][i] + adj[i + 1][num_pts - 1]);
//                }
//            }
//        }
//    }
//
//    if (min_dist == INF) cout << -1 << endl; // 无法完成任务
//    else cout << min_dist << endl;
//
//    return 0;
//}
#pragma endregion

#pragma region 网易互娱2027暑假提前批守卫落座问题

#pragma region 错解
//int main67()
//{
//	int n, m;
//	int x, y;
//	cin >> n >> m;
//	vector<bool> isOccupied(n + 1, false);
//	vector<int> res(m);
//	vector<int> guardSeat(n + 1);
//	set<int> occupied;
//	int cnt = 0;
//	for (int i = 0; i < m; i++)
//	{
//		cin >> x >> y;
//		if (x == 1)
//		{
//			cnt++;
//			if (cnt & 1)
//			{
//				isOccupied[cnt-1] = true;
//				res[i] = cnt - 1;
//				guardSeat[y] = cnt - 1;
//				occupied.insert(cnt - 1);
//			}
//			else
//			{
//				isOccupied[cnt-1] = true;
//				res[i] = cnt - 1;
//				guardSeat[y] = cnt - 1;
//				occupied.insert(cnt - 1);
//			}
//		}
//		if (x == 2)
//		{
//			cnt--;
//			if (guardSeat[y])
//			{
//				isOccupied[guardSeat[y]] = false;
//				res[i] = guardSeat[y];
//				guardSeat[y] = 0;
//				occupied.erase(guardSeat[y]);
//			}
//
//		}
//	}
//	for (int i = 0; i < m; i++)
//	{
//		if (res[i])
//		{
//			cout << " "<< res[i];
//		}
//	}
//	return 0;
//}
#pragma endregion

#pragma region 优先队列正解1
//#include <iostream>
//#include <vector>
//#include <set>
//#include <queue>
//#include <unordered_map>
//using namespace std;
//
//struct Interval {
//    int l, r;      // 区间边界（l和r已被占，中间为空）
//    int pos;       // 该区间内推荐入座的座位
//    int dist;      // 该座位的“最近距离”
//    Interval(int left, int right, int n) : l(left), r(right) {
//        if (l == 0) {               // 左边界
//            pos = 1;
//            dist = r - 1;
//        } else if (r == n + 1) {   // 右边界
//            pos = n;
//            dist = n - l;
//        } else {                   // 中间区间
//            pos = (l + r) / 2;
//            dist = min(pos - l, r - pos);
//        }
//    }
//    // 优先队列需要按 dist 降序，dist 相同按 pos 升序（保证可重复性）
//    bool operator < (const Interval& other) const {
//        if (dist != other.dist) return dist < other.dist;
//        return pos > other.pos;     // 小pos优先，这里 > 是因为 priority_queue 是最大堆
//    }
//};
//
//int main() {
//    ios::sync_with_stdio(false);
//    cin.tie(nullptr);
//
//    int n, m;
//    cin >> n >> m;
//
//    set<int> occupied = {0, n + 1};          // 虚拟边界
//    priority_queue<Interval> pq;
//    unordered_map<int, int> guardPos;        // 守卫编号 -> 当前座位
//
//    // 初始整个区间 (0, n+1)
//    pq.emplace(0, n + 1, n);
//
//    vector<int> ans;  // 用于保存所有输出结果
//
//    while (m--) {
//        int t, d;
//        cin >> t >> d;
//
//        if (t == 1) {   // 到达
//            // 取出有效的最优区间
//            while (!pq.empty()) {
//                Interval cur = pq.top();
//                // 检查该区间是否仍然有效：l 和 r 在 occupied 中且相邻
//                auto itL = occupied.find(cur.l);
//                if (itL != occupied.end() && next(itL) != occupied.end() && *next(itL) == cur.r) {
//                    break;
//                }
//                pq.pop();   // 无效区间，丢弃
//            }
//
//            Interval best = pq.top();
//            pq.pop();
//
//            int seat = best.pos;
//            ans.push_back(seat);   // 保存结果
//
//            // 记录守卫位置
//            guardPos[d] = seat;
//            // 插入新座位
//            occupied.insert(seat);
//            // 分裂出两个新区间加入堆
//            pq.emplace(best.l, seat, n);
//            pq.emplace(seat, best.r, n);
//
//        } else {        // 离开
//            int seat = guardPos[d];
//            guardPos.erase(d);
//            ans.push_back(seat);   // 保存结果
//
//            // 找到前驱和后继
//            auto it = occupied.find(seat);
//            int l = *prev(it);
//            int r = *next(it);
//            occupied.erase(it);
//
//            // 合并出新区间
//            pq.emplace(l, r, n);
//        }
//    }
//
//    // 最后统一输出所有结果
//    for (int x : ans) {
//        cout <<" "<< x;
//    }
//
//    return 0;
//}
#pragma endregion

#pragma region 优先队列正解
//struct Interval
//{
//    int l, r, pos, dis;
//    bool operator < (const Interval& inter)const
//    {
//        if (inter.dis != this->dis) return  this->dis < inter.dis;
//        return this->pos > inter.pos;
//    }
//};
//
//int mainre43()
//{
//    int n, m;
//    int t, d;
//    vector<int>res;
//    set<int>seated;
//    map<int, int> guards;
//    priority_queue<Interval>pq;
//    cin >> n >> m;
//    auto add_interval = [&](int l, int r)
//        {
//            if (l + 1 >= r)return;
//            pq.push({ l, r, (l + r)>> 1, (r-l)>>1 });
//        };
//    for (int i = 0; i < m; ++i)
//    {
//        cin >> t >> d;
//        if (t == 1)
//        {
//            int best_pos = -1;
//            if (seated.empty())best_pos = 1;
//
//            else
//            {
//                int bl = *seated.begin() - 1;
//                int br = n - *seated.rbegin();
//                int max_dis = -1;
//                while (!pq.empty())
//                {
//                    Interval top = pq.top();
//                    auto it = seated.find(top.l);
//                    if (it == seated.end() || next(it) == seated.end() || *(next(it)) != top.r)
//                    {
//                        pq.pop();
//                        continue;
//                    }
//                    if (top.dis > max_dis || (top.dis == max_dis && top.pos < best_pos))
//                    {
//                        best_pos = top.pos;
//                        max_dis = top.dis;
//                    }
//                    break;
//                }
//                if (bl >= max_dis)
//                {
//                    if (bl > max_dis || 1 < best_pos)
//                    {
//                        best_pos = 1;
//                        max_dis = bl;
//                    }
//
//                }
//                if (br > max_dis)
//                {
//                    best_pos = n;
//                    max_dis = br;
//                }
//                else if (br == max_dis && n < best_pos) 
//                {
//                    best_pos = n;
//                }
//                
//            }
//            auto it = seated.insert(best_pos).first;
//            auto pre = (it != seated.begin()) ? prev(it) : seated.end();
//            auto nt = next(it);
//            if (pre != seated.end()) add_interval(*pre, *it);
//            if (nt != seated.end()) add_interval(*it, *nt);
//            guards[d] = best_pos;
//            res.push_back(best_pos);
//        }
//        else
//        {
//            if (guards.find(d) == guards.end())continue;
//            auto it = seated.find(guards[d]);
//            auto pre = (it == seated.begin()) ? seated.end() : prev(it);
//            auto nt = next(it);
//            if (pre != seated.end() && nt != seated.end())
//                add_interval(*pre, *nt);
//
//            res.push_back(guards[d]);
//            seated.erase(guards[d]);
//            guards.erase(d);
//        }
//    }
//    for (int num : res)
//    {
//        cout << " " << num;
//    }
//    return 0;
//}
#pragma endregion


#pragma endregion

#pragma region 网易互娱2026年前骑士药水冲刺问题
// 定义点结构体，存储坐标
//struct Point {
//	double x, y;
//};
//
//// 计算两点之间的欧几里得距离
//double get_dist(Point a, Point b) {
//	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
//}
//
//// 定义一个无穷大的值，表示不可达
//const int INF = 1e9;
//int main() {
//	int n;          // 点的数量
//	double r1, r2;  // r1 是蓝色药瓶距离，r2 是红色药瓶距离
//	if (!(cin >> n >> r1 >> r2)) return 0;
//
//	vector<Point> p(n); // 存储所有点的坐标
//	for (int i = 0; i < n; ++i) cin >> p[i].x >> p[i].y;
//	auto bfs = [&](int start) {
//		vector<int> d(n, INF); // 存储到每个点的最少蓝色药瓶数，初始为无穷大
//		queue<int> q;
//		d[start] = 0;          // 起点距离为 0
//		q.push(start);
//
//		while (!q.empty()) {
//			int u = q.front(); q.pop();
//			for (int v = 0; v < n; ++v) {
//				// 如果点 v 还没访问过，且 u 到 v 的距离在蓝色药瓶范围内
//				if (d[v] == INF && get_dist(p[u], p[v]) <= r1) {
//					d[v] = d[u] + 1; // 步数 +1
//					q.push(v);
//				}
//			}
//		}
//		return d; // 返回从 start 出发到所有点的距离数组
//		};
//	// 计算从起点（索引 0）出发，只用蓝瓶的距离
//	vector<int> d1 = bfs(0);
//
//	// 如果终点（索引 n-1）可达
//	if (d1[n - 1] != INF) {
//		cout << 0 << " " << d1[n - 1] << endl; // 输出：0个红瓶，d1[n-1]个蓝瓶
//		return 0; // 直接结束
//	}
//	// 计算从终点（索引 n-1）出发，只用蓝瓶反向到达所有点的距离
//	vector<int> dn = bfs(n - 1);
//
//	int min_blue = INF; // 记录最少的蓝瓶使用量
//
//	// 遍历所有可能的“跳跃点对” i 和 j
//	for (int i = 0; i < n; ++i) {
//		if (d1[i] == INF) continue; // 如果起点走蓝瓶到不了 i，跳过
//
//		for (int j = 0; j < n; ++j) {
//			if (dn[j] == INF) continue; // 如果终点走蓝瓶到不了 j，跳过
//
//			// 如果 i 和 j 之间的距离在红色药瓶范围内
//			if (get_dist(p[i], p[j]) <= r2) {
//				// 总蓝瓶数 = (起点到i的蓝瓶数) + (j到终点的蓝瓶数)
//				min_blue = min(min_blue, d1[i] + dn[j]);
//			}
//		}
//	}
//	if (min_blue == INF) {
//		cout << "-1 -1" << endl; // 还是到不了
//	}
//	else {
//		cout << 1 << " " << min_blue << endl; // 输出：1个红瓶，最少蓝瓶数
//	}
//
//	return 0;
//}
#pragma endregion


#pragma region 网易互娱7天
//int n, m;
	//cin >> n >> m;
	//vector<int>res;
	//for (int i = 1; i < 8; ++i)
	//{
	//	int curRes = 0;
	//	try
	//	{
	//		/*if (7 - i == 0)throw new char[9]{ "error111" };*/
	//		if (7 - i == 0)throw exception("errrrrrro");
	//		/*if (7 - i == 0)throw MyException();*/
	//		curRes = max(0, min(n, (7 * n - m) / (7 - i)));
	//		curRes = min(curRes, m / i);
	//	}
	//	catch (const char* c)
	//	{
	//		cout << c << endl;
	//	}
	//	catch (int e)
	//	{
	//		curRes = m / e;
	//	}
	//	catch (MyException& e)
	//	{
	//		cout << e.what() << endl;
	//	}
	//	
	//	res.push_back(curRes);
	//}
	//print(res);
#pragma endregion

#pragma region 网易互娱26.3.1妖伞传递
//妖伞传递
	//int n;
	//cin >> n;
	//vector<long long> p(n + 1);
	//vector<long long> q(n + 1);
	//vector<long long> res(n + 1);
	//
	//for (int i = 1; i <= n; ++i) {
	//	cin >> p[i] >> q[i];
	//}
	//deque<int>dq;
	//dq.push_back(1);
	//long long curPos = p[1];
	//int nextIndex = 2;
	//while (!dq.empty() || nextIndex <= n)
	//{
	//	if (dq.empty())
	//	{
	//		dq.push_back(nextIndex);
	//		++nextIndex;
	//	}
	//	else
	//	{
	//		int first = dq.front();
	//		int finalPos = q[first];
	//		bool isJoin = false;
	//		if (nextIndex <= n && p[nextIndex] < finalPos)
	//		{
	//
	//			isJoin = true;
	//			finalPos = p[nextIndex];
	//
	//		}
	//		res[first] += finalPos - curPos;
	//		curPos = finalPos;
	//		if (isJoin)
	//		{
	//			dq.push_back(nextIndex);
	//			++nextIndex;
	//		}
	//		else
	//		{
	//			dq.pop_front();
	//			auto it = dq.begin();
	//			while (it != dq.end()) {
	//				if (q[*it] <= curPos) {
	//					it = dq.erase(it);
	//				}
	//				else {
	//					++it;
	//				}
	//			}
	//		}
	//
	//
	//	}
	//
	//}
	//for (int i = 1; i <= n; ++i) {
	//	cout << res[i] << (i == n ? "" : " ");
	//}
#pragma region 妖伞传递法2
//typedef long long ll;
//
//struct Person {
//    ll p, q;
//};
//妖伞传递
//int main3123() {
//	int n;
//	if (!(std::cin >> n)) return 0;
//	vector<Person> people(n);
//	for (int i = 0; i < n; ++i) cin >> people[i].p >> people[i].q;
//
//	vector<ll> ans(n, 0);
//	vector<bool> joined(n, false);
//	deque<int> team;
//	ll current_pos = people[0].p;
//
//	team.push_back(0);
//	joined[0] = true;
//	int next_to_join = 1;
//
//	while (!team.empty() || next_to_join < n) {
//		if (team.empty()) {
//			int k = next_to_join;
//			team.push_back(k);
//			joined[k] = true;
//			next_to_join++;
//		}
//		else {
//			int holder = team.front();
//			ll min_q = -1;
//			for (int idx : team) {
//				if (min_q == -1 || people[idx].q < min_q) min_q = people[idx].q;
//			}
//			ll next_p = (next_to_join < n) ? people[next_to_join].p : -1;
//
//			ll target = min_q;
//			bool join_event = false;
//			if (next_p != -1 && next_p < target) {
//				target = next_p;
//				join_event = true;
//			}
//
//			ans[holder] += (target - current_pos);
//			current_pos = target;
//
//			if (join_event) {
//				team.push_back(next_to_join);
//				joined[next_to_join] = true;
//				next_to_join++;
//			}
//			else {
//				deque<int> next_team;
//				for (int idx : team) {
//					if (people[idx].q > target) next_team.push_back(idx);
//				}
//				team = next_team;
//			}
//		}
//	}
//
//	for (int i = 0; i < n; ++i) cout << ans[i] << (i == n - 1 ? "" : " ");
//	cout << endl;
//	return 0;
//}
#pragma endregion
#pragma endregion

#pragma region 网易雷火2024
			// 优化输入输出流速度，防止大数据量时超时
//ios_base::sync_with_stdio(false);
//cin.tie(NULL);
//int M;
//if (!(cin >> M)) return 0;
//double totalAccuracy = 0.0;
//for (int i = 0; i < M; ++i)
//{
//	int cursum = 0;
//	string s, s1;
//	cin >> s >> s1;
//	if (s[0] == s1[0]) cursum += 2;
//	string s1sub = s1.substr(1, s.length() - 6);
//	//if (s.compare(1, s.length() - 6, s1sub) == 0) cursum += 3;
//	if (s.compare(1, s.length() - 6, s1, 1, s.length() - 6) == 0) cursum += 3;
//	string plateRec = s.substr(s.length() - 5);
//	string plateCor = s1.substr(s1.length() - 5);
//	cursum += getLCSLength(plateRec, plateCor);
//	totalAccuracy += (cursum / 10.0);
//}
//// 4. 计算总体识别率 (乘以100转为百分比)
//double finalAccuracy = (totalAccuracy / M) * 100.0;
//
//// 5. 格式化输出
//// fixed 和 setprecision(2) 会自动处理四舍五入以及末尾补0的情况 (如 85.00%)
//cout << fixed << setprecision(2) << "总体识别率：" << finalAccuracy << "%" << endl;
#pragma endregion
#pragma region 网易雷火AppCount
//int N, M;
	//if (!(cin >> N >> M)) return 0;

	//// 正确初始化图：N 个空 vector
	//vector<vector<int>> graph(N);
	//for (int i = 0; i < N; ++i) {
	//	int n;
	//	cin >> n;
	//	for (int j = 0; j < n; ++j) {
	//		int dependency;
	//		cin >> dependency;
	//		graph[i].push_back(dependency);
	//	}
	//}

	//// 记录应用主动发起的、尚未释放的请求
	//vector<bool> app_requests(N, false);
	//int max_resident = 0;
	////解决这类“带有循环依赖的资源管理”问题，最安全、最稳妥的工业级做法是放弃引用计数，
	////直接模拟垃圾回收（GC）的可达性分析。核心思路：任何时刻，
	////驻留在内存中的资源 ＝ 由当前所有“尚未释放的应用请求”出发，沿着依赖网能遍历到的所有资源。
	////由于数据量并不大。
	////我们完全可以在每次应用请求 / 释放后，跑一次 BFS（广度优先搜索）来统计当前存活的资源总数。
	//for (int i = 0; i < M; ++i)
	//{
	//	int used, id;
	//	cin >> used >> id;
	//	if (used == 1) app_requests[id] = true;
	//	else if(used == 0) app_requests[id] = false;

	//	queue<int>q;
	//	vector<bool>loads(N, false);
	//	for (int j = 0; j < N; ++j)
	//	{
	//		if (app_requests[j])
	//		{
	//			loads[j] = true;
	//			q.push(j);
	//		}
	//	}
	//	int curAppCnt = 0;
	//	while (!q.empty())
	//	{
	//		int cur = q.front();
	//		q.pop();
	//		++curAppCnt;
	//		for (int depend : graph[cur])
	//		{
	//			if (!loads[depend])
	//			{
	//				loads[depend] = true;
	//				q.push(depend);
	//			}
	//		}
	//	}
	//	max_resident = max_resident > curAppCnt ? max_resident : curAppCnt;
	//}

	//cout << max_resident << "\n";
#pragma endregion

#pragma region 2026.3.21雷火吞噬变强问题
// 提升 cin/cout 的执行效率
	//ios::sync_with_stdio(false);
	//cin.tie(nullptr);

	//int n;
	//long long x;

	//// 读取 n 和初始攻击力 x
	//if (!(cin >> n >> x)) return 0;

	//// 读取所有敌人的攻击力
	//vector<long long> a(n);
	//for (int i = 0; i < n; ++i) {
	//	cin >> a[i];
	//}

	//int max_battles = 0; // 记录历史最高连胜轮数

	//// 枚举每一个进入战斗的起始位置 s
	//for (int s = 0; s < n; ++s) {

	//	// 剪枝优化：如果剩下没打的敌人数量 <= 当前最大连胜记录，直接结束
	//	// 因为即使剩下的全胜，也无法超过 max_battles 了
	//	if (n - s <= max_battles) {
	//		break;
	//	}

	//	long long current_x = x; // 每次从新起点开始，攻击力重置为初始值 x
	//	int current_battles = 0; // 当前起点的连胜轮数

	//	// 从位置 s 开始向后挑战敌人
	//	for (int i = s; i < n; ++i) {
	//		// 严格大于敌人的攻击力才能获胜
	//		if (current_x > a[i]) {
	//			current_battles++;
	//			current_x += a[i]; // 胜利后吸收攻击力
	//		}
	//		else {
	//			break; // 打不过，连胜中断，跳出内层循环
	//		}
	//	}

	//	// 更新历史最高连胜记录
	//	max_battles = max(max_battles, current_battles);
	//}

	//// 输出最多可以进行的战斗轮数
	//cout << max_battles << "\n";
#pragma endregion

#pragma region 网易雷火矩形
//struct Rect {
//	int x1, y1, x2, y2;
//};
//
//int main34() {
//	int n;
//	if (!(cin >> n)) return 0;
//	vector<Rect> rects(n);
//	vector<int> ux, uy;
//	for (int i = 0; i < n; ++i) {
//		cin >> rects[i].x1 >> rects[i].y1 >> rects[i].x2 >> rects[i].y2;
//		ux.push_back(rects[i].x1);
//		ux.push_back(rects[i].x2);
//		uy.push_back(rects[i].y1);
//		uy.push_back(rects[i].y2);
//	}
//
//	sort(ux.begin(), ux.end());
//	ux.erase(unique(ux.begin(), ux.end()), ux.end());
//	sort(uy.begin(), uy.end());
//	uy.erase(unique(uy.begin(), uy.end()), uy.end());
//
//	int nx = ux.size();
//	int ny = uy.size();
//	vector<vector<bool>> grid(nx, vector<bool>(ny, false));
//
//	for (const auto& r : rects) {
//		int ix1 = lower_bound(ux.begin(), ux.end(), r.x1) - ux.begin();
//		int ix2 = lower_bound(ux.begin(), ux.end(), r.x2) - ux.begin();
//		int iy1 = lower_bound(uy.begin(), uy.end(), r.y1) - uy.begin();
//		int iy2 = lower_bound(uy.begin(), uy.end(), r.y2) - uy.begin();
//		for (int i = ix1; i < ix2; ++i) {
//			for (int j = iy1; j < iy2; ++j) {
//				grid[i][j] = true;
//			}
//		}
//	}
//
//	auto is_covered = [&](int i, int j) {
//		if (i < 0 || i >= nx - 1 || j < 0 || j >= ny - 1) return false;
//		return (bool)grid[i][j];
//		};
//
//	vector<pair<int, int>> vertices;
//	for (int i = 0; i < nx; ++i) {
//		for (int j = 0; j < ny; ++j) {
//			bool v1 = is_covered(i - 1, j - 1);
//			bool v2 = is_covered(i, j - 1);
//			bool v3 = is_covered(i - 1, j);
//			bool v4 = is_covered(i, j);
//
//			int count = (int)v1 + (int)v2 + (int)v3 + (int)v4;
//			if (count == 1 || count == 3) {
//				vertices.push_back({ ux[i], uy[j] });
//			}
//			else if (count == 2 && v1 == v4) {
//				vertices.push_back({ ux[i], uy[j] });
//			}
//		}
//	}
//
//	sort(vertices.begin(), vertices.end());
//	for (const auto& v : vertices) {
//		cout << v.first << " " << v.second << endl;
//	}
//
//	return 0;
//}
#pragma endregion
#pragma region 网易雷火地图收集金币问题
//struct GoldCell {
//	int mapIdx, x, y, coin;
//};
//
//// 怪物格结构
//struct MonsterCell {
//	int mapIdx, x, y, a, b, coin;
//};
//
//// 怪物排序：优先处理奖励高的，其次处理总属性（血量+攻击）较低的
//bool compareMonsters(const MonsterCell& m1, const MonsterCell& m2) {
//	if (m1.coin != m2.coin) return m1.coin > m2.coin;
//	return (m1.a + m1.b) < (m2.a + m2.b);
//}
//
//int main4156436() {
//	// 优化输入输出
//	ios::sync_with_stdio(false);
//	cin.tie(nullptr);
//
//	int A, B; // 初始血量和攻击力
//	if (!(cin >> A >> B)) return 0;
//
//	int n, m; // 金币格数量和怪物数量
//	cin >> n >> m;
//
//	// 按地图编号存储资源
//	vector<vector<GoldCell>> mapsGold(11);
//	for (int i = 0; i < n; ++i) {
//		GoldCell g;
//		cin >> g.mapIdx >> g.x >> g.y >> g.coin;
//		if (g.mapIdx >= 1 && g.mapIdx <= 10) mapsGold[g.mapIdx].push_back(g);
//	}
//
//	vector<vector<MonsterCell>> mapsMonsters(11);
//	for (int i = 0; i < m; ++i) {
//		MonsterCell mon;
//		cin >> mon.mapIdx >> mon.x >> mon.y >> mon.a >> mon.b >> mon.coin;
//		if (mon.mapIdx >= 1 && mon.mapIdx <= 10) mapsMonsters[mon.mapIdx].push_back(mon);
//	}
//
//	int C, D; // 通关奖励和恢复成本
//	cin >> C >> D;
//
//	long long currentGold = 0;
//	int currentHealth = A;
//	bool usedRecovery = false; // 记录是否已使用过唯一一次恢复机会
//
//	for (int i = 1; i <= 10; ++i) {
//		// 1. 优先收集当前地图所有金币格（假设路径可达）
//		for (const auto& g : mapsGold[i]) {
//			currentGold += g.coin;
//		}
//
//		// 2. 对当前地图怪物进行排序
//		sort(mapsMonsters[i].begin(), mapsMonsters[i].end(), compareMonsters);
//
//		bool mapCleared = true;
//		for (const auto& mon : mapsMonsters[i]) {
//			int k = mon.a / B; // 杀死怪物需要的总回合数
//
//			// 计算如果不恢复，小师妹能撑多少回合
//			// 每回合失去 mon.b 血量，第 r 回合后血量为 currentHealth - r * mon.b
//			long long totalDamage = (long long)k * mon.b;
//
//			if (totalDamage < currentHealth) {
//				// 情况A：血量充足，直接击杀
//				currentHealth -= (int)totalDamage;
//				currentGold += mon.coin;
//			}
//			else {
//				// 情况B：血量不足，尝试触发恢复机制
//				if (!usedRecovery) {
//					// 计算在哪一回合血量首次 <= 0
//					int r = currentHealth / mon.b;
//					//int r = (currentHealth + mon.b - 1) / mon.b;
//
//					// 必须在血量归零的瞬间拥有足够金币
//					if (currentGold >= D) {
//						currentGold -= D;
//						usedRecovery = true;
//						int healthAfterRecovery = A / 2; // 恢复到初始一半
//
//						// 战斗继续，还剩 k - r 个回合
//						int remainingRounds = k - r;
//						currentHealth = healthAfterRecovery - remainingRounds * mon.b;
//
//						if (currentHealth > 0) {
//							currentGold += mon.coin;
//						}
//						else {
//							mapCleared = false; // 恢复后依然被打死
//							break;
//						}
//					}
//					else {
//						mapCleared = false; // 没钱恢复
//						break;
//					}
//				}
//				else {
//					mapCleared = false; // 已经用过恢复机会了，无法再次触发
//					break;
//				}
//			}
//		}
//
//		if (!mapCleared) break;
//
//		// 3. 地图通关奖励（血量必须 > 0）
//		if (currentHealth > 0) {
//			currentGold += C;
//		}
//		else {
//			break;
//		}
//	}
//
//	cout << currentGold << endl;
//
//	return 0;
//}
#pragma endregion
