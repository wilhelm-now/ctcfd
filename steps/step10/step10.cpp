#include "step10_params.h"
#ifdef CTCFD_POISSON_SOR
#include "step10_sor_omega.h"
#endif


#include <util/for_i.h>
#include <util/number.h>
#include <util/zeros.h>
#include <util/conditional.h>
#include <util/valuelist.h>

#include <iostream>

typedef zeros<NY>::type boundary_row;

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

typedef for_ij<NX, NY, b_func>::type source_b;

template<TYPENAMES_5(px, nx, py, ny, b)> // previous x, next x, previous y, current y, source term at this location
struct compute_poisson_point
{
	typedef NUMBER_MAKE(
		(DY * DY * (NUMBER_GET_TYPE(nx) + NUMBER_GET_TYPE(px))
		 + DX * DX * (NUMBER_GET_TYPE(ny) + NUMBER_GET_TYPE(py))
		 - DX * DX * DY * DY * NUMBER_GET_TYPE(b)) 
		/ (2 * (DX * DX + DY * DY))) type;
};

template<typename previous_x_row, typename current_x_row, typename next_x_row, typename current_b_row>
class compute_poisson_row;

//Specialization to apply boundary condition on far y-limit.
template<TYPENAMES_2(pxpy, pxcy), TYPENAMES_2(cxpy, cxcy), TYPENAMES_2(nxpy, nxcy), TYPENAMES_2(cxpy_b, cxcy_b)>
class compute_poisson_row<TYPELIST_2(pxpy, pxcy), TYPELIST_2(cxpy, cxcy), TYPELIST_2(nxpy, nxcy), TYPELIST_2(cxpy_b, cxcy_b)>
{
public:
	typedef TYPELIST_1(NUMBER_MAKE(0.0)) type;
};

template<
	TYPENAMES_4(pxpy, pxcy, pxny, pxtaily), // p[i-1, j-i], p[i-1, j], p[i-1, j+1], tail at p[i-1, ...]
	TYPENAMES_4(cxpy, cxcy, cxny, cxtaily), // p[i, j-1],   p[i, j],   p[i, j+1],   tail at p[i, ...]
	TYPENAMES_4(nxpy, nxcy, nxny, nxtaily),  // p[i+1, j-1], p[i+1, j], p[i+1, j+1], tail at p[i+1, ...]
	TYPENAMES_4(cxpy_b, cxcy_b, cxny_b, cxtaily_b) // source terms
>
class compute_poisson_row<
	TAILED_TYPELIST_3(pxpy, pxcy, pxny, pxtaily),
	TAILED_TYPELIST_3(cxpy, cxcy, cxny, cxtaily),
	TAILED_TYPELIST_3(nxpy, nxcy, nxny, nxtaily),
	TAILED_TYPELIST_3(cxpy_b, cxcy_b, cxny_b, cxtaily_b)>
{
	typedef typename compute_poisson_point<pxcy, nxcy, cxpy, cxny, cxcy_b>::type computed;
public:
	typedef typelist<computed,
		typename compute_poisson_row<
		TAILED_TYPELIST_2(pxcy, pxny, pxtaily),
#if defined(CTCFD_POISSON_GAUSS_SEIDEL)
		// Mimic gauss-seidel iterations instead of jacobi iterations by using computed for following values
		TAILED_TYPELIST_2(computed, cxny, cxtaily),
#elif defined(CTCFD_POISSON_SOR)
		TAILED_TYPELIST_2(NUMBER_MAKE((1 - CTCFD_POISSON_SOR_OMEGA)*NUMBER_GET_TYPE(cxcy) + CTCFD_POISSON_SOR_OMEGA*NUMBER_GET_TYPE(computed)), cxny, cxtaily),
#else
		TAILED_TYPELIST_2(cxcy, cxny, cxtaily),
#endif
		// finally tail
		TAILED_TYPELIST_2(nxcy, nxny, nxtaily),
		TAILED_TYPELIST_2(cxcy_b, cxny_b, cxtaily_b)>::type > type;
};

#ifdef CTCFD_FASTTRACKED
#ifdef CTCFD_POISSON_JACOBI

template<
	TYPENAMES_7(px_y1, px_y2, px_y3, px_y4, px_y5, px_y6, px_ytail),
	TYPENAMES_7(cx_y1, cx_y2, cx_y3, cx_y4, cx_y5, cx_y6, cx_ytail),
	TYPENAMES_7(nx_y1, nx_y2, nx_y3, nx_y4, nx_y5, nx_y6, nx_ytail),
	TYPENAMES_7(cx_b1, cx_b2, cx_b3, cx_b4, cx_b5, cx_b6, cx_btail)>
