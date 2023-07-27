set(NX 31)
set(NY 31)
set(NT 17)
set(NU 0.05)
set(SIGMA 0.25)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step7_params.h.in 
	${CMAKE_CURRENT_BINARY_DIR}/step7_params.h
	@ONLY)


add_executable(step7 step7.cpp ${CMAKE_CURRENT_BINARY_DIR}/step7_params.h)
target_include_directories(step7 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(step7 util)