#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include "List.h"
#include "BlockList.h"
#include "String.h"
#include "CssSection.h"

using namespace std;


// reads a string from cin until it reaches one of the endChars
// doesn't remove the endChar from cin
void ReadString(String& str, const String& endChars)
{
	String tmp;
	
	while (cin && !endChars.Contains(cin.peek()))
	{
		char input = getchar();
		if (!cin.eof())
			tmp.Append(input);
	}
	str = tmp;
}



///////////////////////////////////////////////////////////////
// CSS PARSING

void ReadSelectors(CssSection& section)
{
	String str;
	do
	{
		ReadString(str, ",{");
		str.RemoveEdgeWhitespace();

		if (str.GetLength() > 0)
		{
			section.selectors.Append(str);
		}

		if (cin.peek() == ',') // skip the ',' and read the next selector
			getchar();
		if (cin.peek() == '{') // if the next char is '{' then we start reading attributes
		{
			getchar();
			return;
		}

	} while (str.GetLength() > 0);
}

void ReadAttributes(CssSection& section)
{
	String name;
	do
	{
		ReadString(name, ":}");
		name.RemoveEdgeWhitespace();

		if (cin.peek() == ':')
		{
			getchar(); // skip the ':' and read the attribute's value

			Attribute attr;
			attr.name = name;
			ReadString(attr.value, ";}");
			attr.value.RemoveEdgeWhitespace();

			bool attributeRepeated = false;
			for (List<Attribute>::Iterator i = section.attributes.begin(); i != section.attributes.end(); ++i)
			{
				if ((*i).name == name)
				{
					attributeRepeated = true;
					(*i).value = attr.value;
					break;
				}
			}

			if (!attributeRepeated)
				section.attributes.Append(attr);
		}

		if (cin.peek() == ';') // skip the ';' after the attribute if there is one
			getchar();
		if (cin.peek() == '}') // if the next char is '}' then stop reading attributes
		{
			getchar();
			return;
		}

	} while (name.GetLength() > 0);
}

void ReadSection(BlockList<CssSection>& css)
{
	CssSection section;

	ReadSelectors(section);
	ReadAttributes(section);

	css.Append(section);
}



///////////////////////////////////////////////////////////////
// COMMAND PARSING

void SelectorCommands(BlockList<CssSection>& css, const String& arg1, const String& arg3, String& result)
{
	if (arg1.IsInt())
	{
		if (arg3 == "?")
		{
			// [i,S,?] return number of selectors for section 'i'
			size_t i = arg1.ToInt() - 1;
			if (i < css.GetLength())
			{
				result = css[i].selectors.GetLength();
			}
		}
		else
		{
			// [i,S,j] return value of selector 'j' for section 'i'
			size_t i = arg1.ToInt() - 1;
			if (i < css.GetLength())
			{
				size_t j = arg3.ToInt() - 1;
				if (j < css[i].selectors.GetLength())
				{
					result = css[i].selectors[j];
				}
			}
		}
	}
	else
	{
		// [z,S,?] return number of selector 'z' for all sections
		int count = 0;
		for (BlockList<CssSection>::Iterator i = css.begin(); i != css.end(); ++i)
		{
			if ((*i).selectors.Contains(arg1))
			{
				count++;
			}
		}
		result = count;
	}
}

void AttributeCommands(BlockList<CssSection>& css, const String& arg1, const String& arg3, String& result)
{
	if (arg1.IsInt())
	{
		if (arg3 == "?")
		{
			// [i,A,?] return number of attributes for section 'i'
			size_t i = arg1.ToInt() - 1;
			if (i < css.GetLength())
			{
				result = css[i].attributes.GetLength();
			}
		}
		else
		{
			// [i,A,n] return value of attribute called 'n' for section 'i'
			size_t i = arg1.ToInt() - 1;
			if (i < css.GetLength())
			{
				for (List<Attribute>::Iterator j = css[i].attributes.beginReversed(); j != css[i].attributes.end(); --j)
				{
					if ((*j).name == arg3)
					{
						result = (*j).value;
						break;
					}
				}
			}
		}
	}
	else
	{
		// [n,A,?] return number of attributes called 'n' for all sections
		int count = 0;
		for (BlockList<CssSection>::Iterator i = css.begin(); i != css.end(); ++i)
		{
			List<Attribute> attributes = (*i).attributes;
			for (List<Attribute>::Iterator j = attributes.begin(); j != attributes.end(); ++j)
			{
				if ((*j).name == arg1)
				{
					count++;
				}
			}
		}
		result = count;
	}
}

