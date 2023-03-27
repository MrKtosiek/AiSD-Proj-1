#include <iostream>
#include <string.h>
#include "List.h"
#include "String.h"
#include "CssSection.h"

using namespace std;


// reads a string from cin until it reaches one of the endChars
// doesn't remove the endChar from cin
void ReadString(String& str, const String& endChars)
{
	//cout << "reading string" << endl;
	String tmp;
	char input;
	while (cin && !endChars.Contains(cin.peek()))
	{
		input = cin.get();
		tmp.Append(input);
		//cout << "read char: " << input << endl;
	}

	//cout << "read string: " << tmp << endl;

	str = tmp;
}



///////////////////////////////////////////////////////////////
// CSS PARSING

void ReadSelectors(CssSection& section)
{
	//cout << "reading selectors" << endl;
	String str;
	do
	{
		ReadString(str, ",{");
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
		ReadString(name, ":}");
		name.RemoveEdgeWhitespace();

		if (cin.peek() == ':')
		{
			cin.get();
			Attribute attr;
			attr.property = name;
			ReadString(attr.value, ";}");
			attr.value.RemoveEdgeWhitespace();

			section.attributes.Append(attr);
		}

		if (cin.peek() == ';')
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

void SelectorCommands(List<CssSection>& css, const String& arg1, const String& arg3, String& result)
{
	if (arg1.IsInt())
	{
		if (arg3 == "?")
		{
			// return number of selectors for section i
			size_t i = arg1.ToInt();
			if (i <= css.GetLength())
			{
				result = css[i - 1].selectors.GetLength();
			}
		}
		else
		{
			// return value of selector j for section i
			size_t i = arg1.ToInt();
			if (i <= css.GetLength())
			{
				size_t j = arg3.ToInt();
				if (j <= css[i - 1].selectors.GetLength())
				{
					result = css[i - 1].selectors[j - 1];
				}
			}
		}
	}
	else
	{
		// return number of selector 'arg1' for all sections
		int count = 0;
		Node<CssSection>* curSec = css.GetFirstNode();
		while (curSec != nullptr)
		{
			if (curSec->GetData().selectors.Contains(arg1))
			{
				count++;
			}
			curSec = curSec->GetNext();
		}
		result = count;
	}
}

void AttributeCommands(List<CssSection>& css, const String& arg1, const String& arg3, String& result)
{
	if (arg1.IsInt())
	{
		if (arg3 == "?")
		{
			// return number of attributes for section i
			size_t i = arg1.ToInt();
			if (i <= css.GetLength())
			{
				result = css[i - 1].attributes.GetLength();
			}
		}
		else
		{
			// return value of attribute 'arg3' for section i
			size_t i = arg1.ToInt();
			if (i <= css.GetLength())
			{
				for (Node<Attribute>* curAttr = css[i-1].attributes.GetLastNode(); curAttr != nullptr; curAttr = curAttr->GetPrev())
				{
					if (curAttr->GetData().property == arg3)
					{
						result = curAttr->GetData().value;
						break;
					}
				}
			}
		}
	}
	else
	{
		// return number of attributes called 'arg1' for all sections
		int count = 0;
		for (Node<CssSection>* curSec = css.GetFirstNode(); curSec != nullptr; curSec = curSec->GetNext())
		{
			for (Node<Attribute>* curAttr = curSec->GetData().attributes.GetFirstNode(); curAttr != nullptr; curAttr = curAttr->GetNext())
			{
				if (curAttr->GetData().property == arg1)
				{
					count++;
				}
			}
		}
		result = count;
	}
}

void EvaluateCommand(List<CssSection>& css, const String& arg1, const String& arg3, String& result)
{
	// return value of attribute called 'arg3' for selector called 'arg1'
	for (Node<CssSection>* curSec = css.GetLastNode(); curSec != nullptr; curSec = curSec->GetPrev())
	{
		if (curSec->GetData().selectors.GetLength() == 0 || curSec->GetData().selectors.Contains(arg1))
		{
			for (Node<Attribute>* curAttr = curSec->GetData().attributes.GetLastNode(); curAttr != nullptr; curAttr = curAttr->GetPrev())
			{
				if (curAttr->GetData().property == arg3)
				{
					result = curAttr->GetData().value;
					return;
				}
			}
		}
	}
}

void DeleteCommand(List<CssSection>& css, const String& arg1, const String& arg3, String& result)
{
	if (arg3 == "*")
	{
		// remove section i
		size_t i = arg1.ToInt();
		if (i <= css.GetLength())
		{
			css.RemoveAt(i - 1);
			result = "deleted";
		}
	}
	else
	{
		// remove attribute called 'arg3' in section i
		size_t i = arg1.ToInt();
		if (i <= css.GetLength())
		{
			for (Node<Attribute>* curAttr = css[i - 1].attributes.GetFirstNode(); curAttr != nullptr; curAttr = curAttr->GetNext())
			{
				if (curAttr->GetData().property == arg3)
				{
					css[i - 1].attributes.Remove(curAttr);

					if (css[i - 1].attributes.GetLength() == 0)
					{
						css.RemoveAt(i - 1);
					}

					result = "deleted";
					return;
				}
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

	while (cin)
	{
		// read arguments
		ReadString(arg1, ",\n");
		if (cin.peek() == ',')
		{
			cin.get();
			ReadString(arg2, ",\n");
			cin.get();
			ReadString(arg3, ",\n");
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
				cout << arg1 << "," << arg2 << "," << arg3 << " == " << result << endl;
		}

		// skip to new line
		cin >> ws;
	}
}


void ReadInput(List<CssSection>& css)
{
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
	List<CssSection> css;

	while (cin)
	{
		ReadInput(css);
	}
	//cout << "test" << endl;
	return 0;
}
