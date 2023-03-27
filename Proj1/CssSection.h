#include "List.h"
#include "String.h"

struct Attribute
{
	String property;
	String value;
};

struct CssSection
{
	List<String> selectors;
	List<Attribute> attributes;
};
