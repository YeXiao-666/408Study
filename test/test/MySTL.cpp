#include<iostream>
#include<string>
#include <cassert>
#include <vector>
#include <stack>
#include <algorithm>
#include "RBTree.hpp"
#include <unordered_map>
#include "Utils.hpp"
#include<numeric>
#include <set>
#include <queue>
#include <map>
using namespace std;

/// <summary>
/// Ctrl + M + O  快速折叠region区
/// </summary>
namespace MySTL
{
	typedef long long ll;
	typedef unsigned long long Ull;
	class String
	{
	public:

		typedef char* iterator;
		typedef char* reverse_iterator;
		typedef const char* const_iterator;
		typedef const char* const_reverse_iterator;
		/// <summary>
		/// 构造函数严格按变量声明顺序进行赋值！！！注意与变量顺序一致，否则后果严重！！！
		/// </summary>
		String() : _data(new char[1]), _size(0), _capacity(0)
		{
			cout << "String无参构造" << endl;
			*_data = '\0';
		}

		//strcpy它简单地将源字符串（包括结尾的 \0）拷贝到目标缓冲区，但不检查目标缓冲区是否足够大。
		//如果源字符串长度超过目标缓冲区的容量，就会发生缓冲区溢出，覆盖相邻内存区域，
		// 可能导致程序崩溃、数据损坏，甚至被攻击者利用执行任意代码。
		//由于缺乏边界检查，strcpy 被视为不安全的函数，在许多安全编码标准（如CERT、MISRA）中禁止使用。

	/// <summary>
	/// 
		//strcpy_s显式传入目标缓冲区大小：调用者必须提供目标缓冲区能够容纳的最大字符数（destsz）。
		//运行时检查：函数会检查源字符串长度（包括终止符）是否小于 destsz。如果超出，不会执行拷贝，
		//而是调用运行时约束处理函数（默认行为可能导致程序终止或返回错误码），
		//并将目标缓冲区置为空字符串（或保持未修改状态，取决于实现）。
		//返回错误码：成功返回 0，失败返回非零值，便于调用者处理错误。
		//保证目标缓冲区以空字符结尾：即使拷贝部分字符串（如截断），也会确保目标以 \0 结尾，避免产生非终止字符串
	/// </summary>
	/// <param name="s"></param>
		String(const char* s) : _size(strlen(s)), _capacity(_size), _data(new char[_capacity + 1]) //深拷贝分配新内存、复制字符、释放旧内存，开销较大
		{
			cout << "String常量指针构造" << endl;
			strcpy_s(_data, _size + 1, s);
		}
		String(const String& s) : _size(s._size), _capacity(s._capacity), _data(new char[_capacity + 1])
		{
			cout << "String常量引用构造" << endl;
			strcpy_s(_data, s._size + 1, s.s_data());
		}

		//String(const String& s)
		//	: String(s._data) //委托构造，委托此String(const char* s)构造函数
		//{
		//}
		////左值引用
		//String(char*& s) : _data(s)
		//{
		//	s = NULL;
		//}

		//移动构造函数，右值引用的作用!!!!!
		//&& 标识的参数可以绑定到临时对象（右值）或通过 std::move 转换的对象。
		//它告诉编译器：传入的对象即将被销毁，可以“偷走”其内部资源，而不是费力地拷贝一份。避免深拷贝（开销大），直接占据该块内存

		String(String&& s) noexcept : _size(s._size), _capacity(s._capacity), _data(s._data)
		{
			cout << "String右值引用构造(移动构造)" << endl;
			s._data = nullptr; //防止其调用析构函数释放该块内存，delete[]空指针是安全的
			s._size = s._capacity = 0;
		}
		~String()
		{
			char** ch = &this->_data;
			delete[] _data;
			cout << "String : " << "dataAddress: " << ch << "  已经成功释放内存" << endl;
			_data = nullptr;
			_size = _capacity = 0;
		}
		String substr(size_t begin, size_t Count = 0)const
		{
			String s = "";
			if (begin >= this->_size || begin < 0 || begin + Count >= this->_size)return s;
			if (Count == 0) Count = _size - begin;
			
			s.resize(Count);
			for (int i = begin; i < begin + Count; ++i)
			{
				s[i - begin] = _data[i];
			}
			//return s; //C++将其(局部变量)视为右值(或隐式move), 调用移动构造.
		}
		/* Traditional:
	  String& operator=(const String& rhs)
	  {
		String tmp(rhs);
		swap(tmp);
		return *this;
	  }
	  */

		bool operator<(const String& s)const
		{
			return strcmp(_data, s._data) < 0;
		}

