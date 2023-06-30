#include "step3_params.h"

#include <util/typelist.h>
#include <util/number.h>

#include <util/for_i.h>
#include <util/valuelist.h>

#include <iostream>


template<typename values>
struct compute_diffusion;

template<typename previous, typename current>
struct compute_diffusion<typelist<previous, typelist<current, null_t> > >
{
	// RHS boundary stops computation and enforces boundary condition
	typedef typename TYPELIST_1(NUMBER_MAKE(1.0)) type;
};

template<typename previous, typename current, typename next>
class compute_diffusion<typelist<previous, typelist<current, next> > >
{
	// u[next_t, current_idx] = u[current_t, current_idx] + (NU*DT/(DX*DX))*(u[current_t, next_idx] - 2*u[current_t, current_idx] + u[current_t, previous_idx])
	typedef typename NUMBER_MAKE(NUMBER_GET_TYPE(current) + (NU*DT/(DX*DX)) *
		(NUMBER_GET_TYPE(next::head) - 2*NUMBER_GET_TYPE(current) + NUMBER_GET_TYPE(previous))
	)
		computed;
public:
	typedef typelist<computed, typename compute_diffusion<typename typelist<current, next> >::type> type;
};

// diffusion
template<unsigned timestep>
struct diffusion;

// initial condition meta-function, same as in step-2
template<unsigned idx>
struct initial
{
	typedef NUMBER_MAKE((idx* DX >= 0.5 && idx * DX <= 1.0) ? 2.0 : 1.0) type;
};

// apply initial condition
template<>
struct diffusion<0>
{
	typedef for_i<NX, initial>::type type;
};

template<unsigned timestep>
struct diffusion
{
	typedef typelist<
		// LHS boundary ignored
		typename diffusion<timestep - 1>::type::head,
		// compute the rest of the domain with numerical scheme
		typename compute_diffusion<typename diffusion<timestep - 1>::type>::type
	> type;
};


int main()
{	
	std::cout << "U0, U1 = [" << value_printer<diffusion<0>::type>() << 
		"], [" << value_printer<diffusion<NT>::type>() << 
		"]\n";
}