void EvaluateCommand(BlockList<CssSection>& css, const String& arg1, const String& arg3, String& result)
{
	// [z,E,n] return value of attribute called 'n' for selector called 'z'
	for (BlockList<CssSection>::Iterator i = css.beginReversed(); i != css.end(); --i)
	{
		if ((*i).selectors.GetLength() == 0 || (*i).selectors.Contains(arg1))
		{
			for (List<Attribute>::Iterator j = (*i).attributes.beginReversed(); j != (*i).attributes.end(); --j)
			{
				if ((*j).name == arg3)
				{
					result = (*j).value;
					return;
				}
			}
		}
	}
}

void DeleteCommand(BlockList<CssSection>& css, const String& arg1, const String& arg3, String& result)
{
	if (arg3 == "*")
	{
		// [i,D,*] remove section 'i'
		size_t i = arg1.ToInt() - 1;
		if (i < css.GetLength())
		{
			css.RemoveAt(i);
			result = "deleted";
		}
	}
	else
	{
		// [i,D,n] remove attribute called 'n' in section 'i'
		size_t i = arg1.ToInt() - 1;
		if (i < css.GetLength())
		{
			for (List<Attribute>::Iterator j = css[i].attributes.begin(); j != css[i].attributes.end(); ++j)
			{
				if ((*j).name == arg3)
				{
					css[i].attributes.Remove(j.GetCurNode());

					if (css[i].attributes.GetLength() == 0) // if the section has no attributes left, delete it
						css.RemoveAt(i);

					result = "deleted";
					return;
				}
			}
		}
	}
}


void ReadCommands(BlockList<CssSection>& css)
{
	while (cin)
	{
		String arg1 = " ";
		String arg2 = " ";
		String arg3 = " ";

		// read arguments
		ReadString(arg1, ",\n"); // read the 1st argument
		if (cin.peek() == ',')   // check if there is a ',' after it
		{
			getchar();               // skip the ','
			ReadString(arg2, ",\n"); // read the 2nd argument
			getchar();               // skip the ','
			ReadString(arg3, ",\n"); // read the 3rd argument
		}

		if (arg1.GetLength() == 0) // if the 1st argument is empty, it's an empty line
		{
			ReadString(arg1, "\n");
			cin >> ws;
			continue;
		}

		// single argument commands
		if (arg1 == "****")
		{
			// finish reading commands
			return;
		}
		else if (arg1 == "?")
		{
			// print how many sections there are
			cout << arg1 << " == " << css.GetLength() << '\n';
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
				AttributeCommands(css, arg1, arg3, result);
			}
			if (arg2 == "E")
			{
				EvaluateCommand(css, arg1, arg3, result);
			}
			if (arg2 == "D")
			{
				DeleteCommand(css, arg1, arg3, result);
			}

			// print result
			if (result.GetLength() > 0)
				cout << arg1 << "," << arg2 << "," << arg3 << " == " << result << '\n';
		}

		// skip to new line
		cin >> ws;
	}
}



void ReadInput(BlockList<CssSection>& css)
{
	// if the next string starts with a '?', it's the beginning of a command section
	// otherwise, it's a CSS section
	if ((cin >> ws).peek() == '?')
	{
		cin >> ws;
		ReadCommands(css);
	}
	else
	{
		ReadSection(css);
	}
}



int main()
{
	iostream::sync_with_stdio(false);
	cin.tie(nullptr);

	BlockList<CssSection> css;

	while (cin)
	{
		ReadInput(css);
	}

	return 0;
}