		bool operator==(const String& s)const
		{
			return strcmp(_data, s._data) == 0;
		}

		bool operator>(const String& s)const
		{
			return strcmp(_data, s._data) > 0;
		}

		bool operator<=(const String& s) const
		{
			return *this < s || *this == s;
		}

		bool operator>=(const String& s) const
		{
			return !(*this < s);
		}
		char& operator[](size_t i)
		{
			if (i >= this->_size)
			{
				/*throw exception("out of string range! exception");*/
				throw out_of_range("out of string range! out_of_range");
			}
			return *(this->_data + i);
		}
		//给const String& s 调用下标使用
		const char& operator[](size_t i) const
		{
			if (i >= this->_size)
			{
				/*throw exception("out of string range! exception");*/
				throw out_of_range("out of string range! out_of_range");
			}
			return *(this->_data + i);
		}
		//char operator[](int i)const
		//{
		//	return *(this->_data + i);
		//}

		/// <summary>
		/// 赋值
		/// </summary>
		/// <param name="s"></param>
		/// <returns></returns>
		/// <summary>
		/// //若考虑移动构造的思路，目标对象（*this）已经拥有资源（比如之前分配的内存）。
		// 如果直接接管源对象的指针而不释放自己的旧资源，就会造成内存泄漏。
		//String& operator =(String&& s)noexcept
		//{
		//	delete[] _data;//存在自赋值问题，data变为野指针
		//	_data = s._data;//外部抛出异常时，data或处于资源已释放但未重新获取的无效状态
		//	s._data = NULL;
		//	return *this;
		//}
		String& operator=(const String& s)noexcept
		{
			if (this != &s) //防止自赋值
			{
				cout << "StringConst引用赋值" << endl;
				char* tmp = new char[s._capacity + 1];
				/*strcpy_s(tmp, s._capacity + 1, s._data);*/
				memcpy(tmp, s._data, s._size + 1);
				delete[] _data;
				_data = tmp;
				_size = s._size;
				_capacity = s._capacity;
			}
			return *this;
		}
		/// <summary>
		/// 参数传值，自动调用拷贝构造（或移动构造），若拷贝失败（如内存不足），原对象不变。
		//swap 交换内部指针和大小，通常不抛异常。
		//临时对象 s 析构时会释放原来的资源（交换后持有的旧资源）。
		/// </summary>
		/// <param name="s"></param>
		/// <returns></returns>
		String& operator =(String s) noexcept
		{
			cout << "String值传递赋值" << endl;
			swap(s);
			return *this;
		}
		/// </summary>
		/// <param name="s">
		/// 利用s为右值即将死亡，无需手动delete，s会调用自己的析构delete
		/// </param>
		/// <returns></returns>
		String& operator =(String&& s) noexcept
		{
			cout << "String右值引用赋值（移动赋值）" << endl;
			swap(s);//指针交换无安全问题，且具有强异常安全保证，无论交换成功与否，均不影响安全。
			return *this;
		}

		String& operator +=(const String& s)noexcept
		{
			append(s);
			return *this;
		}
		String& operator +=(char ch)noexcept
		{
			push_back(ch);
			return *this;
		}
		void insert(size_t i, char ch)
		{
			if (_size == _capacity)
			{
				reserve((_capacity == 0) ? 4 : 2 * _capacity);
			}

			size_t end = _size + 1;//这里定义end位于‘\0’之后的一个位置
			while (end > i)
			{
				_data[end] = _data[end - 1];
				--end;
			}
			_data[end] = ch;
			++_size;
		}
		void insert(size_t i, String& s)
		{
			// 1. 越界检查是好习惯
			if (i > _size)
			{
				throw out_of_range("Insert position out of range");
			}
			size_t len = s.size();
			if (len == 0) return; // 插入空字符串，直接返回
			// 2. 科学扩容
			if (len + _size > _capacity)
			{
				// 取“2倍旧容量”和“需要的新容量”中的最大值
				size_t newSize = (_capacity * 2) > (len + _size) ? (_capacity * 2) : (len + _size);
				reserve(newSize);
			}
			// 3. 将插入点 [i, _size] 的字符（包括结尾的 '\0'）往后平移 len 个位置
		// memmove 的参数：目标地址, 源地址, 要移动的字节数
		// 要移动的字节数是 _size - i + 1 (+1 是为了把 '\0' 也一起移走)
			memmove(_data + i + len, _data + i, _size - i + 1);

			// 这里用 memcpy，只拷字符，不拷 '\0'，防止把原字符串截断
			memcpy(_data + i, s._data, len);
			_size += len;
		}

