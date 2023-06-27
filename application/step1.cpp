#include <util/typelist.h>
#include <util/number.h>

#include <util/ones.h>
#include <util/range.h>
#include <util/conditional.h>
#include <util/for_i.h>

#include <util/valuelist.h>

#include "step1_params.h"

#include <iostream>

#define PASTE_EXEC(x) #x " = " << x

template<unsigned index>
struct initial_condition
{
	typedef typename conditional<(DX*index >= 0.5 && DX*index <= 1), number_t<2>, number_t<1>>::type type;
};

template<typename T>
void print_type()
{
	std::cout << __FUNCSIG__ << '\n';
}

int main()
{
	std::cout << PASTE_EXEC(NX) << ", " PASTE_EXEC(DX) << '\n';
	typedef number_t<1> one_t;
	typedef TYPELIST_4(one_t, one_t, one_t, one_t) manual_ones;
	typedef ones<4>::type generated_ones;

	print_type<manual_ones>();
	print_type<generated_ones>();

	print_type<range<0, 10, 1>::type >();

	std::cout << value_printer<range<0, 10, 2>::type >{} << '\n';
	std::cout << value_printer<range<0, 45, 4>::type >{} << '\n';

	typedef for_i<NX, initial_condition>::type initial;

	print_type<initial>();
}
