#pragma once
#include<iostream>
enum Color
{
	Red,
	Black
};
template< class T>
struct RBTreeNode
{
	RBTreeNode<T>* _left;
	RBTreeNode<T>* _right;
	RBTreeNode<T>* _prev;
	T _data;
	Color _col;


	RBTreeNode(const T& data)
		:_left(nullptr)
		, _right(nullptr)
		, _prev(nullptr)
		, _data(data)
		, _col(Red)//默认插入为红色结点，符合定义
	{
	}
};
namespace MyRBTree
{
	/*参数类型为 T1&& 和 T2&&
		这是万能引用（而非右值引用），可以绑定到左值或右值，并保留实参的值类别。
		返回值类型使用 std::decay  std::decay<T1>为类类型，std::decay<T1>::type为真实类型，如int，bool等，typename 避免编译器识别为静态变量
		std::decay<T>::type 会移除引用、cv 限定符，并将数组 / 函数退化为指针。
		标准库的 std::make_pair 正是这样做的，确保 pair 中存储的是可拷贝 / 移动的纯值类型（例如 const int& 会变成 int）。
		完美转发 std::forward
		将参数以原始的值类别转发给 std::pair 的构造函数，实现最优的拷贝 / 移动。*/
	template<class T1, class T2>
	std::pair<typename std::decay<T1>::type, typename std::decay<T2>::type>
		make_pair(T1&& t1, T2&& t2)
	{
		return std::pair<typename std::decay<T1>::type, typename std::decay<T2>::type>(
			std::forward<T1>(t1),
			std::forward<T2>(t2)
		);
	}
}
//template <class T1, class T2>
//struct pair
//{
//	typedef T1 first_type;
//	typedef T2 second_type;
//	//first_type和second_type分别充当key和value
//	T1 first;
//	T2 second;
//	pair() //无参构造
//		: first(T1()), second(T2())
//	{
//	}
//	pair(const T1& a, const T2& b)//带参构造
//		: first(a), second(b)
//	{
//	}
//};
template<class T, class Ref, class Ptr>
struct RBTree_Iterator
{
	//typedef RBTreeNode<T> Node;//节点类型
	using Node = RBTreeNode<T>;
	//typedef RBTree_Iterator<T, Ref, Ptr> Self;//迭代器类型
	using Self = RBTree_Iterator<T, Ref, Ptr>;
public:
	Ref& operator*()
	{
		return _node->_data;
	}
	Ptr operator->()
	{
		return &(_node->_data);
	}
	bool operator!=(const Self& s) const
	{
		return _node != s._node;
	}

	bool operator==(const Self& s) const
	{
		return _node == s._node;
	}

	Self& operator++()
	{
		if (_node->_right)
		{
			// 下一个就是右子树的最左节点
			Node* cur = _node->_right;
			while (cur->_left)
			{
				cur = cur->_left;
			}

			_node = cur;
		}
		else
		{
			/*对于二叉搜索树（红黑树本质也是 BST）的中序遍历，寻找当前节点的后继遵循：
				（1）情况 A：当前节点有右子树 → 后继为右子树的最左节点（代码已处理）。
				（2）情况 B：当前节点无右子树 → 后继为第一个 “当前节点位于其左子树中” 的祖先节点。
			在 情况 B 中，我们需要沿着父指针向上回溯：
				（1）如果当前节点是父节点的 右孩子，说明父节点及其左子树都已被访问过，需继续向上寻找。
				（2）如果当前节点是父节点的 左孩子，说明父节点尚未被访问，此时父节点即为后继，循环终止。*/

			Node* cur = _node;
			Node* parent = cur->_prev;
			while (parent && cur == parent->_right)
			{
				cur = parent;
				parent = cur->_prev;
			}
			_node = parent;
		}

		return *this;
	}
	//前置--
	Self operator--()
	{

		// 防御式编程：防止 --end() 导致空指针解引用崩溃
		// 注意：如果是 nullptr，这里没法退回最大节点，除非树结构里保存了 root 或者 max 节点
		if (_node == nullptr)
		{
			// 简陋实现下的妥协：直接抛出异常或直接返回
			throw std::out_of_range("Cannot decrement end() iterator without a header node!");
		}

		if (_node->_left) //结点的左子树不为空
		{
			//寻找该结点左子树当中的最右结点
			Node* right = _node->_left;
			while (right->_right)
			{
				right = right->_right;
			}
			_node = right; //--后变为该结点
		}
		else //结点的左子树为空
		{
			//寻找孩子不在父亲左的祖先
			Node* cur = _node;
			Node* parent = cur->_prev;
			while (parent && cur == parent->_left)
			{
				cur = parent;
				parent = cur->_prev;
			}
			_node = parent; //--后变为该结点
		}
		return *this;
	}

