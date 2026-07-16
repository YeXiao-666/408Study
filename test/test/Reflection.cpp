
#include <tuple>
#include <utility>
#include <iostream>
#include <vector>
#include <any>
#include <functional>
#include <map>
using namespace std;


#pragma region C++反射机制
// 动态类型处理器
class AnyProcessor {
public:
	/// <summary>
	/// std::any是一个类型安全的容器，它可以存储任意类型的单个值。
	//特点： 你可以把它想象成一个更高级、更安全的void* ，或者是C语言中union的现代化替代品。它不仅能存东西，还能记住自己存的是什么。
	//优点 ： （1）安全性：void*不知道自己指向什么，强转错了就会内存崩坏；std::any会检查类型，类型不会报错。
	//（2）生命周期管理：std::any会负责对象的拷贝、移动和解析结构。如果你在any里面存了一个std::vector，
	// 当any推理时，vector内存也被正确释放。
	//常见场景：常用于处理解析 JSON / 配置文件、插件系统或者消息队列等事先不知道具体数据类型的场景。
	//缺点： 注意：std::any会有一定的性能开销（由于动态内存分配和类型检查）。如果你已知可能的类型范围，通常建议优先使用std::variant，它会更快。
	/// </summary>
	/// <param name="data"></param>

	static void Process(const std::any& data) {
		// 利用 type() 获取运行时的类型信息
		if (data.type() == typeid(int)) {
			std::cout << "[通用编程] 处理整数: " << std::any_cast<int>(data) * 2 << "\n";
		}
		else if (data.type() == typeid(std::string)) {
			std::cout << "[通用编程] 处理字符串: " << std::any_cast<std::string>(data) << "\n";
		}
		else {
			std::cout << "[通用编程] 未知类型: " << data.type().name() << "\n";
		}
	}
};

void test_dynamic_typing() {
	std::vector<std::any> mixed_data = { 42, std::string("Hello Reflection"), 3.14 };
	for (const auto& item : mixed_data) {
		AnyProcessor::Process(item);
	}
}

// 基类
class Animal {
public:
	virtual ~Animal() = default;
	virtual void Speak() = 0;
};

// 反射注册中心 (单例)
class MetaRegistry {
public:

	using CreatorFunc = std::function<std::unique_ptr<Animal>()>; // 无参返回值为unique_ptr<Animal>类型
	using MethodFunc = std::function<void(Animal*)>;// 无参返回值为(Animal*)类型

	// 注册类和方法
	static void RegisterClass(const std::string& className, CreatorFunc creator) {
		creators()[className] = creator;
	}

	static void RegisterMethod(const std::string& methodName, MethodFunc method) {
		methods()[methodName] = method;
	}

	// 动态创建对象 (解耦)
	static std::unique_ptr<Animal> Create(const std::string& className) {
		if (creators().count(className)) return creators()[className]();
		return nullptr;
	}

	// 动态调用方法 (动态行为)
	static void Invoke(Animal* obj, const std::string& methodName) {
		if (methods().count(methodName)) methods()[methodName](obj);
		else std::cout << "方法未找到\n";
	}
	/// <summary>
	/// 在前面的“反射与工厂模式”代码中，我们需要在注册表（std::map）里保存各种动物的“创建逻辑”。
	/// 但是，创建逻辑的来源可能是五花八门的：
		//（1）普通的全局函数
		//（2）类的静态方法
		//（3）匿名函数（Lambda 表达式）
		//（4）重载了 operator() 的仿函数对象
		//如果没有 std::function，传统的 C 语言函数指针只能指向第1种和第2种，无法保存带有状态的 Lambda 表达式。
		// std::function 利用了底层的“类型擦除（Type Erasure）”技术，统一了这些不同类型的调用者。
	/// </summary>
private:
	/// <summary>
	/// 不使用静态成员变量的原因：躲避 C++ 中一个臭名昭著的终极暗器：静态初始化顺序灾难（Static Initialization Order Fiasco，简称 SIOF）。
	/// （1）我们用宏 REGISTER_CLASS(Dog) 在全局作用域生成了一个静态对象 Dog_Register_Instance。
	/// 它的构造函数会在 main() 函数之前执行，并且在内部调用注册表的 creators。
	/// （2）这里潜伏着一个致命危机：C++ 标准明确规定，对于不同编译单元（也就是不同的 .cpp 文件）中的全局/静态变量，
	/// 它们的初始化顺序是完全未定义的（Undefined）！
	/// 因此宏定义的变量若先于注册表的创建写入则会引发段错误（Segment Fault）引发程序崩溃！

