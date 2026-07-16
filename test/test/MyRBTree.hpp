#pragma once
#include <iostream>
enum Color
{
	Red,
	Black,
};

template<class T>
struct RBTreeNode
{
	RBTreeNode<T>* left;
	RBTreeNode<T>* right;
	RBTreeNode<T>* parent;
	T _data;
	Color _col;
	RBTreeNode(const T& data) : left(nullptr), right(nullptr), parent(nullptr), _data(data), _col(Red) {}
};
namespace MyRBTree
{
	template<class T1, class T2>
	std::pair<T1, T2> make_pair(T1&& t1, T2&& t2)
	{
		return pair<T1, T2>(forward<T1>t1, forward<T2>t2);
	}
}

template<class T, class Ref, class Ptr>
struct RBTree_Iterator
{
	using Node = RBTreeNode<T>;
	using Self = RBTree_Iterator<T, Ref, Ptr>;
public:
	RBTree_Iterator(Node* node, Node* root): _node(node), _root(root){}
	RBTree_Iterator(const RBTree_Iterator<T, T&, T*>& it): _node(it._node), _root(it._root){}

	Ref& operator*()
	{
		return _node->_data;
	}
	Ptr* operator->()
	{
		return &(_node->data);
	}
	bool operator!=(const Self& s)const
	{
		return this->_node != s._node;
	}

	bool operator==(const Self& s)const
	{
		return this->_node == s._node;
	}
	Self& operator++()const
	{
		if (_node->right)
		{
			Node* cur = _node->right;
			while (cur->left)
			{
				cur = cur->left;
			}
			_node = cur;
		}
		else
		{
			Node* cur = _node;
			Node* par = _node->parent;
			while (par && cur == par->right)
			{
				cur = par;
				par = cur->parent;
			}
			_node = par;
		}
		return *this;
	}
	Self& operator--()const
	{
		if (_node == nullptr)
		{
			Node* rightMax = _root;
			while (_root->right)
			{
				_root = _root->right;
			}
			_node = rightMax;
		}
		else if (_node->left)
		{
			Node* cur = _node->left;
			while (cur->right)
			{
				cur = cur->right;
			}
			_node = cur;
		}
		else
		{
			Node* cur = _node;
			Node* par = _node->parent;
			while (par && cur == par->left)
			{
				cur = par;
				par = cur->parent;
			}
			_node = par;
		}
		return *this;
	}
private:
	Node* _node;
	Node* _root;
};