	RBTree_Iterator(Node* node, Node* root)
		:_node(node), _root(root)
	{
	}
	// 允许普通迭代器构造为 const 迭代器 (对 map 无感，但对 set 是必需的)
	RBTree_Iterator(const RBTree_Iterator<T, T&, T*>& it)
		: _node(it._node), _root(it._root)
	{
	}
private:
	Node* _node;
	Node* _root; // 【新增】保存树的根节点，赋予迭代器全局视野
};


/// <summary>
/// STL底层实现中，根节点维护一个头结点head
/// head的left 指向红黑树的最左结点
/// head的right 指向红黑树的最右结点
/// head的parent 指向红黑树的根结点
/// 这样极大节省了内存空间，但代码复杂度很高。日常实现不做此要求
/// </summary>
/// <typeparam name="K"></typeparam>
/// <typeparam name="T"></typeparam>
/// <typeparam name="KeyOfT"></typeparam>
template<class K, class T, class KeyOfT>
class RBTree
{

public:
	typedef RBTree_Iterator<T, T&, T*> iterator;
	typedef RBTree_Iterator<T, const T&, const T*> const_iterator;
	typedef RBTreeNode<T> Node;

	iterator begin()
	{
		//寻找最左结点
		Node* left = _root;
		while (left && left->_left)
		{
			left = left->_left;
		}
		//返回最左结点的正向迭代器
		return iterator(left, _root);
	}
	iterator end()
	{
		//返回由nullptr构造得到的正向迭代器（不严谨）
		return iterator(nullptr, _root);
	}

	const_iterator begin() const
	{
		Node* cur = _root;
		while (cur && cur->_left)
		{
			cur = cur->_left;
		}

		return const_iterator(cur, _root);
	}

	const_iterator end() const
	{
		return const_iterator(nullptr, _root);
	}


	// Find / Insert 中的仿函数更优雅的调用方式：
	KeyOfT getKey; // 实例化一次即可
	//增

	//返回pair<iterator, bool>
	std::pair<iterator, bool> Insert(const T& data)
	{
		if (_root == nullptr)
		{
			_root = new Node(data);
			_root->_col = Black;
			return make_pair(iterator(_root, _root), true);
		}
		Node* parent = nullptr;
		Node* cur = _root;
		KeyOfT GetKey;
		while (cur)
		{
			if (GetKey(cur->_data) < GetKey(data))
			{
				parent = cur;
				cur = cur->_right;
			}
			else if (GetKey(cur->_data) > GetKey(data))
			{
				parent = cur;
				cur = cur->_left;
			}
			else
			{
				return make_pair(iterator(cur, _root), false);
			}
		}
		cur = new Node(data);
		Node* newnode = cur;
		cur->_col = Red;
		if (GetKey(parent->_data) < GetKey(data))
		{
			parent->_right = cur;
			cur->_prev = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_prev = parent;
		}
		while (parent && parent->_col == Red)
		{
			Node* uncle = nullptr;
			Node* grandparent = parent->_prev;

			if (parent == grandparent->_left)
			{
				uncle = grandparent->_right;

				//      gR
				//   fB      uB
				// cR
				if (uncle && uncle->_col == Red)
				{
					// 变色
					parent->_col = uncle->_col = Black;
					grandparent->_col = Red;

					// 继续往上更新处理
					cur = grandparent;
					parent = cur->_prev;
				}
				else//uncle不存在或者uncle->_col==Black
				{
					//进行右单旋
					//       gB                  fB
					//   fR     uB           cR       gR 
					// cR                               uB
					if (cur == parent->_left)
					{
						RotateR(grandparent);
						parent->_col = Black;
						grandparent->_col = Red;

					}
					else
					{
						//进行左右双旋
						//       gB                  gB                fB
						//   fR      uB           cR     uB        cR       gR    
						//       cR             fR                             uB
						RotateL(parent);
						RotateR(grandparent);
						grandparent->_col = Red;
						cur->_col = Black;
					}
					break;
				}
			}
			else// parent == grandfather->_right
			{

				uncle = grandparent->_left;
				//      g
				//   u      f
				//            c
				if (uncle && uncle->_col == Red)
				{
					// 变色
					parent->_col = uncle->_col = Black;
					grandparent->_col = Red;
					// 继续往上更新处理
					cur = grandparent;
					parent = cur->_prev;
				}

				else//uncle不存在或者uncle->_col==Black
				{
					if (cur == parent->_right)
					{
						RotateL(grandparent);
						parent->_col = Black;
						grandparent->_col = Red;
					}
					else
					{
						RotateR(parent);
						RotateL(grandparent);
						grandparent->_col = Red;
						cur->_col = Black;
					}
					break;
				}
			}
		}
		_root->_col = Black;
		return make_pair(iterator(newnode, _root), true);
	}

