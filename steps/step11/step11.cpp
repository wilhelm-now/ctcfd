#include "step11_params.h"
#include "poisson.h"
#include "pressure_eq.h"
#include "velocity_step.h"


#include <util/for_i.h>
#include <util/number.h>
#include <util/point2d.h>
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

struct zero_velocity_func
{
	template<unsigned index_i, unsigned index_j>
	struct func
	{
		typedef POINT_MAKE((index_j == (NY - 1)) ? 1.0 : 0.0, 0.0) type;
	};
};

typedef for_ij<NX, NY, b_func>::type source_b;

typedef zeros2d<NX, NY>::type initial_pressure;
typedef for_ij<NX, NY, zero_velocity_func>::type initial_velocity;

template<unsigned iter>
struct cavity_flow;

template<>
struct cavity_flow<0>
{
	typedef initial_velocity velocity;
	typedef initial_pressure pressure;
};

template<unsigned iter>
struct cavity_flow
{
	typedef typename pressure_rhs<typename cavity_flow<iter - 1>::velocity>::type pressure_forcing;
	typedef typename poisson<NITERS, typename cavity_flow<iter - 1>::pressure, pressure_forcing>::type pressure;
	typedef typename velocity_step<typename cavity_flow<iter - 1>::velocity, pressure>::type velocity;
};


int main()
{

#define COMPONENT_AT(key, component, iter) << ",\n\"" << key << iter << "\": " << value_printer2d<cavity_flow<iter>::component>()
#define PRESSURE_AT(iter) COMPONENT_AT("p", pressure, iter)
#define VELOCITY_AT(iter) COMPONENT_AT("v", velocity, iter)
#define PRESSURE_FORCING_AT(iter) COMPONENT_AT("pf", pressure_forcing, iter)


	std::cout << "{\"NITERS\": " << NITERS << ",\n\"NT\": " << NT
		PRESSURE_FORCING_AT(1)
		PRESSURE_AT(1)
		VELOCITY_AT(1)
		PRESSURE_AT(NT)
		VELOCITY_AT(NT)
		<< ",\n\"x\": " << value_printer2d<for_ij<NX, NY, grid_x>::type>()
		<< ",\n\"y\": " << value_printer2d<for_ij<NX, NY, grid_y>::type>()
		<< "}";
}
