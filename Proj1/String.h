#pragma once
#include "List.h"

class String : public List<char>
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
};