	//删
	bool Erase(const T& data)
	{
		if (_root == nullptr)
		{
			return false;
		}
		Node* delcur = Find(data);

		//按照二叉搜索树的规则进行查找

		if (delcur == nullptr)
		{
			return false;
		}
		else
		{
			Node* delparent = delcur->_prev;
			//del是唯一的根节点
			if ((delcur == _root) && (delcur->_left == nullptr) && (delcur->_right == nullptr))
			{
				delete _root;
				_root = nullptr;
				return true;
			}
			//delcur有两个孩子
			//if (delcur->_left != nullptr && delcur->_right != nullptr)
			//{
			//	//找后序替代删除
			//	Node* minParent = delcur;
			//	Node* minRight = delcur->_right;
			//	while (minRight->_left)
			//	{
			//		minParent = minRight;
			//		minRight = minRight->_left;
			//	}
			//	delcur->_kv.first = minRight->_kv.first; //将待删除结点的key改为minRight的key
			//	delcur->_kv.second = minRight->_kv.second; //将待删除结点的value改为minRight的value
			//	delparent = minParent; //标记实际删除结点的父结点
			//	delcur = minRight; //标记实际删除的结点
			//}

			// 在 Erase 函数中，修复 _kv 不存在的致命错误
			if (delcur->_left != nullptr && delcur->_right != nullptr)
			{
				Node* minParent = delcur;
				Node* minRight = delcur->_right;
				while (minRight->_left)
				{
					minParent = minRight;
					minRight = minRight->_left;
				}

				// 【警告】这里是“值替换”。
				// 前提：传入的 T 必须允许赋值 (例如 pair<K, V> 而非 pair<const K, V>)
				// 工业级代码应在此处做复杂的“指针重接(Relink)”
				delcur->_data = minRight->_data;

				delparent = minParent;
				delcur = minRight;
			}

			//delcur只有一个孩子
			if ((delcur->_left != nullptr && delcur->_right == nullptr) || (delcur->_right != nullptr && delcur->_left == nullptr))
			{
				//只有一个孩子,父亲一定为黑，孩子一定为红
				//1.将红色节点的值赋值给父亲
				//2.将删除节点转换成删除红色叶子节点
				/*Node* child = delcur->_left == nullptr ? delcur->_right : delcur->_left;
				delcur->_kv.first = child->_kv.first;
				delcur->_kv.second = child->_kv.second;
				delparent = delcur;
				delcur = child;*/

				// 单孩子情况同理修复：
				Node* child = delcur->_left == nullptr ? delcur->_right : delcur->_left;
				delcur->_data = child->_data;
			}

			//调整
			if (delcur->_col == Black)
			{
				AdjustRBTreeBalance(delcur, delparent);
			}

			//删除
			Node* parent = delcur->_prev;
			if (delcur == parent->_left)
			{
				parent->_left = nullptr;
			}
			else
			{
				parent->_right = nullptr;
			}
			delete delcur;
			return true;
		}
	}
	//查
	Node* Find(const T& data)
	{
		Node* cur = _root;
		KeyOfT GetKey;
		while (cur)
		{
			if (GetKey(data) < GetKey(cur->_data)) //key值小于该结点的值
			{
				cur = cur->_left; //在该结点的左子树当中查找
			}
			else if (GetKey(data) > GetKey(cur->_data)) //key值大于该结点的值
			{
				cur = cur->_right; //在该结点的右子树当中查找
			}
			else //找到了目标结点
			{
				return cur; //返回该结点
			}
		}
		return nullptr; //查找失败
	}

