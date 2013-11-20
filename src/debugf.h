#pragma once 

#include <iostream>
extern std::ostream std::cerr;
using std::cerr;
#include <stdio.h>

static char *f(const char* format, ...)
{
	static char muna[2048];
	va_list args;
	va_start( args, format );
	vsprintf(muna, format, args );
	va_end( args );
	return muna;
}
