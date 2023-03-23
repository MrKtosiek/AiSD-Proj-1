#pragma once
#include "List.h"

const size_t blockSize = 8;

template <typename T>
class Node
{
private:
	Node<T>* prev = nullptr;
	Node<T>* next = nullptr;
	T data;

public:
	Node<T>(T data, Node<T>* prev, Node<T>* next)
	{
		this->data = data;
		this->prev = prev;
		this->next = next;
	}
	~Node<T>(){}
	
	Node* GetNext()
	{
		return next;
	}
	void SetNext(Node* value)
	{
		next = value;
	}
	
	Node* GetPrev()
	{
		return prev;
	}
	void SetPrev(Node* value)
	{
		prev = value;
	}

	T GetData()
	{
		return data;
	}
	void SetData(T value)
	{
		data = value;
	}

	template <typename T>
	friend class List;
};
