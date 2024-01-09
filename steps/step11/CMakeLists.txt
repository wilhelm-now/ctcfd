set(NX 21)
set(NY 21)
set(LX 2)
set(LY 2)
set(NITERS 50)
set(RHO 1)
set(NU 0.1)
set(DT 0.001)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step11_params.h.in
	${CMAKE_CURRENT_BINARY_DIR}/step11_params.h
	@ONLY)

set(S10_TARGETS
	jacobi CTCFD_POISSON_JACOBI
	gs CTCFD_POISSON_GAUSS_SEIDEL
	sor CTCFD_POISSON_SOR) # sor is short for Successive Over Relaxation

while (S10_TARGETS)
  list(POP_FRONT S10_TARGETS TARGET_SUFFIX TARGET_DEFINE)
  set(TARGET_NAME step11-${TARGET_SUFFIX})
  add_executable(${TARGET_NAME} step11.cpp poisson.h pressure_eq.h ${CMAKE_CURRENT_BINARY_DIR}/step11_params.h)
  target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
  target_link_libraries(${TARGET_NAME} util)
  target_compile_definitions(${TARGET_NAME} PUBLIC ${TARGET_DEFINE})
endwhile()

# make header with optimal successive over relaxation factor
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/step11_sor_omega.h
  COMMAND python sor_omega.py -o ${CMAKE_CURRENT_BINARY_DIR}/step11_sor_omega.h --NX ${NX} --LX ${LX} --NY ${NY} --LY ${LY} --paramname CTCFD_POISSON_SOR_OMEGA
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../common
)
target_sources(step11-sor PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/step11_sor_omega.h)
