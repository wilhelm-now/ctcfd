#include "step4_params.h"
#include "step4_initial.h"
#include "step4_final.h"

#include <util/typelist.h>
#include <util/number.h>

#include <util/valuelist.h>

#include <iostream>

// Single calculation broken out of the typelist computation to be re-used at boundary conditions
template<typename previous, typename current, typename next>
struct compute_burgers_point
{
	/*
		u[next_t, current_idx] = \
			+ u[current_t, current_idx]
			- u[current_t, current_idx] * (DT/DX) * (u[current_t, current_idx] - u[current_t, previous_idx]
			+ (NU * DT / (DX * DX)) * (u[current_t, next_idx] - 2 * u[current_t, current_idx] + u[current_t, previous_idx])
	*/
	typedef typename NUMBER_MAKE(NUMBER_GET_TYPE(current)
		- NUMBER_GET_TYPE(current) * (DT / DX) * (NUMBER_GET_TYPE(current) - NUMBER_GET_TYPE(previous))
		+ (NU * DT / (DX * DX)) * (NUMBER_GET_TYPE(next) - 2 * NUMBER_GET_TYPE(current) + NUMBER_GET_TYPE(previous))
	)
		type;
};

// compute burgers equation over a type/valuelist for a single timestep
template<typename values>
struct compute_burgers;

template<typename previous, typename current>
struct compute_burgers<typelist<previous, typelist<current, null_t> > >
{
	// stop computation, periodic boundary condition enforced at start
	typedef null_t type;
};

template<typename previous, typename current, typename next>
class compute_burgers<typelist<previous, typelist<current, next> > >
{
	typedef typename compute_burgers_point<previous, current, typename next::head>::type computed;
public:
	typedef typelist<computed, typename compute_burgers<typename typelist<current, next> >::type> type;
};

// burgers 
template<unsigned timestep>
struct burgers;

// define initial condition as specialization of solution
template<>
struct burgers<0>
{
	typedef analytical_t0 type;
};

template<unsigned timestep>
class burgers
{
	// Apply periodic boundary conditions
	typedef typename compute_burgers_point<
		typename tl::type_at<typename burgers<timestep - 1>::type, NX - 2>::type, // previous
		typename burgers<timestep - 1>::type::head, // current
		typename burgers<timestep - 1>::type::tail::head // next
	>::type computed_boundary;
public:
	typedef typename tl::append<typelist<
		computed_boundary,
		// compute the rest of the domain with numerical scheme
		typename compute_burgers<typename burgers<timestep - 1>::type>::type
	>, computed_boundary>::type type;
};

// trampolined
template<unsigned timestep>
class burgers_continued;

template<>
struct burgers_continued<90>
{
	typedef burgers<90>::type type;
};

template<unsigned timestep>
class burgers_continued
{
	// Apply periodic boundary conditions
	typedef typename compute_burgers_point<
		typename tl::type_at<typename burgers_continued<timestep - 1>::type, NX - 2>::type, // previous
		typename burgers_continued<timestep - 1>::type::head, // current
		typename burgers_continued<timestep - 1>::type::tail::head // next
	>::type computed_boundary;
public:
	typedef typename tl::append<typelist<
		computed_boundary,
		// compute the rest of the domain with numerical scheme
		typename compute_burgers<typename burgers_continued<timestep - 1>::type>::type
	>, computed_boundary>::type type;
};


#define STEP4_CONCAT_IMPL(x, y) x##y
#define STEP4_CONCAT(x, y) STEP4_CONCAT_IMPL(x, y)

int main()
{
	std::cout << "U0, U1, UA = [" << value_printer<burgers<0>::type>() << 
		"], [" << value_printer<burgers_continued<NT>::type>() <<
		"], [" << value_printer<STEP4_CONCAT(analytical_t, NT)>() <<
		"]\n";
}

