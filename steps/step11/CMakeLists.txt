set(NX 21)
set(NY 21)
set(LX 2)
set(LY 2)
set(NITERS 18)
set(NT 20)
set(RHO 1)
set(NU 0.1)
set(DT 0.001)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step11_params.h.in
	${CMAKE_CURRENT_BINARY_DIR}/step11_params.h
	@ONLY)

set(S11_TARGETS
	jacobi CTCFD_POISSON_JACOBI
	gs CTCFD_POISSON_GAUSS_SEIDEL
	sor CTCFD_POISSON_SOR) # sor is short for Successive Over Relaxation

while (S11_TARGETS)
  list(POP_FRONT S11_TARGETS TARGET_SUFFIX TARGET_DEFINE)
  set(TARGET_NAME step11-${TARGET_SUFFIX})
  add_executable(${TARGET_NAME}
    step11.cpp
    poisson.h
    pressure_eq.h
    velocity_step.h
    ${CMAKE_CURRENT_BINARY_DIR}/step11_params.h)
  target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
  target_link_libraries(${TARGET_NAME} util)
  target_compile_definitions(${TARGET_NAME} PUBLIC ${TARGET_DEFINE})
  
  ADD_PLOT_FLOW_TARGET(
	  SOURCE_TARGET ${TARGET_NAME}
	  PLOT_ARGS --savename ${TARGET_SUFFIX}.png --title ${TARGET_DEFINE})

endwhile()

MAKE_SOR_HEADER(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/step11_sor_omega.h
	NX ${NX}
	LX ${LX}
	NY ${NY}
	LY ${LY}
	PARAMNAME CTCFD_POISSON_SOR_OMEGA
)
target_sources(step11-sor PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/step11_sor_omega.h)
