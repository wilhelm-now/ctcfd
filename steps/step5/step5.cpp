#include "step5_params.h"

#include <util/typelist.h>
#include <util/number.h>

#include <util/for_i.h>
#include <util/conditional.h>

#include <util/valuelist.h>

#include <iostream>

// Setting up initial conditions
// u = 1.0 everywhere except where 0.5 <= (x, y) <= 1.0

struct initial
{
	template<unsigned index_i, unsigned index_j>
	struct func
	{
		typedef NUMBER_MAKE(
			(0.5 <= DX * index_i && DX * index_i <= 1.0) && (0.5 <= DY * index_j && DY * index_j <= 1.0) 
			? 2.0 : 1.0)
			type;
	};
};

template<unsigned>
struct wave2d;

template<>
struct wave2d<0> // save initial conditions
{
	typedef for_ij<NX, NY, initial>::type type;
};


int main()
{
	std::cout << value_printer2d<wave2d<0>::type>() << '\n';
}