		void swap(String& s) noexcept
		{
			std::swap(this->_data, s._data);
			std::swap(this->_size, s._size);
			std::swap(this->_capacity, s._capacity);
		}
		//typedef unsigned long long size_t
		size_t size() const
		{
			/*return strlen(this->_data);*/
			return this->_size;
		}

		void reserve(size_t n)
		{
			if (_capacity < n)
			{
				char* temp = new char[n + 1];
				strcpy_s(temp, n + 1, _data);
				delete[] _data;
				_data = temp;
				_capacity = n;
			}
		}

		void push_back(char& ch)
		{
			if (_capacity == _size)
			{
				reserve((_capacity == 0) ? 4 : _capacity * 2);
			}
			_data[_size] = ch;
			++_size;
			_data[_size] = '\0';
		}
		void push_back(const String& s)
		{
			cout << "深拷贝" << endl;
			size_t new_size = _size + s.size();
			if (new_size + 1 > _capacity) {          // +1 留给 '\0'
				// 倍增扩容，至少保证能容纳 new_size+1
				size_t new_cap = (_capacity == 0) ? 8 : _capacity * 2;
				while (new_cap < new_size + 1) new_cap *= 2;
				reserve(new_cap);
			}
			// 拷贝字符
			for (size_t i = 0; i < s.size(); ++i)
				_data[_size + i] = s[i];
			_size = new_size;
			_data[_size] = '\0';
		}
		void push_back(String&& s)
		{
			cout << "移动拷贝" << endl;
			size_t new_size = _size + s.size();
			if (new_size + 1 > _capacity) {          // +1 留给 '\0'
				// 倍增扩容，至少保证能容纳 new_size+1
				size_t new_cap = (_capacity == 0) ? 8 : _capacity * 2;
				while (new_cap < new_size + 1) new_cap *= 2;
				reserve(new_cap);
			}
			// 拷贝字符
			for (size_t i = 0; i < s.size(); ++i)
				_data[_size + i] = s[i];
			_size = new_size;
			_data[_size] = '\0';
		}
		void append(const String& s)
		{
			size_t len = s.size();
			size_t newSize = (len + _size) > (_size * 2) ? (len + _size) : (_size * 2);
			if (len + _size >= _capacity)
			{
				reserve(newSize);
			}
			// 4. 修复 strcpy_s！第二个参数是【剩余目标缓冲区的最大容量】
			// 整个申请的内存是 _capacity + 1，已经用掉了 _size，所以剩下 _capacity - _size + 1
			strcpy_s(_data + _size, _capacity - _size + 1, s._data);
			_size += len;
		}
		size_t capacity() const
		{
			return this->_capacity;
		}
		void clear()
		{
			_data[0] = '\0';
			_size = 0;
		}

		size_t find(const char* sub, size_t pos = 0)
		{

		}

		size_t find(char ch, size_t pos = 0)
		{
			assert(pos < _size);
			for (size_t i = pos; i < _size; i++)
			{
				if (_data[i] == ch)
				{
					return i;
				}
			}
			return npos;

		}
		friend ostream& operator<<(ostream& out, const String& s)
		{
			// Now this works because s has begin() and end() methods!
			for (char c : s)
			{
				out << c;
			}
			return out;
		}
		friend istream& operator>>(istream& in, String& s)
		{
			s.clear();
			char ch;
			char buf[129];
			int i = 0;
			ch = in.get();
			while (ch != '\n')
			{
				buf[i++] = ch;
				if (i == 128)
				{
					buf[i] = '\0';
					s += buf;
					i = 0;
				}
				ch = in.get();
			}
			if (i != 0)
			{
				buf[i] = '\0';
				s += buf;
			}
			return in;
		}

		// 场景应用：局部变量的生成与高效返回
		String to_string(int value) {
			bool flag = true;
			if (value < 0) {
				flag = false;
				value = 0 - value;
			}

			String str;
			while (value > 0) {
				int x = value % 10;
				value /= 10;
				str += ('0' + x);
			}
			if (flag == false) str += '-';
			std::reverse(str.begin(), str.end());

			// 机制触发：局部变量 str 作为将亡值返回，外部接收方将直接触发移动构造
			return str;
		}

