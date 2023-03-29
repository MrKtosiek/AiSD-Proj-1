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
		for (BlockNode<T>* cur = orig.first; cur != nullptr; cur = cur->next)
		{
			Append(cur->data);
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
	void Append(T data)
	{
		if (length == 0)
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
			last->SetNext(newNode);
			last = newNode;
		}

		length++;
	}
	BlockNode<T>* GetNodeAt(size_t pos) const
	{
		if (pos < length * 2)
		{
			// search from the beginning
			for (BlockNode<T>* curNode = first; curNode != nullptr; curNode = curNode->next)
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
			for (BlockNode<T>* curNode = last; curNode != nullptr; curNode = curNode->prev)
			{
				if (pos == 0)
					return curNode;

				pos--;
			}
		}
		return nullptr;
	}
	BlockNode<T>* GetNodeContaining(size_t index) const
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
				return curNode;
			}
		}
		return nullptr;
	}
	BlockNode<T>* GetLastNode() const
	{
		return last;
	}
	BlockNode<T>* GetFirstNode() const
	{
		return first;
	}
	void Remove(BlockNode<T>* node)
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
		for (BlockNode<T>* curNode = first; curNode != nullptr; curNode = curNode->next)
		{
			//cout << curNode << endl;
			if (curNode->GetData() == data)
			{
				//cout << *this << " contains char: " << data << endl;
				return true;
			}
		}
		//cout << *this << " does not contain char: " << data << endl;
		return false;
	}

	T& operator[](size_t index) const
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
				return (*curNode)[index];
			}
		}
	}
	BlockList<T>& operator=(const BlockList<T>& orig)
	{
		BlockList<T> tmp(orig);
		swap(tmp.first, first);
		swap(tmp.last, last);
		swap(tmp.length, length);
		return *this;
	}
	friend ostream& operator<<(ostream& ostr, const BlockList<T>& list)
	{
		for (BlockNode<T>* curNode = list.first; curNode != nullptr; curNode = curNode->GetNext())
		{
			ostr << curNode->GetData();
		}
		return ostr;
	}


	class Iterator
	{
	private:
		size_t curElement;
		BlockNode<T>* curNode;
	public:
		Iterator(size_t curElement, BlockNode<T>* curNode) : curElement(curElement), curNode(curNode) {}

		Iterator& operator=(BlockNode<T>* node)
		{
			this->curNode = node;
			return *this;
		}

		// Prefix ++ overload
		Iterator& operator++()
		{
			curElement++;
			if (curNode)
				curNode = curNode->next;
			return *this;
		}
		// Postfix ++ overload
		Iterator operator++(int)
		{
			Iterator iterator = *this;
			++* this;
			return iterator;
		}
		// Prefix -- overload
		Iterator& operator--()
		{
			if (curNode)
				curNode = curNode->prev;
			return *this;
		}
		// Postfix -- overload
		Iterator operator--(int)
		{
			Iterator iterator = *this;
			--* this;
			return iterator;
		}

		bool operator!=(const Iterator& iterator)
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
	Iterator end()
	{
		return Iterator(0, nullptr);
	}
};
