#include <iostream>
#include <unordered_map>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

// 事件名称常量，类似于 C# 的 EventTool
namespace EventTool {
    const std::string PlayerDead = "PlayerDead";
}

class EventManager {
private:
    // 基类接口，用于类型擦除
    struct IEventInfo {
        virtual ~IEventInfo() = default;
    };

    // 无参事件信息
    struct EventInfoVoid : IEventInfo {
        struct Listener {
            int id;
            std::function<void()> callback;
        };
        std::vector<Listener> listeners;
        int nextId = 0;

        int Add(std::function<void()> cb) {
            int id = nextId++;
            listeners.push_back({ id, cb });
            return id;
        }

        bool Remove(int id) {
            auto it = std::find_if(listeners.begin(), listeners.end(),
                [id](const Listener& l) { return l.id == id; });
            if (it != listeners.end()) {
                listeners.erase(it);
                return true;
            }
            return false;
        }

        void Trigger() {
            for (auto& l : listeners) {
                l.callback();
            }
        }
    };

    // 带一个参数的事件信息
    template<typename T>
    struct EventInfoT : IEventInfo {
        struct Listener {
            int id;
            std::function<void(T)> callback;
        };
        std::vector<Listener> listeners;
        int nextId = 0;

        int Add(std::function<void(T)> cb) {
            int id = nextId++;
            listeners.push_back({ id, cb });
            return id;
        }

        bool Remove(int id) {
            auto it = std::find_if(listeners.begin(), listeners.end(),
                [id](const Listener& l) { return l.id == id; });
            if (it != listeners.end()) {
                listeners.erase(it);
                return true;
            }
            return false;
        }

        void Trigger(T arg) {
            for (auto& l : listeners) {
                l.callback(arg);
            }
        }
    };

    std::unordered_map<std::string, std::shared_ptr<IEventInfo>> eventDic;

public:
    // 添加无参事件监听，返回句柄
    int AddEventListener(const std::string& eventName, std::function<void()> action) {
        auto it = eventDic.find(eventName);
        if (it != eventDic.end()) {
            auto ptr = std::dynamic_pointer_cast<EventInfoVoid>(it->second);
            if (ptr) {
                return ptr->Add(action);
            }
            // 类型不匹配，返回 -1 表示失败
            return -1;
        }
        else {
            auto eInfo = std::make_shared<EventInfoVoid>();
            int id = eInfo->Add(action);
            eventDic[eventName] = eInfo;
            return id;
        }
    }

    // 添加带一个参数的事件监听
    template<typename T>
    int AddEventListener(const std::string& eventName, std::function<void(T)> action) {
        auto it = eventDic.find(eventName);
        if (it != eventDic.end()) {
            auto ptr = std::dynamic_pointer_cast<EventInfoT<T>>(it->second);
            if (ptr) {
                return ptr->Add(action);
            }
            return -1;
        }
        else {
            auto eInfo = std::make_shared<EventInfoT<T>>();
            int id = eInfo->Add(action);
            eventDic[eventName] = eInfo;
            return id;
        }
    }

    // 移除无参事件监听（通过句柄）
    bool RemoveEventListener(const std::string& eventName, int listenerId) {
        auto it = eventDic.find(eventName);
        if (it != eventDic.end()) {
            auto ptr = std::dynamic_pointer_cast<EventInfoVoid>(it->second);
            if (ptr) {
                return ptr->Remove(listenerId);
            }
        }
        return false;
    }

    // 移除带参事件监听（通过句柄）
    template<typename T>
    bool RemoveEventListener(const std::string& eventName, int listenerId) {
        auto it = eventDic.find(eventName);
        if (it != eventDic.end()) {
            auto ptr = std::dynamic_pointer_cast<EventInfoT<T>>(it->second);
            if (ptr) {
                return ptr->Remove(listenerId);
            }
        }
        return false;
    }

    // 移除整个事件（所有监听）
    void RemoveEvent(const std::string& eventName) {
        eventDic.erase(eventName);
    }

    // 触发无参事件
    void EventTrigger(const std::string& eventName) {
        auto it = eventDic.find(eventName);
        if (it != eventDic.end()) {
            auto ptr = std::dynamic_pointer_cast<EventInfoVoid>(it->second);
            if (ptr) {
                ptr->Trigger();
            }
        }
    }

    // 触发带参事件
    template<typename T>
    void EventTrigger(const std::string& eventName, T arg) {
        auto it = eventDic.find(eventName);
        if (it != eventDic.end()) {
            auto ptr = std::dynamic_pointer_cast<EventInfoT<T>>(it->second);
            if (ptr) {
                ptr->Trigger(arg);
            }
        }
    }
};

// 测试示例
int main() {
    EventManager em;

    // 添加两个无参监听
    int id1 = em.AddEventListener(EventTool::PlayerDead, []() {
        std::cout << "Player dead 1" << std::endl;
        });
    int id2 = em.AddEventListener(EventTool::PlayerDead, []() {
        std::cout << "Player dead 2" << std::endl;
        });

    std::cout << "Trigger all:" << std::endl;
    em.EventTrigger(EventTool::PlayerDead);

    // 移除第一个监听
    em.RemoveEventListener(EventTool::PlayerDead, id1);
    std::cout << "After remove first:" << std::endl;
    em.EventTrigger(EventTool::PlayerDead);

    // 添加带参数的事件
    int id3 = em.AddEventListener<int>("ScoreChanged", [](int score) {
        std::cout << "Score changed to " << score << std::endl;
        });
    em.EventTrigger<int>("ScoreChanged", 100);

    return 0;
}