#include "step11_params.h"
#include "poisson.h"
#include "pressure_eq.h"


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

struct b_func
{
	template<unsigned index_i, unsigned index_j>
	struct func
	{
		typedef NUMBER_MAKE(
			index_i == NX / 4 && index_j == NY / 4 ? 100.0
			: index_i == 3 * NX / 4 && index_j == 3 * NY / 4 ? -100.0 : 0.0) type;
	};
};

typedef for_ij<NX, NY, b_func>::type source_b;

typedef zeros2d<NX, NY>::type initial;

int main()
{

#define VALUES_AT(iter) << ",\n\"p" << iter << "\": " << value_printer2d<poisson<iter, initial, source_b>::type>()
	std::cout << "{\"p0\":" << value_printer2d<poisson<0, initial, source_b>::type>()
		// VALUES_AT(1)
		// VALUES_AT(5)
		VALUES_AT(NITERS)
		<< ",\n\"x\": " << value_printer2d<for_ij<NX, NY, grid_x>::type>()
		<< ",\n\"y\": " << value_printer2d<for_ij<NX, NY, grid_y>::type>()
		<< ",\n\"b\": " << value_printer2d<source_b>()
		<< ",\n\"initial\": " << value_printer2d<initial>()
		<< "}";
}
