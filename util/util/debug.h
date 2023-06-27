#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#define PASTE_EXEC(x) #x " = " << (x)

template<typename T>
void print_type()
{
#if defined(__GNUC__) || defined(__clang__)
	std::cout << __PRETTY_FUNCTION__ << '\n';
#else
	std::cout << __FUNCSIG__ << '\n';
#endif // _MSVC
}

#endif // !DEBUG_H
