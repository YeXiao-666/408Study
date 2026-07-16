#pragma once
#include <iostream>
#include <map>

template<class T>
void print(const T& container)
{

    for (auto it = container.begin(); it != container.end(); ++it)
        std::cout << *it << " ";
    std::cout << std::endl;
}
/// <summary>
/// 通过 Container::size_type，我们可以写出不依赖具体容器内部实现细节的可移植代码。
/// 即使不同容器的 size_type 不同（比如一个用 unsigned int，另一个用 unsigned long long），我们的代码也能自动适配。
///  typename 告诉编译器，后面为类型名Container::size_type，而不是静态成员变量
/// </summary>
/// <typeparam name="Container"></typeparam>
/// <param name="c"></param>
/// <param name="k"></param>
template<typename Container>
void printFirstK(const Container& c, typename Container::size_type k) {
    // 使用 Container::size_type 作为遍历的计数类型
    typename Container::size_type i = 0;
    for (auto it = c.begin(); it != c.end() && i < k; ++it, ++i) {
        std::cout << *it << ' ';
    }
}
template<class T1, class T2>
void print(const std::map<T1, T2>& container)
{
    std::cout << "map print : ----------" << std::endl;
    for (auto it : container)
    {
        std::cout << it.first << " " << it.second << std::endl;
    }
}
