#include "step11_params.h"

#include <util/point2d.h>
#include <util/typelist.h>
#include <util/zeros.h>

// Calculates one velocity step

template<typename velocities>
struct compute_velocity_step;

template<TYPENAMES_5(v_px, v_nx, v_current, v_py, v_ny), // velocity terms, p for previous, n for next
	 TYPENAMES_4(p_px, p_nx, p_py, p_ny)>		 // pressure terms
class velocity_point
{
  typedef NUMBER_MAKE(
		      POINT_GET_U(v_current)
		      - POINT_GET_U(v_current)*(DT/DX)*(POINT_GET_U(v_current) - POINT_GET_U(v_px))
		      - POINT_GET_V(v_current)*(DT/DY)*(POINT_GET_U(v_current) - POINT_GET_U(v_py))
		      - (DT/(RHO*2*DX))*(NUMBER_GET_TYPE(p_nx) - NUMBER_GET_TYPE(p_px))
		      + (NU*DT)*(
				 (POINT_GET_U(v_nx) - 2*POINT_GET_U(v_current) + POINT_GET_U(v_px))/(DX*DX*)
				 +(POINT_GET_U(v_ny) - 2*POINT_GET_U(v_current) + POINT_GET_U(v_py))/(DY*DY))
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
