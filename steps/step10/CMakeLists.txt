set(NX 50)
set(NY 50)
set(LX 2)
set(LY 1)
set(NITERS 100)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step10_params.h.in
	${CMAKE_CURRENT_BINARY_DIR}/step10_params.h
	@ONLY)

set(S10_TARGETS
	jacobi CTCFD_POISSON_JACOBI
	gs CTCFD_POISSON_GAUSS_SEIDEL
	sor CTCFD_POISSON_SOR) # sor is short for Successive Over Relaxation

while (S10_TARGETS)
	list(POP_FRONT S10_TARGETS TARGET_SUFFIX TARGET_DEFINE)
	foreach(TRACKED 0 1)
		set(TARGET_NAME step10-${TARGET_SUFFIX})
		if (${TRACKED})
			set(TARGET_NAME ${TARGET_NAME}-tracked)
		endif()

		add_executable(${TARGET_NAME} step10.cpp ${CMAKE_CURRENT_BINARY_DIR}/step10_params.h)
		target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
		target_link_libraries(${TARGET_NAME} util)
		target_compile_definitions(${TARGET_NAME} PUBLIC ${TARGET_DEFINE})
		
		if(${TRACKED})
			target_compile_definitions(${TARGET_NAME} PUBLIC CTCFD_FASTTRACKED)
		endif()
	endforeach()
endwhile()


MAKE_SOR_HEADER(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/step10_sor_omega.h
	NX ${NX}
	LX ${LX}
	NY ${NY}
	LY ${LY}
	PARAMNAME CTCFD_POISSON_SOR_OMEGA
)

foreach(target step10-sor step10-sor-tracked)
  target_sources(${target} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/step10_sor_omega.h)
endforeach()