		iterator erase(iterator it)
		{
			// 1. 越界及合法性检查
			if (_size == 0 || it < begin() || it >= end())
			{
				return end();
			}

			// 2. 从当前位置开始，把后面的字符（包括 '\0'）全部往前挪一位
			iterator current = it;
			while (current < end())
			{
				*current = *(current + 1); // 当 current 是最后一个有效字符时，current+1 正好是 '\0'
				++current;
			}

			// 3. 更新大小
			--_size;

			// 4. 返回值：标准库的 erase 返回的是被删除元素之后的那个元素的迭代器。
			// 因为后面的元素已经前移填补了空缺，此时的 it 刚好就指向了这个新元素。
			return it;
		}
		void erase(size_t pos, size_t len)
		{
			assert(pos < _size);//调试时作断点
			if (pos >= _size) throw out_of_range("visit out of range !");
			if (len < 0)
			{
				cout << "Error : invalid length of String !!" << endl;
			}
			if (len == npos || len + pos > _size)
			{
				_data[pos] = '\0';
				_size = pos;
			}
			else
			{
				size_t begin = pos + len;
				while (begin <= _size)
				{
					_data[begin - len] = _data[begin];
					begin++;
				}
				_size -= len;
			}
		}
		void resize(size_t n, char ch = ' ')
		{
			if (n <= _size)
			{
				_data[n] = '\0';
				_size = n;
			}
			else
			{
				reserve(n);
				while (_size < n)
				{
					_data[_size] = ch;
					++_size;
				}
				_data[_size] = '\0';
			}
		}
		const char* s_data()const
		{
			return this->_data;
		}
		iterator begin() { return _data; }
		iterator rbegin() { return _data + _size - 1; }
		iterator end() { return _data + _size; }
		iterator rend() { return _data - 1; }

		const_iterator begin() const { return _data; }
		const_iterator end()   const { return _data + _size; }
		/// <summary>
		/// 构造函数严格按变量声明顺序进行赋值
		/// </summary>
	private:
		size_t _size = 0;
		size_t _capacity = 0;
		// 2. 最后声明指针，这样它在分配内存时，_capacity 已经是正确的值了
		char* _data = nullptr;// 避免变为野指针, 在移动赋值构造时及时给予指针值，或者置空

		const static size_t npos = -1;
	};


	template<class T>
	class Vector
	{
	public:
		using iterator = T*;
		using const_iterator = const T*;
		void reserve(size_t n)
		{
			size_t cp = capacity();
			size_t sz = size();
			if (cp < n)
			{
				T* temp = new T[n];
				if (_start)
				{
					for (size_t i = 0; i < sz; ++i)
					{
						temp[i] = _start[i];
					}
					delete[] _start;
				}
				_start = temp; // 新空间的起始地址   基址
				_finish = _start + sz; // 
				_endofstorage = _start + n;
			}

		}
		void resize(size_t n, const T& val = T())
		{
			size_t cp = capacity();
			size_t sz = size();
			if (n <= sz)
			{
				_finish = _start + n;
			}
			else
			{
				reserve(n);
				while (_finish < _start + n)
				{
					*_finish = val;
					++_finish;
				}
			}
		}
		Vector() {}

		template <class InputIterator>
		Vector(InputIterator first, InputIterator last)
		{
			while (first != last)
			{
				push_back(*first);
				++first;
			}
		}
		Vector(size_t n, const T& val = T())
		{
			reserve(n);
			for (size_t i = 0; i < n; i++)
			{
				push_back(val);
			}
		}
		//现代写法的拷贝构造，创建要构造的对象，遍历要拷贝对象的值，追加到要构造对象里
		Vector(const Vector<T>& v)
		{
			reserve(v.capacity());
			for (auto& e : v)
			{
				push_back(e);
			}
		}
		void swap(Vector<T>& v)
		{
			std::swap(_start, v._start);
			std::swap(_finish, v.finish);
			std::swap(_endofstorage, v._endofstorage);
		}
		Vector<T>& operator=(Vector<T> tmp)
		{
			swap(tmp);
			return *this;
		}
		~Vector()
		{
			delete[] _start;
			_start = _finish = _endofstorage = nullptr;
		}
		iterator begin()
		{
			return _start;
		}

		iterator end()
		{
			return _finish;
		}
		const_iterator begin() const
		{
			return _start;
		}

