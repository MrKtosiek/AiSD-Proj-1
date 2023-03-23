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
		Node<T>* cur = orig.first;
		while (cur != nullptr)
		{
			Append(cur->data);
			cur = cur->next;
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

	void Append(T data)
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
			last->SetNext(newNode);
			last = newNode;
		}

		length++;
	}
	Node<T>* GetNodeAt(size_t pos)
	{
		if (pos < length * 2)
		{
			// search from the beginning
			Node<T>* curNode = first;
			while (curNode != nullptr)
			{
				if (pos == 0)
					return curNode;

				pos--;
				curNode = curNode->next;
			}
		}
		else
		{
			// search from the end
			pos = length - pos;
			Node<T>* curNode = last;
			while (curNode != nullptr)
			{
				if (pos == 0)
					return curNode;

				pos--;
				curNode = curNode->prev;
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
		cout << GetNodeAt(index)->data << endl;
		Remove(GetNodeAt(index));
	}

	bool Contains(T data)
	{
		Node<T>* curNode = first;
		while (curNode != nullptr)
		{
			if (curNode->GetData() == data)
				return true;
		}
		return false;
	}

	T& operator[](size_t index)
	{
		return GetNodeAt(index)->data;
	}
	List<T>& operator=(const List<T>& orig)
	{
		List<T> tmp(orig);
		swap(tmp.first, first);
		swap(tmp.last, last);
		swap(tmp.length, length);
		return *this;
	}
	friend ostream& operator<<(ostream& ostr, const List<T>& list)
	{
		Node<T>* curNode = list.first;
		while (curNode != nullptr)
		{
			ostr << curNode->GetData();
			curNode = curNode->GetNext();
		}
		return ostr;
	}
};