	RBTree()
		:_root(nullptr)
	{
	}
	RBTree(const RBTree<K, T, KeyOfT>& rbt)
	{
		_root = _Copy(rbt._root, nullptr);
	}
	RBTree<K, T, KeyOfT>& operator=(RBTree<K, T, KeyOfT> rbt)
	{
		swap(_root, rbt._root);
		return *this; //支持连续赋值
	}
	~RBTree()
	{
		_Destroy(_root);
		_root = nullptr;
	}


private:

	//拷贝树
	Node* _Copy(Node* root, Node* parent)
	{
		if (root == nullptr)
		{
			return nullptr;
		}
		Node* copyNode = new Node(root->_data);
		copyNode->_prev = parent;
		copyNode->_left = _Copy(root->_left, copyNode);
		copyNode->_right = _Copy(root->_right, copyNode);
		return copyNode;
	}

	//析构函数子函数
	void _Destroy(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}
		_Destroy(root->_left);
		_Destroy(root->_right);
		delete root;
	}

	void AdjustRBTreeBalance(Node* delcur, Node* delparent)
	{
		while (delparent)
		{
			Node* brother = (delparent->_left == delcur) ? (delparent->_right) : (delparent->_left);

			//兄弟节点为黑色
			if (brother->_col == Black)
			{
				Node* bl = brother->_left;
				Node* br = brother->_right;
				//且在左侧
				if (brother == delparent->_left)
				{

					if ((brother->_left != nullptr) && (brother->_left->_col == Red))
					{
						//右单旋
						brother->_col = delparent->_col;
						delparent->_col = bl->_col = Black;
						RotateR(delparent);
						break;
					}
					//uncle的右节点为红色，且左节点为黑色或者空
					else if (((br != nullptr) && (br->_col == Red)) && ((bl == nullptr) || (bl->_col == Black)))
					{
						brother->_col = Red;
						br->_col = Black;
						RotateL(brother);
					}
					else
					{
						brother->_col = Red;
						if (delparent->_col == Red)
						{
							delparent->_col = Black;
							break;
						}
						else
						{
							delcur = delparent;
							delparent = delcur->_prev;
						}
					}
				}
				else//在右侧
				{

					if ((br != nullptr) && (brother->_right->_col == Red))
					{
						//左单旋
						brother->_col = delparent->_col;//兄弟旋转后取代父亲节点颜色
						delparent->_col = br->_col = Black;//父亲和侄节点变黑
						RotateL(delparent);
						break;
					}
					else if (((bl != nullptr) && (brother->_left->_col == Red)) && ((br == nullptr) || (br->_col == Black)))
					{
						brother->_col = Red;
						bl->_col = Black;
						RotateR(brother);
					}
					else
					{
						brother->_col = Red;
						if (delparent->_col == Red)
						{
							delparent->_col = Black;
							break;
						}
						else
						{
							delcur = delparent;
							delparent = delcur->_prev;
						}
					}
				}
			}
			else//兄弟节点为红色
			{
				if (brother == delparent->_left)//在左侧
				{
					brother->_col = Black;
					delparent->_col = Red;
					RotateR(delparent);
				}
				else//在右侧
				{
					RotateL(delparent);
					brother->_col = Black;
					delparent->_col = Red;
				}
			}
		}

	}

	//左旋
	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		parent->_right = subRL;
		subR->_left = parent;

		Node* parentParent = parent->_prev;

		parent->_prev = subR;
		if (subRL)
			subRL->_prev = parent;

		if (_root == parent)
		{
			_root = subR;
			subR->_prev = nullptr;
		}
		else
		{
			if (parentParent->_left == parent)
			{
				parentParent->_left = subR;
			}
			else
			{
				parentParent->_right = subR;
			}

			subR->_prev = parentParent;
		}
	}

	//右旋
	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		parent->_left = subLR;
		if (subLR)
			subLR->_prev = parent;

		Node* parentParent = parent->_prev;

		subL->_right = parent;
		parent->_prev = subL;

		if (_root == parent)
		{
			_root = subL;
			subL->_prev = nullptr;
		}
		else
		{
			if (parentParent->_left == parent)
			{
				parentParent->_left = subL;
			}
			else
			{
				parentParent->_right = subL;
			}

			subL->_prev = parentParent;
		}
	}

	Node* _root = nullptr;
};