		const_iterator end() const
		{
			return _finish;
		}
		size_t capacity()
		{
			return _endofstorage - _start;
		}
		size_t size()
		{
			return _finish - _start;
		}
		T& operator[](size_t pos)
		{
			assert(pos < size());

			return _start[pos];
		}
		const T& operator[](size_t pos) const
		{
			assert(pos < size());
			return _start[pos];
		}
		void insert(iterator pos, const T& x)
		{
			assert(pos >= _start && pos < _finish);
			size_t cp = capacity();
			size_t sz = size();
			if (_finish == _endofstorage)
			{
				size_t len = pos - _start;
				reserve((cp == 0) ? 4 : 2 * cp);
				pos = _start + len;
			}//插入之前检查是否需要扩容
			iterator end = _finish - 1;
			while (end >= pos)//将pos之后的数据向后挪动，并在pos处放x
			{
				*(end + 1) = *end;
				--end;
			}
			*pos = x;
			_finish++;
		}
		iterator erase(iterator pos)
		{
			assert(pos >= _start && pos < _finish);
			iterator it = pos + 1;
			while (it < _finish)//将pos之后的数据移动覆盖pos位置的数据
			{
				*(it - 1) = *it;
				++it;
			}
			--_finish;
			return pos;//为了防止迭代器失效，删除后返回pos之后的数据
		}
		void push_back(const T& x)
		{
			if (_finish == _endofstorage)
			{
				reserve((capacity() == 0) ? 4 : 2 * capacity());

			}//容量满了就扩容，并在尾部插入x
			*_finish = x;
			_finish++;
		}

	private:
		iterator _start = nullptr;
		iterator _finish = nullptr;
		iterator _endofstorage = nullptr;

	};

	template <class T>
	struct List_Node
	{
		List_Node(const T& x = T())
			:_data(x)
			, _next(nullptr)
			, _prev(nullptr)
		{
		}
		T _data;
		List_Node<T>* _prev;
		List_Node<T>* _next;
	};//List中的各个节点

	//这里是分别写了两个类，来实现List的迭代器下面是iterator
	template<class T>
	struct List_Iterator
	{
		typedef List_Node<T> Node;
		typedef List_Iterator<T> self;
		List_Iterator(Node* node)
			:_nodeptr(node)
		{
		}

		self& operator++()
		{
			_nodeptr = _nodeptr->_next;
			return *this;
		}

		self& operator--()
		{
			_nodeptr = _nodeptr->_prev;
			return *this;
		}

		self operator++(int)
		{
			self tmp(*this);
			_nodeptr = _nodeptr->_next;

			return tmp;
		}

		self operator--(int)
		{
			self tmp(*this);
			_nodeptr = _nodeptr->_prev;

			return tmp;
		}

		T& operator*()
		{
			return _nodeptr->_data;
		}

		T* operator->()
		{
			return &_nodeptr->_data;
		}

		bool operator!=(const self& s)
		{
			return _nodeptr != s._nodeptr;
		}

		bool operator==(const self& s)
		{
			return _nodeptr == s._nodeptr;
		}
		Node* _nodeptr;
	};

	//这里是const_iterator
	template<class T>
	struct List_Const_Iterator
	{
		typedef List_Node<T> Node;
		typedef List_Const_Iterator<T> self;
		Node* _nodeptr;

		List_Const_Iterator(Node* node)
			:_nodeptr(node)
		{
		}

		self& operator++()
		{
			_nodeptr = _nodeptr->_next;
			return *this;
		}

		self& operator--()
		{
			_nodeptr = _nodeptr->_prev;
			return *this;
		}

		self operator++(int)
		{
			self tmp(*this);
			_nodeptr = _nodeptr->_next;

			return tmp;
		}

		self operator--(int)
		{
			self tmp(*this);
			_nodeptr = _nodeptr->_prev;

			return tmp;
		}

		// *it = 10
		const T& operator*()
		{
			return _nodeptr->_data;
		}

		// it->a1 = 10
		const T* operator->()
		{
			return &_nodeptr->_data;
		}

		bool operator!=(const self& s)
		{
			return _nodeptr != s._nodeptr;
		}

		bool operator==(const self& s)
		{
			return _nodeptr == s._nodeptr;
		}
	};

	//List类的实现

	template<class T>
	class List
	{
		//typedef List_Node<T> Node;
		using Node = List_Node<T>;
	public:
		//typedef List_Iterator<T> iterator;//对不同的迭代器进行重定义这里是实现方式2
		//typedef List_Const_Iterator<T> const_iterator;
		using iterator = List_Iterator<T>;
		using const_iterator = List_Const_Iterator<T>;


		//typedef List_Iterator<T, T&, T*> iterator;这里是实现方式1
		//typedef List_Iterator<T, const T&, const T*> const_iterator;
		void empty_init()
		{
			_head = new Node;
			_head->_next = _head;
			_head->_prev = _head;

			_size = 0;
		}//当List为空时，由于结构为带头双向循环，可以使用空初始化对其进行空初始化。

		List()//构造函数调用空初始化
		{
			empty_init();
		}

		List(const List<T>& lt)//拷贝构造现代写法
		{
			empty_init();
			for (auto e : lt)
			{
				push_back(e);
			}
		}