class compute_poisson_row<
	TAILED_TYPELIST_6(px_y1, px_y2, px_y3, px_y4, px_y5, px_y6, px_ytail),
	TAILED_TYPELIST_6(cx_y1, cx_y2, cx_y3, cx_y4, cx_y5, cx_y6, cx_ytail),
	TAILED_TYPELIST_6(nx_y1, nx_y2, nx_y3, nx_y4, nx_y5, nx_y6, nx_ytail),
	TAILED_TYPELIST_6(cx_b1, cx_b2, cx_b3, cx_b4, cx_b5, cx_b6, cx_btail)>
{
public:
	typedef typelist<typename compute_poisson_point<px_y2, nx_y2, cx_y1, cx_y3, cx_b2>::type,
		typelist<typename compute_poisson_point<px_y3, nx_y3, cx_y2, cx_y4, cx_b3>::type,
		typelist<typename compute_poisson_point<px_y4, nx_y4, cx_y3, cx_y5, cx_b4>::type,
		typelist<typename compute_poisson_point<px_y5, nx_y5, cx_y4, cx_y6, cx_b5>::type,
		typename compute_poisson_row<
			TAILED_TYPELIST_2(px_y5, px_y6, px_ytail),
			TAILED_TYPELIST_2(cx_y5, cx_y6, cx_ytail),
			TAILED_TYPELIST_2(nx_y5, nx_y6, nx_ytail),
			TAILED_TYPELIST_2(cx_b5, cx_b6, cx_btail)>::type
		> > > > type;
};


template<
	TYPENAMES_19(px_y1, px_y2, px_y3, px_y4, px_y5, px_y6, px_y7, px_y8, px_y9, px_y10, px_y11, px_y12, px_y13, px_y14, px_y15, px_y16, px_y17, px_y18, px_ytail),
	TYPENAMES_19(cx_y1, cx_y2, cx_y3, cx_y4, cx_y5, cx_y6, cx_y7, cx_y8, cx_y9, cx_y10, cx_y11, cx_y12, cx_y13, cx_y14, cx_y15, cx_y16, cx_y17, cx_y18, cx_ytail),
	TYPENAMES_19(nx_y1, nx_y2, nx_y3, nx_y4, nx_y5, nx_y6, nx_y7, nx_y8, nx_y9, nx_y10, nx_y11, nx_y12, nx_y13, nx_y14, nx_y15, nx_y16, nx_y17, nx_y18, nx_ytail),
	TYPENAMES_19(cx_b1, cx_b2, cx_b3, cx_b4, cx_b5, cx_b6, cx_b7, cx_b8, cx_b9, cx_b10, cx_b11, cx_b12, cx_b13, cx_b14, cx_b15, cx_b16, cx_b17, cx_b18, cx_btail)>
