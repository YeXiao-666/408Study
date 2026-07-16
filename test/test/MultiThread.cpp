#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <chrono>
#include <type_traits> // 必须引入，用于 std::invoke_result_t
using namespace std;

// ==========================================
// C++17 版本的线程池实现
// ==========================================
class ThreadPool {
public:
    explicit ThreadPool(size_t threads);
    ~ThreadPool();

    // 提交任务的接口：
    // 1. 使用 [[nodiscard]] 警告调用者不要忽略 future 返回值
    // 2. 使用 C++17 的 std::invoke_result_t 替代已弃用的 std::result_of
    template<class F, class... Args>
    [[nodiscard]] auto enqueue(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<F, Args...>>;

private:
    std::vector<std::thread> workers;          // 工作线程 (Thread)
    std::queue<std::function<void()>> tasks;   // 任务队列

    // 管程组件
    std::mutex queue_mutex;                    // 互斥锁 (Mutex)
    std::condition_variable condition;         // 条件变量
    bool stop;                                 // 停止标志
};

// 构造函数
ThreadPool::ThreadPool(size_t threads) : stop(false) {
    for (size_t i = 0; i < threads; ++i) {
        workers.emplace_back([this] {
            for (;;) {
                std::function<void()> task;

                {
                    // C++17 CTAD: 自动推导锁的类型，省略了 <std::mutex>
                    std::unique_lock lock(this->queue_mutex);

                    this->condition.wait(lock, [this] {
                        return this->stop || !this->tasks.empty();
                        });

                    if (this->stop && this->tasks.empty()) {
                        return;
                    }

                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                task();
            }
            });
    }
}

// 析构函数
ThreadPool::~ThreadPool() {
    {
        // C++17: std::scoped_lock 替代 lock_guard，更安全，支持多锁
        // 同样使用了 CTAD 自动推导类型
        std::scoped_lock lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();

    for (std::thread& worker : workers) {
        worker.join();
    }
}

// Enqueue 方法实现
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
-> std::future<std::invoke_result_t<F, Args...>>
{
    // C++17 使用 invoke_result_t
    using return_type = std::invoke_result_t<F, Args...>;

    // 将任务包装为 std::packaged_task
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();

    {
        // C++17: scoped_lock 管理互斥量
        std::scoped_lock lock(queue_mutex);
        if (stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        tasks.emplace([task]() { (*task)(); });
    }

    condition.notify_one();
    return res;
}

// ==========================================
// 测试代码
// ==========================================

int compute_task(int id, int sleep_ms) {
    std::cout << "Task " << id << " running on thread " << std::this_thread::get_id() << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
    return id * id;
}



int main51256()
{


    std::cout << "Main thread ID: " << std::this_thread::get_id() << "\n\n";
    std::cout << "--- C++17 ThreadPool Execution ---\n";
    ThreadPool pool(3);
    std::vector<std::future<int>> pool_results;
    // 提交任务到线程池
    for (int i = 1; i <= 5; ++i) {
        pool_results.emplace_back(pool.enqueue(compute_task, i, 500));
    }

    for (auto& result : pool_results) {
        std::cout << "Pool Task Result: " << result.get() << "\n";
    }

    std::cout << "\n--- C++17 std::async Execution ---\n";

    // 异步实现
    auto async_res = std::async(std::launch::async, compute_task, 10, 500);

    // 同步实现 (延迟执行)
    auto sync_res = std::async(std::launch::deferred, compute_task, 11, 500);

    std::cout << "Async Result: " << async_res.get() << "\n";
    std::cout << "Sync (deferred) Result: " << sync_res.get() << "\n";
    return 0;
}