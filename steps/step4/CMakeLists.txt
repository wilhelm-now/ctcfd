set(NX 41)
set(NT 20)
set(NU 0.3)
set(SIGMA 0.2)
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/step4_params.h.in 
	${CMAKE_CURRENT_BINARY_DIR}/step4_params.h
	@ONLY)

add_executable(step4 step4.cpp ${CMAKE_CURRENT_BINARY_DIR}/step4_params.h)
target_include_directories(step4 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(step4 util)