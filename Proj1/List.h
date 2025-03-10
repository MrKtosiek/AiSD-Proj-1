#pragma once
#include <iostream>
#include "Node.h"

using namespace std;

template <typename T>
class List
{
protected:
	size_t length = 0;
	Node<T>* first = nullptr;
	Node<T>* last = nullptr;


public:
	List()
	{
	}
	List(const List<T>& orig)
	{
		for (Node<T>* cur = orig.first; cur != nullptr; cur = cur->next)
		{
			Append(cur->data);
		}
	}
	~List()
	{
		Node<T>* curNode = first;
		while (curNode != nullptr)
		{
			Node<T>* tmp = curNode;
			curNode = curNode->next;
			delete tmp;
		}
	}

	size_t GetLength() const
	{
		return length;
	}
	void Append(T& data)
	{
		if (length == 0)
		{
			// create the first node
			Node<T>* newNode = new Node<T>(data, nullptr, nullptr);
			first = newNode;
			last = newNode;
		}
		else
		{
			// add a new node
			Node<T>* newNode = new Node<T>(data, last, nullptr);
			last->next = newNode;
			last = newNode;
		}

		length++;
	}
	Node<T>* GetNodeAt(size_t pos) const
	{
		if (pos < length * 2)
		{
			// search from the beginning
			for (Node<T>* curNode = first; curNode != nullptr; curNode = curNode->next)
			{
				if (pos == 0)
					return curNode;

				pos--;
			}
		}
		else
		{
			// search from the end
			pos = length - pos;
			for (Node<T>* curNode = last; curNode != nullptr; curNode = curNode->prev)
			{
				if (pos == 0)
					return curNode;

				pos--;
			}
		}
		return nullptr;
	}
	void Remove(Node<T>* node)
	{
		if (node == nullptr)
			return;


		if (node->prev == nullptr)
		{
			if (node->next == nullptr)
			{
				first = nullptr;
				last = nullptr;
			}
			else
			{
				first = node->next;
				node->next->prev = nullptr;
			}
		}
		else
		{
			if (node->next == nullptr)
			{
				last = node->prev;
				node->prev->next = nullptr;
			}
			else
			{
				node->next->prev = node->prev;
				node->prev->next = node->next;
			}
		}
		
		delete node;
		length--;
	}
	void RemoveAt(size_t index)
	{
		Remove(GetNodeAt(index));
	}

	bool Contains(const T& data) const
	{
		for (Node<T>* curNode = first; curNode != nullptr; curNode = curNode->next)
		{
			if (curNode->data == data)
			{
				return true;
			}
		}
		return false;
	}

	T& operator[](size_t index) const
	{
		return GetNodeAt(index)->data;
	}
	List<T>& operator=(const List<T>& orig)
	{
		List<T> tmp(orig);
		length = tmp.length;

		Node<T>* t = tmp.first;
		tmp.first = first;
		first = t;

		t = tmp.last;
		tmp.last = last;
		last = t;

		return *this;
	}

	
	class Iterator
	{
	private:
		Node<T>* curNode;
	public:
		explicit Iterator(Node<T>* curNode) : curNode(curNode) {}

		Node<T>* GetCurNode() const
		{
			return curNode;
		}

		Iterator& operator++()
		{
			if (curNode)
				curNode = curNode->next;
			return *this;
		}
		Iterator& operator--()
		{
			if (curNode)
				curNode = curNode->prev;
			return *this;
		}

		bool operator!=(const Iterator& iterator) const
		{
			return curNode != iterator.curNode;
		}

		T& operator*()
		{
			return curNode->data;
		}
	};
	Iterator begin()
	{
		return Iterator(first);
	}
	Iterator beginReversed()
	{
		return Iterator(last);
	}
	static Iterator end()
	{
		return Iterator(nullptr);
	}
};
