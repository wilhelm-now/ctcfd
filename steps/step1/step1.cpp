#include <util/typelist.h>
#include <util/number.h>

#include <util/for_i.h>
#include <util/valuelist.h>
#include <util/conditional.h>

#include "step1_params.h"

#include <iostream>

struct initial_condition
{
	template<unsigned index>
	struct func
	{
		typedef typename conditional<(DX* index >= 0.5 && DX * index <= 1), NUMBER_MAKE(2), NUMBER_MAKE(1)>::type type;
	};
};

template<typename values>
struct compute_wave;

template<typename T>
struct compute_wave<typelist<T, null_t> >
{
	typedef null_t type;
};

template<typename previous, typename current>
class compute_wave<typelist<previous, current> >
{
	// backwards diff
	// u[current_t=timestep, idx] = u[previous_t, idx] - (C*DT/DX)*(u[previous_t, idx] - u[previous_t, idx-1]
	enum { computed_value = (long long)(current::head::raw_value*1.0 - (C*DT*1.0/DX)*(current::head::raw_value - previous::raw_value*1.0)) };
public:
	typedef typelist<number_c< computed_value >, typename compute_wave<current>::type> type;
};


template<unsigned timestep>
struct wave_eq;

template<>
struct wave_eq<0>
{
	typedef for_i<NX, initial_condition>::type type;
};

template<unsigned timestep>
struct wave_eq
{
	typedef typelist <
		typename wave_eq<timestep - 1>::type::head, // backward diff, no data on LHS
		typename compute_wave<typename wave_eq<timestep - 1>::type>::type
		> type;
};


int main()
{
	std::cout << "U0, U1, UM = [" << value_printer<wave_eq<0>::type>()
		<< "], [" << value_printer<wave_eq<NT>::type>()
		<< "], [" << value_printer<wave_eq<NT/2>::type>()
		<< "]\n";
}
	