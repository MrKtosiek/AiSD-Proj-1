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
	~List()
	{
		while (first != nullptr)
		{
			Remove(last);
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
	Node<T>* GetNodeAt(size_t index)
	{
		Node<T>* curNode = first;
		while (curNode != nullptr)
		{
			if (index == 0)
				return curNode;

			index--;
			curNode = curNode->next;
		}

		return nullptr;
	}
	void Remove(Node<T>* node)
	{
		if (node == nullptr)
			return;

		if (node->next == nullptr)
			last = node->prev;
		else
			node->next->prev = node->prev;

		if (node->prev == nullptr)
			first = node->next;
		else
			node->prev->next = node->next;

		delete node;
		length--;
	}
	void RemoveAt(size_t index)
	{
		cout << GetNodeAt(index)->data << endl;
		Remove(GetNodeAt(index));
	}

	//T& operator[]()
	//{
	//
	//}

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

