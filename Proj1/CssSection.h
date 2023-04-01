#pragma once
#include "List.h"
#include "String.h"

struct Attribute
{
	String name;
	String value;
};

struct CssSection
{
	List<String> selectors;
	List<Attribute> attributes;
};