	/// 解决方法：局部静态变量（Construct On First Use 惯用法） 首次使用时构造，经典设计模式 Meyers' Singleton（Meyers 单例）
	///C++ 对**局部静态变量（Local Static Variables）**的初始化时机有严格的规定：
	/// 局部静态变量只会在程序的执行流第一次经过它的声明处时，才会被初始化。
	/// C++11开始编译器被强制要求保证局部静态变量的初始化是绝对线程安全的（Thread-Safe）。 业界称之为 Magic Statics（魔法静态变量）
	/// 这种写法强制将被动、不可控的初始化顺序，变成了按需、即时的懒加载（Lazy Initialization）
	/// 底层自动加锁，保证单例模式
	/// </summary>
	/// <returns></returns>
	static std::map<std::string, CreatorFunc>& creators() { static std::map<std::string, CreatorFunc> c; return c; }
	static std::map<std::string, MethodFunc>& methods() { static std::map<std::string, MethodFunc> m; return m; }

};

// 具体类：狗
class Dog : public Animal {
public:
	void Speak() override { std::cout << "汪汪汪！\n"; }
	void WagTail() { std::cout << "狗正在摇尾巴~\n"; }
};

//这个宏的具体作用：实现“自动化注册”
//这段代码的终极目标只有一个：让这段注册逻辑在 main() 函数执行之前自动运行！

//在 C++ 中，全局变量和静态（static）变量的构造函数会在程序启动时、进入 main() 函数之前自动执行。
//利用这个特性，我们把注册逻辑塞进一个结构体的构造函数里，然后声明一个全局静态对象。
// 这样，当程序一启动，Dog 就悄无声息地把自己注册到 MetaRegistry 这个字典里了。
// 宏：用于简化注册（见下一节）
#define REGISTER_CLASS(ClassName) \
    struct ClassName##_Register { \
        ClassName##_Register() { \
            MetaRegistry::RegisterClass(#ClassName, []() { return std::make_unique<ClassName>(); }); \
        } \
    } static ClassName##_Register_Instance;  //【核心魔法】在全局作用域实例化一个该结构体的静态对象

REGISTER_CLASS(Dog)

void test_dynamic_behavior() {
	// 1. 解耦与灵活性：通过字符串创建对象，无需包含具体的 Dog 强依赖
	std::string classNameFromConfig = "Dog";
	auto myPet = MetaRegistry::Create(classNameFromConfig);

	if (myPet) {
		myPet->Speak(); // 虚函数多态

		// 2. 动态行为：注册并按字符串调用非虚方法
		//优点： if (auto d = ...) 的精妙之处（带初始化的条件分支）
		//这是 C++ 中非常推荐的优雅写法（Idiom），它把声明、赋值、类型推导和判空四个操作浓缩在了一起：

		//缺点： dynamic_cast 虽然安全，但它在底层的实现原理是遍历类的继承树（通过虚表指针查找 type_info），
		// 这在性能敏感的场景（比如每帧调用上万次）中开销是非常昂贵的。
		//dynamic_cast 的底层依赖于编译器生成的 RTTI（运行时类型信息，通常是 type_info 结构）。当你执行 dynamic_cast<Dog*>(animalPtr) 时，底层大致会发生这些事：
		//（1）取虚表指针（vptr）：顺着指针找到对象的虚函数表（vtable）。
		//（2）拿类型信息（type_info）：从虚表前面的隐藏位置取出类型元数据。
		//（3）遍历继承树（最耗时）：它要在运行时的内存中，顺着继承树一层一层往上找，比对类型的名字（有时候甚至是字符串比较！）来确认当前对象的类型究竟是不是 Dog 或者是 Dog 的派生类。
		//如果继承层级很深，或者每帧要对几万个对象做类型检查，这种查表和字符串比对的开销是灾难性的。
		MetaRegistry::RegisterMethod("WagTail", [](Animal* a) {
			if (auto d = dynamic_cast<Dog*>(a)) d->WagTail();   //dynamic_cast<Dog*>(a) 的作用这就是 C++ RTTI（运行时类型识别）的核心操作，称为向下转型（Downcasting）。
			}); //如果 a 确实指向一个 Dog 对象：它会返回一个有效的 Dog* 指针，否则为nullptr

		std::string actionFromNetwork = "WagTail";
		MetaRegistry::Invoke(myPet.get(), actionFromNetwork);
	}
}
//既然编译器原生的 RTTI 太慢，那我们就自己写一套！

