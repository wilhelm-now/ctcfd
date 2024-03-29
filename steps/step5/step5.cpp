#include "step5_params.h"

#include <util/typelist.h>
#include <util/number.h>

#include <util/for_i.h>
#include <util/conditional.h>

#include <util/ones.h>

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

// Boundary conditions is u = 1 at each edge
// X boundary saved here
typedef ones<NX>::type x_boundary;

// for just rows
template<typename previous_row, typename current_row>
struct compute_wave_row;

template<typename previous_x_values, typename previous_y>
struct compute_wave_row<previous_x_values, typelist<previous_y, null_t> >
{
	typedef null_t type; // end of domain at y limit
};

template<
	typename ignored_previous, // u[t, i-1, j-1]
	typename previous_x,       // u[t, i-1, j]
	typename next_previous_x,  // u[t, i-1, j + 1]
	typename previous_y,       // u[t,   i, j-1]
	typename current,          // u[t,   i, j]
	typename next              // u[t,   i, j + 1]
>
struct compute_wave_row<
	typelist<ignored_previous, typelist<previous_x, next_previous_x> >,
	typelist<previous_y, typelist<current, next> > >
{
	// u[t+1, i, j] = u[t, i, j] - (C*DT/DX)(u[t, i, j] - u[t, i-1, j]) - (C*DT/DY)(u[t, i, j] - u[t, i, j-1])
	typedef NUMBER_MAKE(
		NUMBER_GET_TYPE(current) 
		- (C*DT/DX)*(NUMBER_GET_TYPE(current) - NUMBER_GET_TYPE(previous_x)) 
		- (C*DT/DY)*(NUMBER_GET_TYPE(current) - NUMBER_GET_TYPE(previous_y))) computed;
		
	typedef typelist<computed, 
		typename compute_wave_row<
			typelist<previous_x, next_previous_x>, 
			typelist<current, next> >::type
	> type;
};

// computation for entire domain
template<typename values>
struct compute_wave;

template<typename previous_x>
struct compute_wave<typelist<previous_x, null_t> >
{
	typedef null_t type; // end of domain at x limit
};

template<typename previous_x, typename current_x, typename next_x>
struct compute_wave<typelist<previous_x, typelist<current_x, next_x> > >
{
	typedef typelist<
		typelist<NUMBER_MAKE(1), // boundary condition and rest of calculatin
		typename compute_wave_row<previous_x, current_x>::type
		>
		,
		typename compute_wave<typelist<current_x, next_x> >::type
		> type;
};

template<unsigned>
struct wave2d;

template<>
struct wave2d<0> // save initial conditions
{
	typedef for_ij<NX, NY, initial>::type type;
};

template<unsigned timestep>
struct wave2d
{
	typedef typelist<x_boundary,
		typename compute_wave<typename wave2d<timestep - 1>::type>::type> type;
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
		typedef NUMBER_MAKE(index_j * DY) type;
	};
};

int main()
{
	std::cout << "{\"u0\": " << value_printer2d<wave2d<0>::type>()
		<< ",\n\"u" << NT << "\": " << value_printer2d<wave2d<NT>::type>()
		<< ",\n\"x\": " << value_printer2d<for_ij<NX, NY, grid_x>::type>()
		<< ",\n\"y\": " << value_printer2d<for_ij<NX, NY, grid_y>::type>()
		<< "}";
}