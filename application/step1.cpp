#include <util/typelist.h>
#include <util/number.h>

#include "step1_params.h"

#include <iostream>

#define PASTE_EXEC(x) #x " = " << x

int main()
{
	std::cout << PASTE_EXEC(NX) << ", " PASTE_EXEC(DX) << '\n';
}
