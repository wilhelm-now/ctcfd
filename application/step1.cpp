#include <util/typelist.h>
#include <util/number.h>

#include <util/ones.h>
#include <util/range.h>
#include <util/conditional.h>
#include <util/for_i.h>

#include <util/valuelist.h>

#include <util/debug.h>

#include "step1_params.h"

#include <iostream>


template<unsigned index>
struct initial_condition
{
	typedef typename conditional<(DX*index >= 0.5 && DX*index <= 1), number_t<2>, number_t<1>>::type type;
};

template<unsigned timestep>
struct computed;

template<>
struct computed<0>
{
	typedef for_i<NX, initial_condition>::type type;
};

template<unsigned timestep>
struct computed
{
	// todo backwards diff
	// u[current_t=timestep, idx] = u[previous_t, idx] - (C*DT/DX)*(u[previous_t, idx] - u[previous_t, idx-1]
	//(C * DT * 1.0 / DX);  // scale thing
	typedef computed<0>::type type; // TODO: not this.
	typedef typelist<
		computed<timestep - 1>::type::head, // no boundary condition on LHS
		null_t> attempt;
};


int main()
{
	std::cout << PASTE_EXEC(NX) << ", " PASTE_EXEC(DX) << '\n';

	std::cout << value_printer<computed<0>::type>{} << '\n';
	std::cout << value_printer<computed<NT>::type>{} << '\n';
}
	