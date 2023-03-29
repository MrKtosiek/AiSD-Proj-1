#pragma once
#include "BlockList.h"

const size_t blockSize = 8;

template <typename T>
class BlockNode
{
private:
	T data[blockSize] = {};
	bool isUsed[blockSize] = {};
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

	T& GetData(size_t index)
	{
		return data[index];
	}
	void SetData(size_t index, const T& value)
	{
		data[index] = value;
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
		size_t cursor = 0;
		for (size_t i = 0; i < blockSize; i++)
		{
			if (!isUsed[i])
				continue;
			cursor++;
		}
		return data[cursor];
	}

	template <typename T>
	friend class BlockList;
};
