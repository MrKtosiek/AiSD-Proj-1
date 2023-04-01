#pragma once
#include "BlockList.h"

const size_t blockSize = 8;

template <typename T>
class BlockNode
{
private:
	T data[blockSize] = {};
	size_t elementCount = 0;
	BlockNode<T>* prev = nullptr;
	BlockNode<T>* next = nullptr;

public:
	BlockNode<T>(BlockNode<T>* prev, BlockNode<T>* next) : prev(prev), next(next) {}
	BlockNode<T>(const BlockNode<T>& orig) : data(orig.data), prev(orig.prev), next(orig.next) {}

	BlockNode* GetNext() const
	{
		return next;
	}
	void SetNext(BlockNode* value)
	{
		next = value;
	}

	BlockNode* GetPrev() const
	{
		return prev;
	}
	void SetPrev(BlockNode* value)
	{
		prev = value;
	}

	// check if there is space at the end of the block
	bool IsFull() const
	{
		return elementCount >= blockSize;
	}
	void AddElement(T& element)
	{
		data[elementCount] = element;
		elementCount++;
	}
	void RemoveElement(size_t index)
	{
		for (size_t i = index; i < elementCount - 1; i++)
		{
			swap(data[i], data[i + 1]);
		}
		elementCount--;
	}

	bool Contains(const T& element) const
	{
		for (size_t i = 0; i < elementCount; i++)
		{
			if (data[i] == element)
			{
				return true;
			}
		}
		return false;
	}

	BlockNode<T>& operator=(const BlockNode<T>& orig)
	{
		BlockNode<T> tmp(orig);
		swap(tmp.next, next);
		swap(tmp.prev, prev);
		swap(tmp.data, data);
		return *this;
	}
	T& operator[](size_t index)
	{
		return data[index];
	}

	template <typename T>
	friend class BlockList;
};
