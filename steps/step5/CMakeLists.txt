set(NX 81)
set(NY 81)
set(NT 100)
set(C 1)
set(SIGMA 0.2)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step5_params.h.in 
	${CMAKE_CURRENT_BINARY_DIR}/step5_params.h
	@ONLY)

add_executable(step5 step5.cpp ${CMAKE_CURRENT_BINARY_DIR}/step5_params.h)
target_include_directories(step5 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(step5 util)