class compute_poisson_row<
	TAILED_TYPELIST_18(px_y1, px_y2, px_y3, px_y4, px_y5, px_y6, px_y7, px_y8, px_y9, px_y10, px_y11, px_y12, px_y13, px_y14, px_y15, px_y16, px_y17, px_y18, px_ytail),
	TAILED_TYPELIST_18(cx_y1, cx_y2, cx_y3, cx_y4, cx_y5, cx_y6, cx_y7, cx_y8, cx_y9, cx_y10, cx_y11, cx_y12, cx_y13, cx_y14, cx_y15, cx_y16, cx_y17, cx_y18, cx_ytail),
	TAILED_TYPELIST_18(nx_y1, nx_y2, nx_y3, nx_y4, nx_y5, nx_y6, nx_y7, nx_y8, nx_y9, nx_y10, nx_y11, nx_y12, nx_y13, nx_y14, nx_y15, nx_y16, nx_y17, nx_y18, nx_ytail),
	TAILED_TYPELIST_18(cx_b1, cx_b2, cx_b3, cx_b4, cx_b5, cx_b6, cx_b7, cx_b8, cx_b9, cx_b10, cx_b11, cx_b12, cx_b13, cx_b14, cx_b15, cx_b16, cx_b17, cx_b18, cx_btail)>
{
public:
	typedef typelist<typename compute_poisson_point<px_y2, nx_y2, cx_y1, cx_y3, cx_b2>::type,
		typelist<typename compute_poisson_point<px_y3, nx_y3, cx_y2, cx_y4, cx_b3>::type,
		typelist<typename compute_poisson_point<px_y4, nx_y4, cx_y3, cx_y5, cx_b4>::type,
		typelist<typename compute_poisson_point<px_y5, nx_y5, cx_y4, cx_y6, cx_b5>::type,
		typelist<typename compute_poisson_point<px_y6, nx_y6, cx_y5, cx_y7, cx_b6>::type,
		typelist<typename compute_poisson_point<px_y7, nx_y7, cx_y6, cx_y8, cx_b7>::type,
		typelist<typename compute_poisson_point<px_y8, nx_y8, cx_y7, cx_y9, cx_b8>::type,
		typelist<typename compute_poisson_point<px_y9, nx_y9, cx_y8, cx_y10, cx_b9>::type,
		typelist<typename compute_poisson_point<px_y10, nx_y10, cx_y9, cx_y11, cx_b10>::type,
		typelist<typename compute_poisson_point<px_y11, nx_y11, cx_y10, cx_y12, cx_b11>::type,
		typelist<typename compute_poisson_point<px_y12, nx_y12, cx_y11, cx_y13, cx_b12>::type,
		typelist<typename compute_poisson_point<px_y13, nx_y13, cx_y12, cx_y14, cx_b13>::type,
		typelist<typename compute_poisson_point<px_y14, nx_y14, cx_y13, cx_y15, cx_b14>::type,
		typelist<typename compute_poisson_point<px_y15, nx_y15, cx_y14, cx_y16, cx_b15>::type,
		typelist<typename compute_poisson_point<px_y16, nx_y16, cx_y15, cx_y17, cx_b16>::type,
		typelist<typename compute_poisson_point<px_y17, nx_y17, cx_y16, cx_y18, cx_b17>::type,
		typename compute_poisson_row<
			TAILED_TYPELIST_2(px_y17, px_y18, px_ytail),
			TAILED_TYPELIST_2(cx_y17, cx_y18, cx_ytail),
			TAILED_TYPELIST_2(nx_y17, nx_y18, nx_ytail),
			TAILED_TYPELIST_2(cx_b17, cx_b18, cx_btail)>::type
		> > > > > > > > > > > > > > > > type;
};

#endif // CTCFD_POISSON_JACOBI
#endif // CTCFD_FASTTRACKED

template<typename values, typename source_b>
class compute_poisson;

template<typename previous_x_row, typename current_x_row, typename previous_source, typename current_source>
class compute_poisson<TYPELIST_2(previous_x_row, current_x_row), TYPELIST_2(previous_source, current_source)>
{
public:
	typedef TYPELIST_1(boundary_row) type;
};

template<typename previous_x_row, typename current_x_row, typename next_x_row, typename further_x, TYPENAMES_4(previous_source, current_source, next_source, further_source)>
class compute_poisson<TAILED_TYPELIST_3(previous_x_row, current_x_row, next_x_row, further_x), TAILED_TYPELIST_3(previous_source, current_source, next_source, further_source)>
{
	typedef typename compute_poisson_row<previous_x_row, current_x_row, next_x_row, current_source>::type computed_row;
	typedef typelist<NUMBER_MAKE(0.0), computed_row> result; // Set p=0 at minimum
public:
	typedef typelist<result,
		typename compute_poisson<
#if defined(CTCFD_POISSON_GAUSS_SEIDEL) || defined(CTCFD_POISSON_SOR)
		// Mimic gauss-seidel iterations instead of jacobi iterations by using computed for following values
		TAILED_TYPELIST_2(result, next_x_row, further_x),
#else
		TAILED_TYPELIST_2(current_x_row, next_x_row, further_x), 
#endif
		TAILED_TYPELIST_2(current_source, next_source, further_source)>::type>
		type;
};

template<unsigned iteration>
struct poisson;

template<>
struct poisson<0>
{
	typedef zeros2d<NX, NY>::type type;
};

template<unsigned iteration>
struct poisson
{
	typedef typelist<boundary_row, typename compute_poisson<typename poisson<iteration - 1>::type, source_b>::type> type;
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
		typedef NUMBER_MAKE(index_j* DY) type;
	};
};


int main()
{
#define PRESSURE_AT(iteration) << ",\n\"p" << iteration << "\": " << value_printer2d<poisson<iteration>::type>()
	std::cout << "{\"p0\":" << value_printer2d<poisson<0>::type>()
		PRESSURE_AT(NITERS)
		<< ",\n\"x\": " << value_printer2d<for_ij<NX, NY, grid_x>::type>()
		<< ",\n\"y\": " << value_printer2d<for_ij<NX, NY, grid_y>::type>()
		<< ",\n\"b\": " << value_printer2d<source_b>()
		<< "}";
}
