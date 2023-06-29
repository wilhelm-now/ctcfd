#include "step2_params.h"

#include <util/typelist.h>
#include <util/number.h>

#include <util/for_i.h>
#include <util/valuelist.h>

#include <iostream>

template<typename values>
struct compute_convection;

// Stop iteration at end of domain/typelist
template<typename T>
struct compute_convection<typelist<T, null_t> >
{
	typedef null_t type;
};

template<typename previous, typename current>
class compute_convection<typelist<previous, current> >
{
	// shoulde be u[i] - u[i]*(DT/DX)*(u[i] - u[i-1])
	typedef typename NUMBER_MAKE(NUMBER_GET_TYPE(current::head) - NUMBER_GET_TYPE(current::head) * (DT * 1.0 / DX) * (NUMBER_GET_TYPE(current::head) - NUMBER_GET_TYPE(previous))) computed;
public:
	typedef typelist<computed, typename compute_convection<current>::type> type;
};

// non-linear convection
template<unsigned timestep>
struct convection;

// initial condition meta-function
template<unsigned idx>
struct initial
{
	typedef NUMBER_MAKE((idx* DX >= 0.5 && idx * DX <= 1.0) ? 2.0 : 1.0) type;
};

// apply initial condition
template<>
struct convection<0>
{
	typedef for_i<NX, initial>::type type;
};

template<unsigned timestep>
struct convection
{
	typedef typelist<
		// still backward diff in space so don't change first point
		typename convection<timestep - 1>::type::head, 
		// but compute the rest of the domain
		typename compute_convection<typename convection<timestep - 1>::type>::type
	> type;
};

int main()
{
	std::cout << "U0, U1 = [" << value_printer<convection<0>::type>() << "]"
		<< ", [" << value_printer<convection<NT>::type>() << "]"
		<< '\n';
}