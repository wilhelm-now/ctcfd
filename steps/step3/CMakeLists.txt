set(NX 41)
set(NT 20)
set(NU 0.3)
set(SIGMA 0.2)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step3_params.h.in 
	${CMAKE_CURRENT_BINARY_DIR}/step3_params.h
	@ONLY)

add_executable(step3 step3.cpp ${CMAKE_CURRENT_BINARY_DIR}/step3_params.h)
target_include_directories(step3 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(step3 util)