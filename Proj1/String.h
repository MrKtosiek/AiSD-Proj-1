#pragma once

class String
{
private:
	size_t length = 0;
	size_t capacity = 1;
	char* chars = nullptr;

public:

	String()
	{
		chars = new char[capacity];
		chars[0] = 0;
	}
	String(const String& orig)
	{
		length = orig.length;
		capacity = orig.capacity;
		chars = new char[orig.capacity]();
		for (size_t i = 0; i < capacity; i++)
		{
			chars[i] = orig.chars[i];
		}
	}
	String(const char* str)
	{
		capacity = strlen(str) + 1;
		chars = new char[capacity]();
		for (int i = 0; str[i] != 0; i++)
		{
			Append(str[i]);
		}
	}
	~String()
	{
		if(chars != nullptr)
			delete[] chars;
	}

	size_t GetLength() const
	{
		return length;
	}

	void Append(const char& c)
	{
		length++;

		if (length >= capacity)
		{
			capacity *= 2;
			char* tmp = new char[capacity]();

			for (size_t i = 0; i < length; i++)
			{
				tmp[i] = chars[i];
			}

			delete[] chars;
			chars = tmp;
		}

		chars[length - 1] = c;
	}
	bool Contains(const char& c) const
	{
		for (size_t i = 0; i < length; i++)
		{
			if (chars[i] == c)
			{
				return true;
			}
		}
		return false;
	}

	// removes leading and trailing whitespace
	void RemoveEdgeWhitespace()
	{
		if (length == 0)
		{
			return;
		}

		size_t start = 0;
		size_t end = length - 1;

		// count leading whitespace
		while ((isspace(chars[start]) || chars[start] < ' ') && start < length)
		{
			start++;
		}
		// count trailing whitespace
		while ((isspace(chars[end]) || chars[end] < ' ') && end > 0)
		{
			end--;
		}

		// copy the selected chars to a new array
		length = end - start + 1;
		capacity = length + 1;
		char* tmpChars = new char[capacity]();
		for (size_t i = start; i <= end; i++)
		{
			tmpChars[i - start] = chars[i];
		}

		delete[] chars;
		chars = tmpChars;
	}

	bool IsInt() const
	{
		for (size_t i = 0; i < length; i++)
		{
			if (chars[i] < '0' || chars[i] > '9')
			{
				return false;
			}
		}

		return true;
	}
	int ToInt() const
	{
		int value = 0;
		for (size_t i = 0; i < length; i++)
		{
			value *= 10;
			char digit = chars[i];
			if (digit >= '0' && digit <= '9')
			{
				value += digit - '0';
			}
		}
		return value;
	}
	char* GetChars() const
	{
		return chars;
	}

	bool operator==(const String& other) const
	{
		if (length != other.length)
			return false;

		for (size_t i = 0; i < length; i++)
		{
			if (chars[i] != other.chars[i])
			{
				return false;
			}
		}

		return true;
	}
	String& operator=(const int orig)
	{
		int value = orig;
		do
		{
			Append((value % 10) + '0');
			value /= 10;

		} while (value != 0);

		// flip the string
		for (size_t i = 0; i < length / 2; i++)
		{
			char tmp = chars[i];
			chars[i] = chars[length - i - 1];
			chars[length - i - 1] = tmp;
		}

		return *this;
	}
	String& operator=(const String& orig)
	{
		String tmp(orig);
		length = tmp.length;
		capacity = tmp.capacity;
		char* t = tmp.chars;
		tmp.chars = chars;
		chars = t;
		return *this;
	}
	friend ostream& operator<<(ostream& ostr, const String& str)
	{
		ostr << str.chars;
		return ostr;
	}
};
