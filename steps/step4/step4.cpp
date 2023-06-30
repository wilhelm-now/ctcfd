#include "step4_params.h"
#include "step4_initial.h"

#include <util/typelist.h>
#include <util/number.h>

#include <util/valuelist.h>

#include <iostream>


template<typename values>
struct compute_burgers;

template<typename previous, typename current>
struct compute_burgers<typelist<previous, typelist<current, null_t> > >
{
	// RHS boundary stops computation and enforces boundary condition
	typedef typename TYPELIST_1(NUMBER_MAKE(1.0)) type;
};

template<typename previous, typename current, typename next>
class compute_burgers<typelist<previous, typelist<current, next> > >
{
	// u[next_t, current_idx] = u[current_t, current_idx] + (NU*DT/(DX*DX))*(u[current_t, next_idx] - 2*u[current_t, current_idx] + u[current_t, previous_idx])
	typedef typename NUMBER_MAKE(NUMBER_GET_TYPE(current) + (NU*DT/(DX*DX)) *
		(NUMBER_GET_TYPE(next::head) - 2*NUMBER_GET_TYPE(current) + NUMBER_GET_TYPE(previous))
	)
		computed;
public:
	typedef typelist<computed, typename compute_burgers<typename typelist<current, next> >::type> type;
};

// burgers 
template<unsigned timestep>
struct burgers;


// save initial condition
template<>
struct burgers<0>
{
	typedef analytical_t0 type;
};

template<unsigned timestep>
struct burgers
{
	typedef typelist<
		// LHS boundary ignored
		typename burgers<timestep - 1>::type::head,
		// compute the rest of the domain with numerical scheme
		typename compute_burgers<typename burgers<timestep - 1>::type>::type
	> type;
};

int main()
{
	std::cout << "U0, U1 = [" << value_printer<burgers<0>::type>() << 
		"], [" << value_printer<burgers<NT>::type>() << 
		"]\n";
}

