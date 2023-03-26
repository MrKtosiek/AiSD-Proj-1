#include <iostream>
#include <string.h>
#include "List.h"
#include "String.h"
#include "CssSection.h"

using namespace std;


// reads a string from cin until it reaches one of the endChars
// doesn't remove the endChar from cin
String ReadString(String endChars)
{
	//cout << "reading string" << endl;
	String str;
	char input;
	while (!endChars.Contains(cin.peek()))
	{
		cin >> noskipws >> input;
		str.Append(input);
		//cout << "read char: " << input << endl;
	}

	//cout << "next char: " << (char)cin.peek() << "(" << cin.peek() << ")" << endl;
	return str;
}



///////////////////////////////////////////////////////////////
// CSS PARSING

void ReadSelectors(CssSection& section)
{
	//cout << "reading selectors" << endl;
	String str;
	do
	{
		str = ReadString(",{");
		str.RemoveEdgeWhitespace();

		if (str.GetLength() > 0)
		{
			section.selectors.Append(str);
		}

		if (cin.peek() == ',')
			cin.get();
		if (cin.peek() == '{')
		{
			cin.get();
			return;
		}

	} while (str.GetLength() > 0);
}

void ReadAttributes(CssSection& section)
{
	//cout << "reading attributes" << endl;
	String name;
	do
	{
		name = ReadString(":}");
		name.RemoveEdgeWhitespace();

		if (cin.peek() == ':')
		{
			Attribute attr;
			attr.name = name;
			attr.value = ReadString(";}");
			attr.value.RemoveEdgeWhitespace();

			section.attributes.Append(attr);
		}

		if (cin.peek() == ':')
			cin.get();
		if (cin.peek() == '}')
		{
			cin.get();
			return;
		}

	} while (name.GetLength() > 0);
}

void ReadSection(List<CssSection>& css)
{
	CssSection section;
	
	ReadSelectors(section);
	ReadAttributes(section);

	css.Append(section);
}


///////////////////////////////////////////////////////////////
// COMMAND PARSING

void SelectorCommands(List<CssSection>& css, String arg1, String arg3, String& result)
{
	if (arg3 == "?")
	{
		// return number of selectors for section i
		size_t i = arg1.ToInt();
		if (i < css.GetLength())
		{
			result = css[i - 1].selectors.GetLength();
		}
	}
	else
	{
		// return value of selector j for section i
		size_t i = arg1.ToInt();
		if (i < css.GetLength())
		{
			size_t j = arg3.ToInt();
			if (j < css[i - 1].selectors.GetLength())
			{
				result = css[i - 1].selectors[j - 1];
			}
		}
	}
}

void ReadCommands(List<CssSection>& css)
{
	//cout << "reading commands" << endl;

	String arg1;
	String arg2;
	String arg3;

	while (true)
	{
		// read arguments
		arg1 = ReadString(",\n");
		if (cin.peek() == ',')
		{
			cin.get();
			arg2 = ReadString(",\n");
			cin.get();
			arg3 = ReadString(",\n");
		}

		// single argument commands
		if (arg1 == "****")
		{
			//cout << "finish reading commands" << endl;
			return;
		}
		else if (arg1 == "?")
		{
			cout << arg1 << " == " << css.GetLength() << endl;
		}
		else
		{
			// three argument commands
			String result;
			if (arg2 == "S")
			{
				SelectorCommands(css, arg1, arg3, result);
			}
			if (arg2 == "A")
			{

			}
			if (arg2 == "E")
			{

			}
			if (arg2 == "D")
			{

			}

			// print result
			if (result.GetLength() > 0)
				cout << arg1 << "," << arg2 << "," << arg3 << " == " << result << endl;
		}

		// skip to new line
		ReadString("\n");
		cin.get();
	}
}


void ReadInput(List<CssSection>& css)
{
	if ((cin >> ws).peek() == '?')
	{
		ReadString("\n");
		cin.get();
		ReadCommands(css);
	}
	else
	{
		ReadSection(css);
	}
}



int main()
{
	List<CssSection> css;

	while (cin.peek())
	{
		ReadInput(css);
	}

	return 0;
}