//核心思想是：用极其廉价的“指针比较”或“整数比较”，来替代昂贵的“类型树遍历和字符串比较”，
// 一旦确认安全，直接使用零开销的 static_cast 进行强转。

/// <summary>
/// OOP面向对象的多态机制最终优化：
/// 没有字符串比对：判断类型是否匹配，仅仅是比较两个 const ClassInfo* 指针的内存地址是否相等（current == targetInfo）。
/// 在 CPU 眼里，比较两个 64 位整数（地址）的速度快得令人发指。
///极浅的遍历：游戏引擎的继承树通常是经过设计的，不会深得离谱。顺着 parent 指针往上跳几步就能得出结论。
//拥抱 static_cast：static_cast 在 C++ 中仅仅是在编译期计算一下指针的偏移量（对于多重继承等情况），
//在运行时几乎是零指令开销。我们用自定义的 IsA 充当了“安全卫士”，
//把风险排除了，从而可以大胆放心地使用最快的 static_cast。
/// </summary>
namespace OOPPromote
{
	// （1）类型的“身份证”
	struct ClassInfo {
		const char* className;
		const ClassInfo* parent; // 指向父类的身份证
	};
	//基类与宏定义（植入基因）
	//（2）我们在基类中强制要求每个对象都能交出自己的“身份证”，并且能沿着血脉往上查。
	class Object {
	public:
		virtual ~Object() = default;

		// 获取当前对象的实际身份证 (虚函数多态)
		virtual const ClassInfo* GetClassInfo() const { return &Object::staticClassInfo; }

		// 核心逻辑：顺着族谱查，看自己是不是目标类型
		bool IsA(const ClassInfo* targetInfo) const {
			const ClassInfo* current = GetClassInfo();
			while (current != nullptr) {
				if (current == targetInfo) return true; // 指针比对，极速！
				current = current->parent;              // 找父类
			}
			return false;
		}

		// 基类的身份证：没有父类
		static const ClassInfo staticClassInfo;
	};
	const ClassInfo Object::staticClassInfo = { "Object", nullptr };

