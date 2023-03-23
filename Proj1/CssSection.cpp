#include "List.h"
#include "String.h"

struct CssSection
{
	List<String> selectors;
	List<String> attributes;
};