		void swap(List<T>& lt)
		{
			std::swap(_head, lt._head);
			std::swap(_size, lt._size);
		}

		// lt3 = lt1
		List<int>& operator=(List<int> lt)//赋值重载现代写法
		{
			swap(lt);

			return *this;
		}

		size_t size()
		{
			return _size;
		}

		iterator begin()
		{
			//return iterator(_head->_next);//这里需要返回一个迭代器类型
			return _head->_next;//不写iterator的原因是，能够进行隐式类型转换。
		}

		iterator end()
		{
			//return iterator(_head->_next);
			return _head;
		}


		const_iterator begin()const
		{
			return _head->_next;
		}


		const_iterator end() const
		{
			return _head;
		}


		iterator insert(iterator pos, const T& x)
		{//链表的插入逻辑定义新节点，找到插入的位置，进行节点之间的连接
			Node* cur = pos._nodeptr;
			Node* prev = cur->_prev;
			Node* newnode = new Node(x);
			prev->_next = newnode;
			newnode->_prev = prev;
			newnode->_next = cur;
			cur->_prev = newnode;
			++_size;
			return iterator(newnode);

		}

		iterator erase(iterator pos)
		{//链表的删除逻辑，找到要删除的位置，释放对应节点，将其前后节点进行连接

			Node* cur = pos._nodeptr;
			Node* prev = cur->_prev;
			Node* next = cur->_next;
			prev->_next = next;
			next->_prev = prev;
			delete cur;
			cur = nullptr;
			--_size;
			return iterator(next);//为了方便遍历返回删除结点的后一个节点
		}
		void clear()
		{//释放全部节点
			iterator it = begin();
			while (it != end())
			{
				it = erase(it);
			}

		}
		~List()
		{
			clear();
			delete _head;
			_head = nullptr;
			_size = 0;
		}
		void push_back(const T& x)
		{
			insert(end(), x);
		}

	private:
		Node* _head;
		int _size;

	};

	//const迭代器遍历不能直接定义const迭代器进行遍历，由于类型匹配的原则，普通List对象调用begin调的
	//是普通的begin，要么const的begin不和普通的begin重名，要么采用下面的方式遍历
		/*template<typename Container>
		void print_container(const Container& con)
		{
			typename Container::const_iterator it = con.begin();
	// 注意：此处typename的作用是明确告诉编译器，Ref是Iterator类中的一个类型，而不是静态成员变
	// 否则编译器编译时就不知道Ref是Iterator中的类型还是静态成员变量
	// 因为静态成员变量也是按照 类名::静态成员变量名 的方式访问的
			while (it != con.end())
			{
				cout << *it << " ";
				++it;
			}
			cout << endl;
		}*/


	template<class T, class Container = deque<T>>
	class Stack
	{
	public:
		void push(const T& x)
		{
			_con.push_back(x);
		}
		void pop()
		{
			_con.pop_back();
		}
		const T& top()
		{
			return _con.back();
		}
		bool empty()
		{
			return _con.empty();
		}
		size_t size()
		{
			return _con.size();
		}
	private:
		Container _con;
	};

	template<class T, class Container = deque<T>>
	class Queue
	{
	public:
		void push(const T& x)
		{
			_con.push_back(x);
		}
		void pop()
		{
			_con.pop_front();
		}
		const T& front()
		{
			return _con.front();
		}
		const T& back()
		{
			return _con.back();
		}
		bool empty()
		{
			return _con.empty();
		}
		size_t size()
		{
			return _con.size();
		}
	private:
		Container _con;
	};

	template<class T>
	class Less
	{
	public:
		bool operator()(const T& x, const T& y)
		{
			return x < y;
		}
	};