	// 宏：一键给派生类植入“身份证”
#define DECLARE_CLASS(ClassName, ParentName) \
public: \
    static const ClassInfo staticClassInfo; \
    virtual const ClassInfo* GetClassInfo() const override { return &ClassName::staticClassInfo; } 

#define IMPLEMENT_CLASS(ClassName, ParentName) \
    const ClassInfo ClassName::staticClassInfo = { #ClassName, &ParentName::staticClassInfo };


//（3）实现 FastCast（见证奇迹的时刻）
//有了上面的基础，我们就可以抛弃 dynamic_cast，写出引擎级别的极速转换模板了。
// 极速安全转换模板
	template <typename To, typename From>
	To* FastCast(From* obj) {
		if (!obj) return nullptr;

		// 1. 编译期推导目标类型的“身份证” (To::staticClassInfo)
		// 2. 运行期极速检查 (obj->IsA)
		// 注意：这里移除了指针的指针等复杂边界处理，仅作核心原理展示
		if (obj->IsA(&std::remove_pointer_t<To>::staticClassInfo)) {
			// 3. 一旦确认安全，直接用无性能损耗的 static_cast 强转！
			return static_cast<To*>(obj);
		}
		return nullptr;
	}

	// 定义 Animal 和 Dog
	class Animal : public Object {
		DECLARE_CLASS(Animal, Object)
	public:
		virtual void Breathe() { std::cout << "呼吸...\n"; }
	};
	IMPLEMENT_CLASS(Animal, Object)

		class Dog : public Animal {
		DECLARE_CLASS(Dog, Animal)
		public:
			void WagTail() { std::cout << "引擎极速摇尾巴！\n"; }
	};
	IMPLEMENT_CLASS(Dog, Animal)

		void test_fast_cast() {
		// 模拟从事件系统拿到了一个基类指针
		Object* unknownObj = new Dog();

		// 原生写法（慢）： auto d = dynamic_cast<Dog*>(unknownObj)
		// 引擎写法（极快）：
		if (auto d = FastCast<Dog>(unknownObj)) {
			d->WagTail();
		}
		else {
			std::cout << "类型转换失败\n";
		}

		delete unknownObj;
	}
}

/// <summary>
/// 现代游戏引擎和高性能计算的真正核心——面向数据设计（Data - Oriented Design, DOD）。
/// 在传统的面向对象（OOP）中，我们习惯于用“继承”来描述世界（比如 Object -> Animal -> Dog）。
/// 但在处理海量对象（比如同屏十万只怪物、子弹、粒子）时，OOP 暴露出两个致命弱点：
///多态的代价：即使我们用上一节的 FastCast 优化了类型转换，虚函数调用依然会打断 CPU 流水线。
///缓存未命中（Cache Miss）：对象的内存是分散在堆（Heap）里的。CPU 每次顺着指针去内存里抓取数据，
/// 就像是在巨大的仓库里东奔西跑，大部分时间都浪费在了“等数据传过来”上。
///为了打破物理极限，现代引擎（如 Unity DOTS、虚幻引擎的 Mass 框架、以及开源界大名鼎鼎的 EnTT）
/// 采用了一种彻底掀桌子的架构：ECS（Entity - Component - System）
/// 
/// ECS 的三大颠覆性概念
///在 ECS 中，没有类，没有继承，没有多态，甚至没有我们常说的“对象”。
///Entity（实体）：它根本不是一个类，它仅仅是一个无符号整数（ID）。比如 Entity 10086。
///Component（组件）：它是纯数据（POD, Plain Old Data），里面只有变量，没有任何函数和逻辑。
///System（系统）：它是纯逻辑的全局函数，里面没有任何状态。它负责遍历包含特定组件的数组，然后狂奔执行计算。
/// </summary>

#pragma endregion

namespace DOD //Data-Oriented Design
{
#pragma region entt开源的ECS架构
	//#include <iostream>
		//#include <vector>
		//	// 假设我们使用开源库 entt
		//#include <entt/entt.hpp>
		//
		//// 1. ECS 的纯数据组件 (依然极简)
		//	struct Position { float x, y; };
		//	struct Velocity { float dx, dy; };
		//
		//	// 2. 传统的 OOP 玩家类 (处理复杂逻辑，如输入、状态机、背包)
		//	class PlayerOop {
		//	public:
		//		PlayerOop(entt::registry& reg) : ecsRegistry(reg) {
		//			// 【关键点】在 OOP 对象内部，持有一个 ECS 实体 ID
		//			ecsId = ecsRegistry.create();
		//			ecsRegistry.emplace<Position>(ecsId, 0.0f, 0.0f);
		//		}
		//
		//		void FireBullet() {
		//			// OOP 负责触发逻辑，但把繁重的物理计算直接扔给 ECS 引擎
		//			auto bulletId = ecsRegistry.create();
		//			ecsRegistry.emplace<Position>(bulletId, GetX(), GetY());
		//			ecsRegistry.emplace<Velocity>(bulletId, 10.0f, 0.0f); // 子弹速度
		//		}
		//
		//		float GetX() const { return ecsRegistry.get<Position>(ecsId).x; }
		//		float GetY() const { return ecsRegistry.get<Position>(ecsId).y; }
		//
		//	private:
		//		entt::registry& ecsRegistry; // 引用全局的 ECS 世界
		//		entt::entity ecsId;          // 玩家在 ECS 世界中的“倒影”
		//	};
		//
		//	// 3. 游戏主循环 (主宰一切的地方)
		//	void GameLoop(entt::registry& registry, PlayerOop& player) {
		//		// 阶段 A：执行少量的 OOP 逻辑 (比如玩家开火)
		//		player.FireBullet();
		//
		//		// 阶段 B：执行纯 ECS 的高性能系统 (毫无多态，纯数组狂飙)
		//		auto view = registry.view<Position, Velocity>();
		//		for (auto entity : view) {
		//			auto& pos = view.get<Position>(entity);
		//			auto& vel = view.get<Velocity>(entity);
		//			pos.x += vel.dx; // 假设 deltaTime 为 1
		//		}
		//
		//		// 阶段 C：渲染 (可以直接从 ECS 拿数据渲染，避开 OOP 的臃肿)
		//		std::cout << "玩家位置: " << player.GetX() << ", " << player.GetY() << "\n";
		//		std::cout << "当前同屏高性能实体数: " << registry.storage<Position>().size() << "\n";
		//	}
		//}
#pragma endregion



// 纯数据组件
	struct Position {
		float x, y;
		// 为了方便看效果，加个打印
		Position(float _x, float _y) : x(_x), y(_y) {
			std::cout << "  -> 初始化 Position(" << x << ", " << y << ")\n";
		}
	};

