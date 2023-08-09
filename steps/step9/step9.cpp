#include "step9_params.h"

#include <util/for_i.h>

#include <util/number.h>
#include <util/zeros.h>
#include <util/conditional.h>
#include <util/valuelist.h>

#include <iostream>

typedef zeros<NY>::type boundary_near;

struct far_func
{
	template<unsigned index_j>
	struct func
	{
		typedef NUMBER_MAKE(index_j* DY) type;
	};
};

typedef for_i<NY, far_func>::type boundary_far;

struct initial_func
{
	template<unsigned index_i>
	struct func
	{
		typedef typename conditional<index_i == (NX - 1), boundary_far, boundary_near /*zeros*/>::type type;
	};
};

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

template<unsigned iteration>
struct laplace;

template<>
struct laplace<0>
{
	typedef for_i<NX, initial_func>::type type;
};


int main()
{
	std::cout << "{\"p0\":" << value_printer2d<laplace<0>::type>()
		<< ",\n\"x\": " << value_printer2d<for_ij<NX, NY, grid_x>::type>()
		<< ",\n\"y\": " << value_printer2d<for_ij<NX, NY, grid_y>::type>()
		<< "}";
}