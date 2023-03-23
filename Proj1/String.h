#pragma once
#include "List.h"

class String : List<char>
{
public:

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


	friend ostream& operator<<(ostream& ostr, const String& string)
	{
		Node<char>* curNode = string.first;
		while (curNode != nullptr)
		{
			ostr << curNode->GetData();
			curNode = curNode->GetNext();
		}
		return ostr;
	}
};