	template<class T, class Container = vector<T>, class Compare = Less<T>>
	class priority_queue
	{
	public:
		priority_queue() {}
		priority_queue(const Compare& comp) : _comp(comp) {}
		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator end, const Compare& comp = Compare())
			:_con(first, end), _comp(comp)
		{
			//size_t 是无符号整型（Unsigned Integer）。无符号整型永远 >= 0。
			//当 i 递减到 0 并执行 --i 后，它不会变成 - 1，而是会溢出变成 SIZE_MAX
			// （通常是 18446744073709551615），导致无限循环和数组越界。
			//如果传入的 _con.size() 为 0，0 / 2 - 1 会直接在初始化时触发下溢，程序直接崩溃。
			/*for (size_t i = _con.size() / 2 - 1; i >= 0; --i)
			{
				AdjustDown(i);
			}*/
			for (int i = ((int)_con.size() - 2) / 2; i >= 0; --i) 
			{
				AdjustDown(i);
			}
		}
		void AdjustUp(size_t child)
		{
			int parent = (child - 1) / 2;
			while (child > 0)
			{
				if (_comp(_con[parent], _con[child]))
				{
					swap(_con[child], _con[parent]);
					child = parent;
					parent = (child - 1) / 2;
				}


				/*if (_con[child] > _con[parent])
				{
					swap(_con[child], _con[parent]);
					child = parent;
					parent = (child-1) / 2 ;
				}*/


				else
				{
					break;
				}
			}
		}
		void AdjustDown(size_t parent)
		{
			int child = parent * 2 + 1;
			while (child < _con.size())
			{
				if (child + 1 < _con.size() && _comp(_con[child], _con[child + 1]))
				{
					child++;
				}


				if (_comp(_con[parent], _con[child]))
				{
					swap(_con[child], _con[parent]);
					parent = child;
					child = parent * 2 + 1;
				}


				/*if (_con[child] > _con[parent])
				{
					swap(_con[child], _con[parent]);
					parent = child;
					child = parent * 2 + 1;
				}*/


				else
				{
					break;
				}
			}
		}
		void push(const T& x)
		{
			_con.push_back(x);
			AdjustUp(_con.size() - 1);
		}
		void pop()
		{
			swap(_con[0], _con[_con.size() - 1]);
			_con.pop_back();
			AdjustDown(0);
		}
		const T& top()
		{
			return _con[0];
		}
		bool empty()
		{
			return _con.empty();
		}

		size_t size()
		{
			return _con.size();
		}
	private:
		Container _con;
		Compare _comp;
	};


	/// <summary>
	/// 序列式容器
	/// </summary>
	/// <typeparam name="T1"></typeparam>
	/// <typeparam name="T2"></typeparam>
	

	template<class K>
	class set
	{
		// 仿函数：set 的 value 就是 key，直接返回即可
		struct SetKeyOfT
		{
			const K& operator()(const K& key) const // 加上 const 尾缀更安全
			{
				return key;
			}
		};

	public:
		// set 的普通迭代器和 const 迭代器，全部强转为底层的 const_iterator，防止修改
		typedef typename RBTree<K, K, SetKeyOfT>::const_iterator iterator;
		typedef typename RBTree<K, K, SetKeyOfT>::const_iterator const_iterator;

		iterator begin() const
		{
			return _set.begin();
		}

		iterator end() const
		{
			return _set.end();
		}

		// 插入操作
		pair<iterator, bool> insert(const K& key)
		{
			// _set.Insert 返回的是 pair<RBTree::iterator, bool>
			// 利用我们在迭代器里加的拷贝构造，这里会自动将其转换为 pair<set::iterator, bool>
			auto ret = _set.Insert(key);
			return pair<iterator, bool>(ret.first, ret.second);
		}

		// 查找操作
		iterator find(const K& key) const
		{
			// 假设底层红黑树的 Find 返回 Node*
			auto node = _set.Find(key);
			return iterator(node);
		}

	private:
		RBTree<K, K, SetKeyOfT> _set;
	};

	template<class K, class V>
	class map
	{
		// 仿函数：从 pair 节点中提取出 key 用于树的比较
		struct MapKeyOfT
		{
			const K& operator()(const pair<const K, V>& kv) const // 必须是 const K
			{
				return kv.first;
			}
		};

	public:
		// map 的普通迭代器允许修改 value (second)，所以用底层普通的 iterator
		typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::iterator iterator;
		typedef typename RBTree<K, pair<const K, V>, MapKeyOfT>::const_iterator const_iterator;

		iterator begin()
		{
			return _map.begin();
		}

		iterator end()
		{
			return _map.end();
		}

		// 常量版本的 begin 和 end
		const_iterator begin() const { return _map.begin(); }
		const_iterator end() const { return _map.end(); }

		// 插入操作：参数严格使用 const pair<const K, V>&
		pair<iterator, bool> insert(const pair<const K, V>& kv)
		{
			return _map.Insert(kv);
		}

		// 核心神仙方法：operator[]
		V& operator[](const K& key)
		{
			// 1. 尝试插入一个 pair。如果键存在，插入失败，返回原节点的迭代器。
			// 2. 如果键不存在，插入成功，返回新插入节点（默认构造 V()）的迭代器。
			// 3. 无论成功与否，ret.first 都指向树中键为 key 的那个迭代器。

			// 注意：不要用 make_pair(key, V())，因为它会推导为 pair<K, V> 而非 pair<const K, V>
			pair<iterator, bool> ret = _map.Insert(pair<const K, V>(key, V()));

			// 通过迭代器访问 second (即 V)，并以引用返回
			return ret.first->second;
		}

		// 查找操作
		iterator find(const K& key)
		{
			auto node = _map.Find(key);
			return iterator(node);
		}

	private:
		// 注意中间的模板参数必须是 pair<const K, V>
		RBTree<K, pair<const K, V>, MapKeyOfT> _map;
	};
}

