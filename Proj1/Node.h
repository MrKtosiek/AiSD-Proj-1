#pragma once
#include "List.h"

const size_t blockSize = 8;

template <typename T>
class Node
{
private:
	T data;
	Node<T>* prev = nullptr;
	Node<T>* next = nullptr;

public:
	Node<T>(const T& data, Node<T>* prev, Node<T>* next)
		:data(data), prev(prev), next(next) {}
	Node<T>(const Node<T>& orig)
	{
		next = orig.next;
		prev = orig.prev;
		data = orig.data;
	}

	Node* GetNext() const
	{
		return next;
	}
	void SetNext(Node* value)
	{
		next = value;
	}
	
	Node* GetPrev() const
	{
		return prev;
	}
	void SetPrev(Node* value)
	{
		prev = value;
	}

	T& GetData()
	{
		return data;
	}
	void SetData(const T& value)
	{
		data = value;
	}

	Node<T>& operator=(const Node<T>& orig)
	{
		Node<T> tmp(orig);
		swap(tmp.next, next);
		swap(tmp.prev, prev);
		swap(tmp.data, data);
		return *this;
	}

	template <typename T>
	friend class List;
};
