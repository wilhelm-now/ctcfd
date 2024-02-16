#include "step11_params.h"

#include <util/point2d.h>
#include <util/typelist.h>
#include <util/for_i.h>


/*
  Calculates one velocity step
  boundary conditions:
  u = 1 at far y limit
  u, v = 0 at every other limit
*/


template<typename velocities, typename pressures>
struct velocity_step;

template<TYPENAMES_5(v_px, v_nx, v_current, v_py, v_ny), // velocity terms, p for previous, n for next
	 TYPENAMES_4(p_px, p_nx, p_py, p_ny)>		 // pressure terms
class compute_velocity_point
{
  typedef NUMBER_MAKE(
		      POINT_GET_U(v_current)
		      - POINT_GET_U(v_current)*(DT/DX)*(POINT_GET_U(v_current) - POINT_GET_U(v_px))
		      - POINT_GET_V(v_current)*(DT/DY)*(POINT_GET_U(v_current) - POINT_GET_U(v_py))
		      - (DT/(RHO*2*DX))*(NUMBER_GET_TYPE(p_nx) - NUMBER_GET_TYPE(p_px))
		      + (NU*DT)*(
				 (POINT_GET_U(v_nx) - 2*POINT_GET_U(v_current) + POINT_GET_U(v_px))/(DX*DX)
				 +(POINT_GET_U(v_ny) - 2*POINT_GET_U(v_current) + POINT_GET_U(v_py))/(DY*DY)
				  )
		      ) u;
  typedef NUMBER_MAKE(
		      POINT_GET_V(v_current)
		      - POINT_GET_U(v_current)*(DT/DX)*(POINT_GET_V(v_current) - POINT_GET_V(v_px))
		      - POINT_GET_V(v_current)*(DT/DY)*(POINT_GET_V(v_current) - POINT_GET_V(v_py))
		      - (DT/(RHO*2*DY))*(NUMBER_GET_TYPE(p_ny) - NUMBER_GET_TYPE(p_py))
		      + (NU*DT)*(
				 (POINT_GET_V(v_nx) - 2*POINT_GET_V(v_current) + POINT_GET_V(v_px))/(DX*DX)
				 +(POINT_GET_V(v_ny) - 2*POINT_GET_V(v_current) + POINT_GET_V(v_py))/(DY*DY))
		      ) v;
public:
  typedef point2d_c<u, v> type;
};


template<typename v_prev_row, typename v_current_row, typename v_next_row, typename p_prev_row, typename p_current_row, typename p_next_row>
struct compute_velocity_row;


// specialization at far y limit = moving lid
template<
  TYPENAMES_2(v_pxpy, v_pxcy),   
  TYPENAMES_2(v_cxpy, v_cxcy),   
  TYPENAMES_2(v_nxpy, v_nxcy),   
  TYPENAMES_2(p_pxpy, p_pxcy),   
  TYPENAMES_2(p_cxpy, p_cxcy),   
  TYPENAMES_2(p_nxpy, p_nxcy)    
>
struct compute_velocity_row<
	TYPELIST_2(v_pxpy, v_pxcy),	// uv[t, i-1, j-1], uv[t, i-1, j]
	TYPELIST_2(v_cxpy, v_cxcy),	// uv[t, i, j-1], uv[t, i, j]
	TYPELIST_2(v_nxpy, v_nxcy),	// uv[t, i+1, j-1], uv[i+1, j]
	TYPELIST_2(p_pxpy, p_pxcy),	// p[t, i-1, j-1], p[t, i-1, j]
	TYPELIST_2(p_cxpy, p_cxcy),	// p[t, i, j-1], p[t, i, j]
	TYPELIST_2(p_nxpy, p_nxcy)	// p[t, i+1, j-1], p[i+1, j]
>
{
  typedef TYPELIST_1(POINT_MAKE(1.0, 0.0)) type;
};


template<
  TYPENAMES_3(v_pxpy, v_pxcy, v_pxny_tail),
  TYPENAMES_3(v_cxpy, v_cxcy, v_cxny_tail),
  TYPENAMES_3(v_nxpy, v_nxcy, v_nxny_tail),
  TYPENAMES_3(p_pxpy, p_pxcy, p_pxny_tail),
  TYPENAMES_3(p_cxpy, p_cxcy, p_cxny_tail),
  TYPENAMES_3(p_nxpy, p_nxcy, p_nxny_tail)
>
struct compute_velocity_row<
  TAILED_TYPELIST_2(v_pxpy, v_pxcy, v_pxny_tail),
  TAILED_TYPELIST_2(v_cxpy, v_cxcy, v_cxny_tail),
  TAILED_TYPELIST_2(v_nxpy, v_nxcy, v_nxny_tail),
  TAILED_TYPELIST_2(p_pxpy, p_pxcy, p_pxny_tail),
  TAILED_TYPELIST_2(p_cxpy, p_cxcy, p_cxny_tail),
  TAILED_TYPELIST_2(p_nxpy, p_nxcy, p_nxny_tail)
>
{
	typedef typelist<
		typename compute_velocity_point<v_pxcy, v_nxcy, v_cxcy, v_cxpy, typename v_cxny_tail::head, p_pxcy, p_nxcy, p_cxpy, typename p_cxny_tail::head>::type,
		typename compute_velocity_row<
			TAILED_TYPELIST_1(v_pxcy, v_pxny_tail),
			TAILED_TYPELIST_1(v_cxcy, v_cxny_tail),
			TAILED_TYPELIST_1(v_nxcy, v_nxny_tail),
			TAILED_TYPELIST_1(p_pxcy, p_pxny_tail),
			TAILED_TYPELIST_1(p_cxcy, p_cxny_tail),
			TAILED_TYPELIST_1(p_nxcy, p_nxny_tail)
		>::type
	  > type;
};


struct zero_point
{
	template<unsigned>
	struct func
	{
		typedef POINT_MAKE(0.0, 0.0) type;
	};
};

typedef for_i<NY, zero_point>::type boundary_velocity;  // for no slip boundary condition


template<typename velocities, typename pressures>
struct compute_velocity_step;

template<TYPENAMES_2(vprev, vcurrent), TYPENAMES_2(pprev, pcurrent)>
struct compute_velocity_step<TYPELIST_2(vprev, vcurrent), TYPELIST_2(pprev, pcurrent)>
{
	typedef TYPELIST_1(boundary_velocity) type;
};

template<TYPENAMES_4(v0, v1, v2, vtail), TYPENAMES_4(p0, p1, p2, ptail)>
struct compute_velocity_step<TAILED_TYPELIST_3(v0, v1, v2, vtail), TAILED_TYPELIST_3(p0, p1, p2, ptail)>
{
private:
	typedef typelist<POINT_MAKE(0.0, 0.0), typename compute_velocity_row<v0, v1, v2, p0, p1, p2>::type> computed;
public:
	typedef typelist<computed,
		typename compute_velocity_step<TAILED_TYPELIST_2(v1, v2, vtail), TAILED_TYPELIST_2(p1, p2, ptail)>::type
	> type;
};


template<typename velocities, typename pressures>
struct velocity_step
{
	typedef typelist<boundary_velocity,
		typename compute_velocity_step<velocities, pressures>::type> type;
};