	struct Velocity {
		float dx, dy;
		Velocity(float _dx, float _dy) : dx(_dx), dy(_dy) {
			std::cout << "  -> 初始化 Velocity(" << dx << ", " << dy << ")\n";
		}
	};

	// 模拟的迷你 ECS 注册表
	class MiniRegistry {
	public:
		// ==========================================
		// 1. 数据如何初始化？ (核心解答)
		// ==========================================
		// 使用可变参模板 (Args...) 接收比如 (10.0f, 20.0f) 这样的参数
		template <typename C, typename... Args>
		void emplace(int entity_id, Args&&... args) {
			// 获取对应类型 C 的专属存储池
			auto& storage = get_storage<C>();
			// 利用 std::forward 完美转发参数，在 map 内部直接原地调用组件的构造函数！
			storage.emplace(
				std::piecewise_construct,
				std::forward_as_tuple(entity_id),
				std::forward_as_tuple(std::forward<Args>(args)...)
			);

			// 记录一下这个实体，用于后续遍历
			if (std::find(active_entities.begin(), active_entities.end(), entity_id) == active_entities.end()) {
				active_entities.push_back(entity_id);
			}
		}

		// ==========================================
		// 2. 数据如何获取？ (替换掉 static dummy)
		// ==========================================
		template <typename C>
		C& get_component(int entity_id) {
			auto& storage = get_storage<C>();
			// 从真实的存储池中，根据实体 ID 把它的专属数据拿出来
			return storage.at(entity_id);
		}

		// ==========================================
		// 3. 遍历执行 (View 与 each)
		// ==========================================
		template <typename... Components, typename Func>
		void view_each(Func&& func) {
			// 这里简单遍历所有实体（真实的 ECS 会先做严格的组件交集筛选）
			for (int entity : active_entities) {
				// 参数包展开：把真实的组件引用喂给 Lambda！
				func(get_component<Components>(entity)...);
			}
		}

	private:
		std::vector<int> active_entities;

		// 一个极其粗暴的黑科技：用局部静态变量为每种组件类型生成一个独立的 map 存储池
		// 注意：这里的 static 是修饰整个 map 的，而不是单个组件！
		// 意味着所有 Position 存在一个 map 里，所有 Velocity 存在另一个 map 里。
		template <typename C>
		std::unordered_map<int, C>& get_storage() {
			static std::unordered_map<int, C> storage;
			return storage;
		}
	};

	// ==========================================
	// 业务测试代码
	// ==========================================
	void test_real_ecs_data()
	{
		MiniRegistry registry;

		std::cout << "--- 开始初始化实体 1 (狗) ---\n";
		// 这里的 (10.0f, 20.0f) 会被 emplace 完美转发给 Position 的构造函数
		registry.emplace<Position>(1, 10.0f, 20.0f);
		registry.emplace<Velocity>(1, 2.0f, 0.0f);

		std::cout << "\n--- 开始初始化实体 2 (鸟) ---\n";
		registry.emplace<Position>(2, 50.0f, 100.0f);
		registry.emplace<Velocity>(2, 0.0f, 5.0f);

		std::cout << "\n--- 执行系统逻辑 (each) ---\n";
		// 纯爽快的业务逻辑
		registry.view_each<Position, Velocity>([](Position& pos, Velocity& vel) {
			// 现在拿到的 pos 和 vel，是实打实的、每个实体专属的数据！
			pos.x += vel.dx;
			pos.y += vel.dy;
			std::cout << "实体移动后坐标: (" << pos.x << ", " << pos.y << ")\n";
			});
	}
}

#pragma endregion

int main51243123()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);


	test_dynamic_typing();
	//test_dynamic_behavior();
	//DOD::test_real_ecs_data();
	return 0;
}