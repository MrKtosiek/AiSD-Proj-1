#pragma once
#include "List.h"

class String : public List<char>
{
public:

	String()
	{
	}
	String(const char* str)
	{
		for (int i = 0; str[i] != 0; i++)
		{
			Append(str[i]);
		}
	}

	// removes leading and trailing whitespace
	void RemoveEdgeWhitespace()
	{
		// remove leading whitespace
		while (first != nullptr && isspace(first->GetData()))
		{
			Remove(first);
		}

		// remove trailing whitespace
		while (last != nullptr && isspace(last->GetData()))
		{
			Remove(last);
		}
	}

	bool IsInt()
	{
		Node<char>* curNode = first;
		while (curNode != nullptr)
		{
			if (curNode->GetData() <= '0' || curNode->GetData() >= '9')
			{
				return false;
			}
		}

		return true;
	}
	int ToInt()
	{
		int value = 0;
		Node<char>* curNode = first;
		while (curNode != nullptr)
		{
			value *= 10;
			char digit = curNode->GetData();
			if (digit >= '0' && digit <= '9')
			{
				value += digit - '0';
			}
			curNode = curNode->GetNext();
		}
		return value;
	}

	bool operator==(const String& other)
	{
		if (length != other.length)
			return false;

		Node<char>* nodeL = first;
		Node<char>* nodeR = other.first;

		while (nodeL != nullptr && nodeR != nullptr)
		{
			if (nodeL->GetData() != nodeR->GetData())
			{
				return false;
			}
			nodeL = nodeL->GetNext();
			nodeR = nodeR->GetNext();
		}

		return true;
	}
	String& operator=(int orig)
	{
		do
		{
			Push((orig % 10) + '0');
			orig /= 10;

		} while (orig != 0);

		return *this;
	}
};
