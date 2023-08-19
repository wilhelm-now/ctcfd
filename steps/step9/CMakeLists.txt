set(NX 31)
set(NY 31)
set(LX 2)
set(LY 1)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step9_params.h.in
	${CMAKE_CURRENT_BINARY_DIR}/step9_params.h
	@ONLY)

add_executable(step9 step9.cpp ${CMAKE_CURRENT_BINARY_DIR}/step9_params.h l1_norm.h)
target_include_directories(step9 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(step9 util)

set(S9_EXTRA_TARGETS
	gs CTCFD_LAPLACE_GAUSS_SEIDEL
	sor CTCFD_LAPLACE_SOR) # sor is short for Successive Over Relaxation

while (S9_EXTRA_TARGETS)
	list(POP_FRONT S9_EXTRA_TARGETS TARGET_SUFFIX TARGET_DEFINE)
	add_executable(step9-${TARGET_SUFFIX} step9.cpp ${CMAKE_CURRENT_BINARY_DIR}/step9_params.h l1_norm.h)
	target_include_directories(step9-${TARGET_SUFFIX} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
	target_link_libraries(step9-${TARGET_SUFFIX} util)
	target_compile_definitions(step9-${TARGET_SUFFIX} PUBLIC ${TARGET_DEFINE})
endwhile()

# make header with optimal successive over relaxation factor
add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/step9_sor_omega.h
    COMMAND python step9_sor_omega.py -o ${CMAKE_CURRENT_BINARY_DIR}/step9_sor_omega.h --NX ${NX} --LX ${LX} --NY ${NY} --LY ${LY}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
target_sources(step9-sor PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/step9_sor_omega.h)