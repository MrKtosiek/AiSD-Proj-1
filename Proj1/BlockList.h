#pragma once
#include <iostream>
#include "BlockNode.h"

using namespace std;

template <typename T>
class BlockList
{
protected:
	size_t length = 0;
	size_t blockCount = 0;
	BlockNode<T>* first = nullptr;
	BlockNode<T>* last = nullptr;


public:
	BlockList()
	{
	}
	BlockList(const BlockList<T>& orig)
	{
		length = orig.length;
		for (BlockNode<T>* cur = orig.first; cur != nullptr; cur = cur->next)
		{
			AddNode();
			*last = *cur;
		}
	}
	~BlockList()
	{
		BlockNode<T>* curNode = first;
		while (curNode != nullptr)
		{
			BlockNode<T>* tmp = curNode;
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
			AddNode();
		}

		if (last->IsFull())
		{
			AddNode();
		}

		last->AddElement(data);
		length++;
	}
	void AddNode()
	{
		if (blockCount == 0)
		{
			// create the first node
			BlockNode<T>* newNode = new BlockNode<T>(nullptr, nullptr);
			first = newNode;
			last = newNode;
		}
		else
		{
			// add a new node
			BlockNode<T>* newNode = new BlockNode<T>(last, nullptr);
			last->next = newNode;
			last = newNode;
		}

		blockCount++;
	}

	// returns the found node and replaces the input index with the index of the element in the node
	BlockNode<T>* GetNodeContaining(size_t& index) const
	{
		BlockNode<T>* curNode = first;
		while (curNode != nullptr)
		{
			if (index >= curNode->elementCount)
			{
				index -= curNode->elementCount;
				curNode = curNode->next;
			}
			else
			{
				//index--;
				return curNode;
			}
		}
		return nullptr;
	}
	
	void RemoveNode(BlockNode<T>* node)
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
		blockCount--;
	}
	void RemoveAt(size_t index)
	{
		BlockNode<T>* node = GetNodeContaining(index);
		if (node)
		{
			node->RemoveElement(index);
			length--;

			if (node->elementCount == 0)
			{
				RemoveNode(node);
			}
		}
	}
	bool Contains(const T& data) const
	{
		for (BlockNode<T>* curNode = first; curNode != nullptr; curNode = curNode->next)
		{
			if (curNode->Contains(data))
			{
				return true;
			}
		}
		return false;
	}

	T& operator[](size_t index) const
	{
		BlockNode<T>* node = GetNodeContaining(index);
		return (*node)[index];
	}
	BlockList<T>& operator=(const BlockList<T>& orig)
	{
		BlockList<T> tmp(orig);
		length = tmp.length;
		blockCount = tmp.blockCount;

		BlockNode<T>* t = tmp.first;
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
		size_t curElement;
		BlockNode<T>* curNode;
	public:
		Iterator(size_t curElement, BlockNode<T>* curNode) : curElement(curElement), curNode(curNode) {}

		BlockNode<T>* GetCurNode() const
		{
			return curNode;
		}

		Iterator& operator++()
		{
			curElement++;
			if (curElement >= curNode->elementCount)
			{
				curNode = curNode->next;
				curElement = 0;
			}
			return *this;
		}
		Iterator& operator--()
		{
			if (curElement == 0)
			{
				curNode = curNode->prev;
				if (curNode)
					curElement = curNode->elementCount - 1;
			}
			else
			{
				curElement--;
			}
			return *this;
		}

		bool operator!=(const Iterator& iterator) const
		{
			return curNode != iterator.curNode;
		}

		T& operator*()
		{
			return (*curNode)[curElement];
		}
	};
	Iterator begin()
	{
		return Iterator(0, first);
	}
	Iterator beginReversed()
	{
		return Iterator(last->elementCount - 1, last);
	}
	static Iterator end()
	{
		return Iterator(0, nullptr);
	}
};
