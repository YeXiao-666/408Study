#include <iostream>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <cstdint>
#include <unordered_set>

using namespace std;

// 组件定义（纯数据）
struct Position { float x, y; };
struct Velocity { float vx, vy; };
struct Health { int hp; };
struct DamageTag {};  // 标记组件

// 实体ID类型
using Entity = uint32_t;

// 简单的ECS注册表
class Registry {
private:
    Entity nextEntity = 0;
    unordered_map<Entity, Position> positions;
    unordered_map<Entity, Velocity> velocities;
    unordered_map<Entity, Health> healths;
    unordered_map<Entity, DamageTag> damageTags;

    // 针对每个组件的存在性检查函数
    bool hasPosition(Entity e) const { return positions.find(e) != positions.end(); }
    bool hasVelocity(Entity e) const { return velocities.find(e) != velocities.end(); }
    bool hasHealth(Entity e) const { return healths.find(e) != healths.end(); }
    bool hasDamageTag(Entity e) const { return damageTags.find(e) != damageTags.end(); }

    // 模板特化：根据组件类型调用对应的存在性检查
    template<typename Comp> bool hasComponent(Entity e) const;

    // 针对每个组件的获取函数
    Position& getPosition(Entity e) { return positions.at(e); }
    Velocity& getVelocity(Entity e) { return velocities.at(e); }
    Health& getHealth(Entity e) { return healths.at(e); }
    DamageTag& getDamageTag(Entity e) { return damageTags.at(e); }

    // 模板特化：根据组件类型获取对应引用
    template<typename Comp> Comp& getComponent(Entity e);

    // 递归展开参数包检查所有组件是否存在
    template<typename Comp, typename... Rest>
    bool hasAllComponents(Entity e) const {
        return hasComponent<Comp>(e) && hasAllComponents<Rest...>(e);
    }
    template<typename...>
    bool hasAllComponents(Entity) const { return true; }

public:
    Entity createEntity() {
        return nextEntity++;
    }

    // 通过重载为每个组件类型提供添加函数
    void addComponent(Entity e, Position comp) { positions[e] = comp; }
    void addComponent(Entity e, Velocity comp) { velocities[e] = comp; }
    void addComponent(Entity e, Health comp) { healths[e] = comp; }
    void addComponent(Entity e, DamageTag comp) { damageTags[e] = comp; }

    // 视图：返回所有拥有指定组件列表的实体的组件引用元组
    template<typename... Comps>
    auto view() {
        vector<tuple<Comps&...>> result;
        unordered_set<Entity> entities;

        // 收集所有出现过的实体ID
        for (const auto& entry : positions) entities.insert(entry.first);
        for (const auto& entry : velocities) entities.insert(entry.first);
        for (const auto& entry : healths) entities.insert(entry.first);
        for (const auto& entry : damageTags) entities.insert(entry.first);

        /*for (Entity e : entities) {
            if (hasAllComponents<Comps...>(e)) {
                result.emplace_back(getComponent<Comps>(e)...);
            }
        }*/
        return result;
    }
};

// 模板特化定义：必须放在类外部（或内部inline，但这里放在外部）
template<> inline bool Registry::hasComponent<Position>(Entity e) const { return hasPosition(e); }
template<> inline bool Registry::hasComponent<Velocity>(Entity e) const { return hasVelocity(e); }
template<> inline bool Registry::hasComponent<Health>(Entity e) const { return hasHealth(e); }
template<> inline bool Registry::hasComponent<DamageTag>(Entity e) const { return hasDamageTag(e); }

template<> inline Position& Registry::getComponent<Position>(Entity e) { return getPosition(e); }
template<> inline Velocity& Registry::getComponent<Velocity>(Entity e) { return getVelocity(e); }
template<> inline Health& Registry::getComponent<Health>(Entity e) { return getHealth(e); }
template<> inline DamageTag& Registry::getComponent<DamageTag>(Entity e) { return getDamageTag(e); }

// 移动系统
class MovementSystem {
public:
    void update(Registry& reg) {
        for (auto& tuple : reg.view<Position, Velocity>()) {
            Position& pos = get<0>(tuple);
            Velocity& vel = get<1>(tuple);
            pos.x += vel.vx;
            pos.y += vel.vy;
            cout << "移动实体: 新位置 (" << pos.x << ", " << pos.y << ")" << endl;
        }
    }
};

// 伤害系统
class DamageSystem {
public:
    void update(Registry& reg) {
        for (auto& tuple : reg.view<Health, DamageTag>()) {
            Health& health = get<0>(tuple);
            health.hp -= 10;
            cout << "伤害实体: 剩余生命 " << health.hp << endl;
        }
    }
};

// 辅助打印系统
class PrintSystem {
public:
    void update(Registry& reg) {
        for (auto& tuple : reg.view<Health>()) {
            Health& health = get<0>(tuple);
            cout << "实体生命值: " << health.hp << endl;
        }
    }
};

int main5234324() {
    Registry registry;

    Entity player = registry.createEntity();
    registry.addComponent(player, Position{ 0, 0 });
    registry.addComponent(player, Velocity{ 1, 2 });
    registry.addComponent(player, Health{ 100 });

    Entity enemy = registry.createEntity();
    registry.addComponent(enemy, Position{ 10, 10 });
    registry.addComponent(enemy, Velocity{ -1, -1 });
    registry.addComponent(enemy, Health{ 50 });
    registry.addComponent(enemy, DamageTag{});

    Entity wall = registry.createEntity();
    registry.addComponent(wall, Position{ 5, 5 });

    cout << "=== 初始状态 ===" << endl;
    PrintSystem().update(registry);

    cout << "\n=== 运行移动系统 ===" << endl;
    MovementSystem().update(registry);

    cout << "\n=== 运行伤害系统 ===" << endl;
    DamageSystem().update(registry);

    cout << "\n=== 最终状态 ===" << endl;
    PrintSystem().update(registry);

    return 0;
}