int mainmkjj() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	vector<int>v;
	map<int, int>m;
	priority_queue<int> p;
	set<int>set;
	set.insert(10);
	set.find(1);
	MySTL::priority_queue<int, greater<int>>pq;
	MySTL::map<MySTL::String, MySTL::String> map;
	map.insert(make_pair("11", "11"));
	string s;
	stoi(s);

	//MySTL::String s = "hello World";
	//MySTL::String s1 = "hello World!!!!";
	//cout << s1.substr(1) << endl;
	//cout << s1 << endl;*/
	//s1.push_back(s);
	//s1.push_back(MySTL::String("You Are So beautiful!!"));

	//auto cmp = [](const pair<int, int>& a, const pair<int, int>& b)
	//	{
	//		return a.second > b.second;
	//	};
	//cout <<typeid(cmp).name();
	//vector<int>v = { 1, 1, 1, 2, 2, 3 };
	//print(topKFrequent(v, 2));
	//deque<int> de;
	//set<int, greater<int>>set;
	//auto cmp = [](std::pair<string, int>& a, std::pair<string, int>& b)
	//	{
	//		return a.second > b.second;
	//	};
	//map<string, int> map;
	//
	//
	//map.insert({ "张飞", 58 });
	//map.insert(make_pair("赵云", 70));
	//map.emplace("刘备", 64);
	//map["关于"] = 89;
	//vector<std::pair<string, int>> vec(map.begin(), map.end());
	//sort(vec.begin(), vec.end(), cmp);
	//cout << "vec : -------------" << endl;
	//for (auto& [name, age] : vec)
	//{
	//	cout << name << " " << age << endl;
	//}
	//cout << "map : -------------" << endl;
	//for (auto& [name, age] : map)
	//{
	//	cout << name << " " << age << endl;
	//}
	//cout << "map : -------------" << endl;
	//for (std::pair<const string, int>& pr : map)
	//{
	//	cout << pr.first << " " << pr.second << endl;
	//}
	//vector<int> v{ 10, 10, 20, 30, 20, 10, 80 ,99, 21, 23, 840 };
	//cout << count(v.begin(), v.end(), 10)<<endl;
	//cout << "reverse --------------" << endl;
	//reverse(v.begin(), v.end());
	//print(v);
	//cout << "sort --------------" << endl;
	//sort(v.begin(), v.end());
	//print(v);
	//cout << "sort 降序--------------" << endl;

	///*sort(v.begin(), v.end(), [](int a, int b) {
	//	return a > b;
	//	});*/
	//sort(v.begin(), v.end(), greater<int>());
	//print(v);
	//cout << "maxelement--------------" << endl;
	//cout << *max_element(v.begin(), v.end())<< endl;
	//cout << accumulate(v.begin(), v.end(), 0)<< endl;
	//cout << reduce(v.begin(), v.end())<< endl;
	return 0;
}
	
	/*cout << "m.size()  " << m.size() << endl;
	Ull x = -3;
	cout << " Ull x = " << x << endl;
	try
	{
		cout << s[-1] << endl;
		cout << s[5] << endl;
	}
	catch (const exception& e)
	{
		cout << " overflow error :" << e.what() << endl;
	}
	String s2 = s;
	cout << "s2 = " << s2 << endl;
	String s3(std::move(s2));
	cout << "s3 = "<< s3<< endl;
	cout << "Address of s: " << &s << endl;
	cout << "Address of m: " << &m << endl;
	cout << "Address of s2: " << &s2 << endl;
	cout << "Address of s3: " << &s3 << endl;*/
	//string类是使用char即作为它的字符类型。
	//string类是basic_string模板类的一个实例，它使用char来实例化basic_string模板类，
	// 并用char_traits和allocator作为basic_string的默认参数
	//1. string是表示字符串的字符串类
	//该类的接口与常规容器的接口基本相同，再添加了一些专门用来操作string的常规操作。

	//	3. string在底层实际是：basic_string模板类的别名即使用了typedef：
	//typedef basic_string<char, char_traits, allocator>   string;
	//4. 不能操作多字节或者变长字符的序列，在使用时要包含对应的头文件
	//using string = basic_string<char, char_traits<char>, allocator<char>>;