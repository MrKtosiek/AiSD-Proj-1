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

	// check if there is space at the end of the block
	bool IsFull() const
	{
		return isUsed[blockSize - 1];
	}
	void AddElement(T& element)
	{
		int i;
		for (i = blockSize - 2; i >= 0; i--)
		{
			if (isUsed[i])
			{
				break;
			}
		}

		data[i + 1] = element;
		isUsed[i + 1] = true;
		elementCount++;
	}
	void RemoveElement(size_t index)
	{
		size_t i;
		for (i = 0; i < blockSize; i++)
		{
			if (!isUsed[i])
				continue;
			if (index == 0)
			{
				isUsed[i] = false;
				elementCount--;
				break;
			}
			index--;
		}
	}

	bool Contains(const T& element) const
	{
		for (size_t i = 0; i < elementCount; i++)
		{
			if (isUsed[i] && data[i] == element)
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
		size_t i;
		for (i = 0; i < blockSize - 1; i++)
		{
			if (!isUsed[i])
				continue;
			if (index == 0)
				return data[i];
			index--;
		}
		return data[i];
	}

	template <typename T>
	friend class BlockList;
};
