#include "step11_params.h"
#include "poisson.h"


#include <util/for_i.h>
#include <util/number.h>
#include <util/zeros.h>
#include <util/conditional.h>
#include <util/valuelist.h>

#include <iostream>

struct grid_x
{
	template<unsigned index_i, unsigned index_j>
	struct func
	{
		typedef NUMBER_MAKE(index_i* DX) type;
	};
};

struct grid_y
{
	template<unsigned index_i, unsigned index_j>
	struct func
	{
		typedef NUMBER_MAKE(index_j* DY) type;
	};
};

#include <util/debug.h>

int main()
{
	typedef poisson<NITERS, source_b, source_b> p0;
	print_type<p0>();

/*
	std::cout << "{\"p0\":" << value_printer2d<poisson<NITERS, source_b, source_b>::type>()
		<< ",\n\"x\": " << value_printer2d<for_ij<NX, NY, grid_x>::type>()
		<< ",\n\"y\": " << value_printer2d<for_ij<NX, NY, grid_y>::type>()
		<< ",\n\"b\": " << value_printer2d<source_b>()
		<< "}";
		*/
}
