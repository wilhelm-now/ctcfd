#include "step3_params.h"

#include <util/typelist.h>
#include <util/number.h>

#include <util/for_i.h>

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

int main(){}